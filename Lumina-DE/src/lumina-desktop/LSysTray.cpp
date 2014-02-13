//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LSysTray.h"

LSysTray::LSysTray(QWidget *parent) : QWidget(parent){
  this->setContentsMargins(0,0,0,0);
  layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    layout->setSpacing(2);
  this->setLayout(layout);
  //this->setFrameWidth(1);
  //this->setFrameStyle(QFrame::Panel | QFrame::Raised);
  //this->setLineWidth(1);
  //this->setStyleSheet("QFrame { background-color: black; }");
  //this->setFixedHeight(22);
  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  trayID = 0;
  isRunning = false;
}

LSysTray::~LSysTray(){
 if(isRunning){
   this->stop();
 }
}

void LSysTray::start(){
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
    return;
  }
  //Create a simple window to register as the tray (not visible)
  trayID = XCreateSimpleWindow(disp, root,-1,-1,1,1,0,0,0);
  //register this widget as the system tray
  XSetSelectionOwner(disp, _NET_SYSTEM_TRAY_S, trayID, CurrentTime);
  //Make sure that it was registered properly
  if(XGetSelectionOwner(disp, _NET_SYSTEM_TRAY_S) != trayID){
    qWarning() << "Could not register the system tray";
    return;
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
    XChangeProperty(disp, trayID, XInternAtom(disp,"_NET_SYSTEM_TRAY_VISUAL",true),
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
    msg.data.l[2] = trayID;
    msg.data.l[3] = 0;
    msg.data.l[4] = 0;
  XSendEvent(disp, root, False, StructureNotifyMask, (XEvent*)&msg);
  
  //Now connect the application signals to the X event parser
  connect(LSession::instance(),SIGNAL(SystemTrayEvent(XEvent*)),this,SLOT(parseX11Event(XEvent*)) );
  qDebug() << "System Tray Started Successfully";
  isRunning = true;
}

void LSysTray::stop(){
  //Release all the tray applications and delete the containers
  for(int i=(trayIcons.length()-1); i>=0; i--){
    trayIcons[i]->discardClient();
    delete trayIcons.takeAt(i);
  }
  //Now destroy the invisible "tray"
  if(trayID!=0){
    XDestroyWindow(QX11Info::display(), trayID);
    trayID = 0;
  }
  disconnect(this);
  isRunning = false;
}

// ====  X Event Filter ====
void LSysTray::parseX11Event(XEvent *event){
  //This public slot will be connected (indirectly) to the main application X filter system
  switch(event->type){
    case ClientMessage:
    	//qDebug() << "SysTray: ClientMessage";
    	if(event->xany.window == trayID){
    	  parseClientMessageEvent(&(event->xclient));
    	}
    	break;
    case SelectionClear:
    	//qDebug() << "SysTray: Selection Clear";
    	this->stop();
    	//break;
    //case DestroyNotify:
    	//This event happens every time the icon changes
    	// - let the container signal handle when the app truly closes
    	
  }
  
}


// ========================
//    PRIVATE FUNCTIONS
// ========================
void LSysTray::addTrayIcon(Window win){
  if(win == 0 || !isRunning){ return; }
  //qDebug() << "System Tray: Add Tray Icon:" << win;
  bool exists = false;
  for(int i=0; i<trayIcons.length(); i++){
    if(trayIcons[i]->clientWinId() == win){ exists=true; break; }
  }
  if(!exists){
    //qDebug() << " - New Icon";
    QX11EmbedContainer *cont = new QX11EmbedContainer(this);
      cont->setFixedSize(22,22);
    //Now embed the client
    cont->embedClient(win);
    connect(cont,SIGNAL(clientIsEmbedded()),this,SLOT(updateStatus()) );
    connect(cont,SIGNAL(clientClosed()),this,SLOT(trayAppClosed()) );
    trayIcons << cont;
    layout->addWidget(cont);
    //Set the background on the client window
    //XReparentWindow(QX11Info::display(), win, cont->winId(), 0, 0);
    XSetWindowBackgroundPixmap(QX11Info::display(), win, ParentRelative);
  }
}

void LSysTray::parseClientMessageEvent(XClientMessageEvent *event){
  //qDebug() << "System Tray: Parse Client Message Event";
  unsigned long opcode = event->data.l[1];
  switch(opcode){
    case SYSTEM_TRAY_REQUEST_DOCK:
        addTrayIcon(event->data.l[2]); //Window ID
        break;
    case SYSTEM_TRAY_BEGIN_MESSAGE:
        //Let the window manager handle the pop-up messages for now
        break;    	    
    case SYSTEM_TRAY_CANCEL_MESSAGE:
        //Let the window manager handle the pop-up messages for now
        break;
    default:
    	//Unknown system tray operation code
    	opcode=1; //junk operation for compiling purposes
  }
}

void LSysTray::updateStatus(){
  qDebug() << "System Tray: Client Attached";
  //Make sure that each icon has the appropriate background color
  /*for(int i=0;  i<trayIcons.length(); i++){
    if(trayIcons[i]->clientWinId() != 0){
      XSetWindowBackgroundPixmap(QX11Info::display(), trayIcons[i]->clientWinId(), ParentRelative);
      XSetWindowBackground(QX11Info::display(), trayIcons[i]->clientWinId(), CopyFromParent);
    }
  }*/
}

void LSysTray::trayAppClosed(){
  for(int i=0;  i<trayIcons.length(); i++){
    if(trayIcons[i]->clientWinId() == 0){
      qDebug() << "System Tray: Removing icon";
      QX11EmbedContainer *cont = trayIcons.takeAt(i);
      layout->removeWidget(cont);
      delete cont;
    }
  }
  layout->update(); //update the layout (no gaps)
  this->update();	
}

