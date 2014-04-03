//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LuminaX11.h"

//=====   WindowList() ========
QList<WId> LX11::WindowList(){
  QList<WId> output;
  //Method 1 - EWMH
  /*Atom a = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST", true);
  Atom realType;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(), a, 0L, (~0L),
  	     false, AnyPropertyType, &realType, &format, &num, &bytes, &data);
  if( (status >= Success) && (num > 0) ){
    for(unsigned int i=0; i<num; i++){
       Window win = (Window) data[i];
       if(win != 0){ output << win; }
    }
    XFree(data);
  }*/
	
  //Method 2 - ICCC
  output = LX11::findChildren(QX11Info::appRootWindow(),1);
  //Now go through them and filter out all the invalid windows
  Display *disp = QX11Info::display();

  for(int i=0; i<output.length(); i++){
    //Now filter
    bool good = false;
    QString name = LX11::WindowName(output[i]);
    if( !name.isEmpty() && !name.startsWith("LuminaDesktop") && name!="PCDM-session" ){ //not an ICCC compliant window (or the desktop)
      //Now check for a usable window (either visible/invisible)
      XWMHints *hints = XGetWMHints(disp, output[i]);
      if( hints!=0 ){ 
        if(hints->initial_state != WithdrawnState ){ good=true; }
	XFree(hints);
      }
    }
    //Remove item from list if not good
    if(!good){
      output.removeAt(i);
      i--; //make sure we don't miss a window since the list index/length got decreased 
    }
  }
  
  return output;
}

// ===== findChildren() =====
QList<WId> LX11::findChildren(Window parent, int levels){
  Window rootR, parentR;
  Window *childrenR;
  unsigned int num;
  int stat = XQueryTree(QX11Info::display(), parent, &rootR, &parentR, &childrenR, &num);
  QList<WId> output;
  if(stat != 0 && num > 0){
    for(int i=0; i<int(num); i++){
      output << childrenR[i];
      if(levels > 0){
       output << LX11::findChildren(childrenR[i], levels-1); //call this recursively
      }
    }
    XFree(childrenR);
  }	
  return output;
}

// ===== ActiveWindow() =====
WId LX11::ActiveWindow(){
  Display *disp = QX11Info::display();
  Atom SA = XInternAtom(disp, "_NET_ACTIVE_WINDOW", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, QX11Info::appRootWindow() , SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
  WId window=0;
  if(status >= Success && data){
    window = Window( data );
    XFree(data);
  }
  return window;  		
}

// ===== CloseWindow() =====
void LX11::CloseWindow(WId win){
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

// ===== IconifyWindow() =====
void LX11::IconifyWindow(WId win){
  XIconifyWindow(QX11Info::display(), win, QX11Info::appScreen());	
}

// ===== RestoreWindow() =====
void RestoreWindow(WId win){
  Display *disp = QX11Info::display();
    XMapWindow(disp, win); //make it visible again
    XRaiseWindow(disp, win); //raise it to the top of the stack
}

// ===== WindowName() =====
QString LX11::WindowName(WId win){
  QString nm = LX11::getNetWMProp(win, "_NET_WM_NAME");
  if(nm.isEmpty()){ 
    char *txt;
    if( XFetchName(QX11Info::display(), win, &txt) != 0){
      nm = QString(txt);
    }
    XFree(txt);
  }
  return nm;
}

// ===== WindowVisibleName() =====
QString LX11::WindowVisibleName(WId win){
  return LX11::getNetWMProp(win, "_NET_WM_VISIBLE_NAME");	
}

// ===== WindowIconName() =====
QString LX11::WindowIconName(WId win){
  return LX11::getNetWMProp(win, "_NET_WM_ICON_NAME");	
}

// ===== WindowVisibleIconName() =====
QString LX11::WindowVisibleIconName(WId win){
  return LX11::getNetWMProp(win, "_NET_WM_VISIBLE_ICON_NAME");	
}

// ===== GetWindowState() =====
LX11::WINDOWSTATE LX11::GetWindowState(WId win, bool forDisplay){
  //forDisplay lets the function know whether it needs to follow the TaskBar/Pager ignore rules
  Display *disp = QX11Info::display();
  Atom SA = XInternAtom(disp, "_NET_WM_STATE", false);
  Atom ATTENTION = XInternAtom(disp, "NET_WM_STATE_DEMANDS_ATTENTION", false);
  Atom SKIPP = XInternAtom(disp, "NET_WM_STATE_SKIP_PAGER", false);
  Atom HIDDEN = XInternAtom(disp, "NET_WM_STATE_HIDDEN", false);
  Atom SKIPT = XInternAtom(disp, "NET_WM_STATE_SKIP_TASKBAR", false);
  Atom type;
  int format;
  unsigned long num, bytes;
  unsigned char *data = 0;
  int status = XGetWindowProperty( disp, win, SA, 0, ~(0L), false, AnyPropertyType,
  	  			&type, &format, &num, &bytes, &data);
  LX11::WINDOWSTATE state = LX11::VISIBLE;
  if(status >= Success && data){
    for(unsigned int i=0; i<num; i++){
      if(forDisplay && (data[i] == SKIPP || data[i]==SKIPT) ){
      	state = LX11::IGNORE;
      	break;
      }else if(data[i]==HIDDEN){
	state = LX11::INVISIBLE;
      }else if(data[i]==ATTENTION){
	state = LX11::ATTENTION;
      }
    }
    XFree(data);
  }
  return state;  	
}

// ===== getNetWMProp() =====
QString LX11::getNetWMProp(WId win, QString prop){
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
