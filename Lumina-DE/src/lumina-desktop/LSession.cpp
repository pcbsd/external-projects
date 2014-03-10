//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LSession.h"

Window LuminaSessionTrayID;

LSession::LSession(int &argc, char ** argv) : QApplication(argc, argv){
  this->setApplicationName("Lumina Desktop Environment");
  this->setApplicationVersion("0.0.2");
  this->setOrganizationName("LuminaDesktopEnvironment");
  this->setQuitOnLastWindowClosed(false); //since the LDesktop's are not necessarily "window"s
  LuminaSessionTrayID = 0;
}

LSession::~LSession(){

}

bool LSession::x11EventFilter(XEvent *event){
  //Detect X Event types and send the appropriate signal(s)
   switch(event->type){
  // -------------------------
    case ClientMessage:
    	//Only check if the client is the system tray, otherwise ignore
    	if(event->xany.window == LuminaSessionTrayID){
    	  //qDebug() << "SysTray: ClientMessage";
    	  parseClientMessageEvent(&(event->xclient));
    	}
    	break;
    case SelectionClear:
    	if(event->xany.window == LuminaSessionTrayID){
    	  //qDebug() << "SysTray: Selection Clear";
    	  this->CloseSystemTray();
    	}
    	break;
    case MapNotify:      //window mapped (visible)
    case UnmapNotify:    //window unmapped (invisible)
    //case VisibilityNotify: //window subsection visibility changed
    case ReparentNotify: //window re-parented
    case DestroyNotify:  //window destroyed
    case CreateNotify:   //window created
    	emit WindowListEvent();
    	
  }
  // -----------------------

  //Now continue on with the event handling (don't change it)
  return false;
}

//=================
//   SYSTEM TRAY
//=================
bool LSession::StartupSystemTray(){
  qDebug() << "Starting System Tray";
  //Setup the freedesktop standards compliance
  Display *disp = QX11Info::display();
  Window root = QX11Info::appRootWindow();
  
  //Get the appropriate atom for this screen
  QString str = QString("_NET_SYSTEM_TRAY_S%1").arg(DefaultScreen(disp));
  qDebug() << "Default Screen Atom Name:" << str;
  Atom _NET_SYSTEM_TRAY_S = XInternAtom(disp,str.toAscii(),false);
  //Make sure that there is no other system tray running
  if(XGetSelectionOwner(disp, _NET_SYSTEM_TRAY_S) != None){
    qWarning() << "An alternate system tray is currently in use";
    return false;
  }
  //Create a simple window to register as the tray (not visible)
  LuminaSessionTrayID = XCreateSimpleWindow(disp, root,-1,-1,1,1,0,0,0);
  //register this widget as the system tray
  XSetSelectionOwner(disp, _NET_SYSTEM_TRAY_S, LuminaSessionTrayID, CurrentTime);
  //Make sure that it was registered properly
  if(XGetSelectionOwner(disp, _NET_SYSTEM_TRAY_S) != LuminaSessionTrayID){
    qWarning() << "Could not register the system tray";
    return false;
  }
  //Now register the orientation of the system tray
  //int horz = _NET_SYSTEM_TRAY_ORIENTATION_HORZ;
  //XChangeProperty(disp, trayID, XInternAtom(disp,"_NET_SYSTEM_TRAY_ORIENTATION",true),
  	  	//XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &horz, 1);
  //Now get the visual ID for the system tray
  XVisualInfo *XVI = new XVisualInfo;
    XVI->screen = QX11Info::appScreen();
    XVI->depth = 32;
    XVI->c_class = TrueColor;
  int num;
  XVI = XGetVisualInfo(disp, VisualScreenMask | VisualDepthMask | VisualClassMask , XVI, &num);
  VisualID vis = 0;
  if(XVI != 0){
    XRenderPictFormat *fmt;
    for(int i=0; i<num; i++){
      fmt = XRenderFindVisualFormat(disp, XVI[i].visual);
      if( (fmt !=0) && (fmt->type == PictTypeDirect) && (fmt->direct.alphaMask!=0) ){
        vis = XVI[i].visualid;
        break;
      }
    }
  }
  XFree(XVI); //done with this - clean it up
  //Now register the visual ID
  if(vis!=0){
    XChangeProperty(disp, LuminaSessionTrayID, XInternAtom(disp,"_NET_SYSTEM_TRAY_VISUAL",true),
  	  	XA_VISUALID, 32, PropModeReplace, (unsigned char*) &vis, 1);	  
  }
  //Finally, send out an X event letting others know that the system tray is up and running
  XClientMessageEvent msg;
    msg.type = ClientMessage;
    msg.window = root;
    msg.message_type = XInternAtom(disp,"MANAGER",true);
    msg.format = 32;
    msg.data.l[0] = CurrentTime;
    msg.data.l[1] = _NET_SYSTEM_TRAY_S;
    msg.data.l[2] = LuminaSessionTrayID;
    msg.data.l[3] = 0;
    msg.data.l[4] = 0;
  XSendEvent(disp, root, False, StructureNotifyMask, (XEvent*)&msg);
  
  //Success
  return true;
}

bool LSession::CloseSystemTray(){
  XDestroyWindow(QX11Info::display(), LuminaSessionTrayID);
  return true; //no additional checks for success at the moment
}

void LSession::parseClientMessageEvent(XClientMessageEvent *event){
  unsigned long opcode = event->data.l[1];
  switch(opcode){
    case SYSTEM_TRAY_REQUEST_DOCK:
        emit NewSystemTrayApp(event->data.l[2]); //Window ID
        break;
    case SYSTEM_TRAY_BEGIN_MESSAGE:
        //Let the window manager handle the pop-up messages for now
        break;    	    
    case SYSTEM_TRAY_CANCEL_MESSAGE:
        //Let the window manager handle the pop-up messages for now
        break;
    /*default:
    	//Unknown system tray operation code
    	opcode=1; //junk operation for compiling purposes*/
  }
}

//============================
//   WINDOW LIST OPERATIONS
//============================
QList<WId> LSession::WindowList(WId root){
  QList<WId> output;
  if(root == 0){ root = QX11Info::appRootWindow(); }
  Atom a = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST", true);
  Atom realType;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty(QX11Info::display(), root, a, 0L, (~0L),
  	     false, AnyPropertyType, &realType, &format, &num, &bytes, &data);
  if( (status >= Success) && (num > 0) ){
    Q_ASSERT(format == 32);
    quint32 *array = (quint32*) data;
    for(quint32 i=0; i<num; i++){
      output << (Window) array[i];
    }
    XFree(data);
  }
  return output;
}

void LSession::CloseWindow(WId win){
XClientMessageEvent msg;
    msg.type = ClientMessage;
    msg.window = win;
    msg.message_type = XInternAtom(QX11Info::display(),"_NET_CLOSE_WINDOW",true);
    msg.format = 32;
    msg.data.l[0] = CurrentTime;
    msg.data.l[1] = 2; //Direct user interaction
    msg.data.l[2] = 0;
    msg.data.l[3] = 0;
    msg.data.l[4] = 0;
  XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False, StructureNotifyMask, (XEvent*)&msg);		
}

QString LSession::WindowName(WId win){
  return LSession::getNetWMProp(win, "_NET_WM_NAME");
}

QString LSession::WindowVisibleName(WId win){
  return LSession::getNetWMProp(win, "_NET_WM_VISIBLE_NAME");	
}

QString LSession::WindowIconName(WId win){
  return LSession::getNetWMProp(win, "_NET_WM_ICON_NAME");	
}

QString LSession::WindowVisibleIconName(WId win){
  return LSession::getNetWMProp(win, "_NET_WM_VISIBLE_ICON_NAME");	
}

bool LSession::WindowRequiresAttention(WId win){
  Display *disp = QX11Info::display();
  Atom SA = XInternAtom(disp, "_NET_WM_STATE", false);
  Atom LookFor = XInternAtom(disp, "NET_WM_STATE_DEMANDS_ATTENTION", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, win, SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
  bool attention = false;
  if(status >= Success && data){
    for(unsigned int i=0; i<num; i++){
      if(data[i] == LookFor){
      	attention = true;
      	break;
      }
    }
    XFree(data);
  }
  return attention;  	
}

QString LSession::getNetWMProp(WId win, QString prop){
  Display *disp = QX11Info::display();
  Atom NA = XInternAtom(disp, prop.toUtf8(), false);
  Atom utf = XInternAtom(disp, "UTF8_STRING", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, win, NA, 0, 65536, false, utf,
  	  			&type, &format, &num, &bytes, &data);
  QString property;
  if(status >= Success && data){
    property = QString::fromUtf8( (char *) data);
    XFree(data);
  }
  return property;
}
