#include "lumina-wm/EWMH.h"

EWMH::EWMH(const Window& ID){
  AID = ID;
}

EWMH::~EWMH(){

}

// ===== Public Functions =====
QString EWMH::getTitle() const{
  unsigned char* prop;
  unsigned long num;
  QString title;
  if( getProperty(getAtom("_NET_WM_NAME"), getAtom("UTF8_STRING"), &prop, &num) ){
    title = QString::fromUtf8( (char*) prop);
    XFree(prop);
  }
  return title;
}

QString EWMH::getVisibleTitle() const{
  unsigned char* prop;
  unsigned long num;
  QString title;
  if( getProperty(getAtom("_NET_WM_VISIBLE_NAME"), getAtom("UTF8_STRING"), &prop, &num) ){
    title = QString::fromUtf8( (char*) prop);
    XFree(prop);
  }
  return title;	
}

QPixmap EWMH::getIconPixmap() const{
  unsigned char* prop;
  unsigned long num;
  QPixmap pix;
  //Need the icon size first
  int iWidth;
  if( getProperty(getAtom("_NET_WM_ICON"), XA_CARDINAL, &prop, &num, 0, 1) ){
    iWidth = prop[0];
    XFree(prop);
  }else{ return pix; }
  
  int iHeight;
  if( getProperty(getAtom("_NET_WM_ICON"), XA_CARDINAL, &prop, &num, 1, 1) ){
    iHeight = prop[0];
    XFree(prop);
  }else{ return pix; }
  
  //Now get the pixmap
  if( getProperty(getAtom("_NET_WM_ICON"), XA_CARDINAL, &prop, &num, 2, iWidth*iHeight) ){
    QImage tmp(prop, iWidth, iHeight, QImage::Format_ARGB32);
    pix = QPixmap::fromImage(tmp);
    XFree(prop);
  }
  
  return pix;	
}

Atom EWMH::getWindowType() const{
  unsigned char* prop;
  unsigned long num;
  Atom out;
  if( getProperty(LWMUtils::getAtom("_NET_WM_WINDOW_TYPE"), XA_ATOM, &prop, &num) ){
    Atom* types = (Atom*)prop; //convert to a list of the atom property types
    out = types[0]; //only need the first type
    XFree(prop);
  }else{
    out = LWMUtils::getAtom("_NET_WM_WINDOW_TYPE_NORMAL");	  
  }
  return out;
}

// ========================
//   STATIC EWMH SETTERS
// ========================
void EWMH::setSupportedHints(){
  const char* supportedHints[] = {
  	// EWMH - Root
        "_NET_SUPPORTED",
        "_NET_CLIENT_LIST",
        "_NET_CLIENT_LIST_STACKING",
        "_NET_ACTIVE_WINDOW",
        "_NET_SUPPORTING_WM_CHECK",
        "_NET_NUMBER_OF_DESKTOPS",
        "_NET_DESKTOP_NAMES",
        "_NET_DESKTOP_GEOMETRY",
        "_NET_DESKTOP_VIEWPORT",
        "_NET_WORKAREA",
        "_NET_CURRENT_DESKTOP",
        "_NET_SHOWING_DESKTOP",
        // "_NET_VIRTUAL_ROOTS",
        // "_NET_DESKTOP_LAYOUT",

        "_NET_CLOSE_WINDOW",
        "_NET_MOVERESIZE_WINDOW",
        "_NET_WM_MOVERESIZE",
        "_NET_RESTACK_WINDOW",
        // "_NET_REQUEST_FRAME_EXTENTS",

        //----------------------------------------------------------------------

        // EWMH - Client
        "_NET_WM_NAME",
        "_NET_WM_VISIBLE_NAME",
        "_NET_WM_ICON_NAME",
        "_NET_WM_VISIBLE_ICON_NAME",
        "_NET_WM_ICON",
        //"_NET_WM_DESKTOP",

        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_WINDOW_TYPE_DESKTOP",
        "_NET_WM_WINDOW_TYPE_DOCK",
        "_NET_WM_WINDOW_TYPE_TOOLBAR",
        "_NET_WM_WINDOW_TYPE_MENU",
        "_NET_WM_WINDOW_TYPE_UTILITY",
        "_NET_WM_WINDOW_TYPE_SPLASH",
        "_NET_WM_WINDOW_TYPE_DIALOG",
        "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU",
        "_NET_WM_WINDOW_TYPE_POPUP_MENU",
        "_NET_WM_WINDOW_TYPE_TOOLTIP",
        "_NET_WM_WINDOW_TYPE_NOTIFICATION",
        "_NET_WM_WINDOW_TYPE_COMBO",
        "_NET_WM_WINDOW_TYPE_DND",
        "_NET_WM_WINDOW_TYPE_NORMAL",

        /*
        "_NET_WM_STATE",
        "_NET_WM_STATE_MODAL",
        "_NET_WM_STATE_STICKY",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_SHADED",
        "_NET_WM_STATE_SKIP_TASKBAR",
        "_NET_WM_STATE_SKIP_PAGER",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_FULLSCREEN",
        "_NET_WM_STATE_ABOVE",
        "_NET_WM_STATE_BELOW",
        "_NET_WM_STATE_DEMANDS_ATTENTION",

        "_NET_WM_ALLOWED_ACTIONS",
        "_NET_WM_ACTION_MOVE",
        "_NET_WM_ACTION_RESIZE",
        "_NET_WM_ACTION_MINIMIZE",
        "_NET_WM_ACTION_SHADE",
        "_NET_WM_ACTION_STICK",
        "_NET_WM_ACTION_MAXIMIZE_HORZ",
        "_NET_WM_ACTION_MAXIMIZE_VERT",
        "_NET_WM_ACTION_FULLSCREEN",
        "_NET_WM_ACTION_CHANGE_DESKTOP",
        "_NET_WM_ACTION_CLOSE",
        "_NET_WM_ACTION_ABOVE",
        "_NET_WM_ACTION_BELOW",

        "_NET_WM_STRUT",
        "_NET_WM_STRUT_PARTIAL",
        "_NET_WM_ICON_GEOMETRY",
        "_NET_WM_PID",
        "_NET_WM_HANDLED_ICONS",
        "_NET_WM_USER_TIME_WINDOW",
        "_NET_FRAME_EXTENTS",
        */

        "_NET_WM_USER_TIME"  
  };
  int num = sizeof(supportedHints)/sizeof(char*);
  Atom aList[num];
  for(int i=0; i<num; i++){
    aList[i] = LWMUtils::getAtom(supportedHints[i]);	  
  }
  
  XChangeProperty(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()),
  	  LWMUtils::getAtom("_NET_SUPPORTED"), XA_ATOM, 32, PropModeReplace, 
  	  (const unsigned char*) &aList, num); 
}
// ================================================
void EWMH::setClientList(QList<LWindow*> *list){
  //Set the list of clients to the _NET_CLIENT_LIST atom
  int winNum = list->size();
  Window winList[winNum];
  for(int i=0; i<winNum; i++){
    winList[i] = list->at(i)->getAppID();  
  }
  
  XChangeProperty(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()),
  	  LWMUtils::getAtom("_NET_CLIENT_LIST"), XA_WINDOW, 32, PropModeReplace, 
  	  (const unsigned char*) &winList, numWin); 
}
// ================================================
void EWMH::setClientStackList(QList<LWindow*> list){
  //Set the list of clients to the _NET_CLIENT_LIST_STACKING atom
  int winNum = list->size();
  Window winList[winNum];
  for(int i=0; i<winNum; i++){
    winList[i] = list->at(i)->getAppID();  
  }
  
  XChangeProperty(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()),
  	  LWMUtils::getAtom("_NET_CLIENT_LIST_STACKING"), XA_WINDOW, 32, PropModeReplace, 
  	  (const unsigned char*) &winList, numWin); 	
}
// ================================================
void EWMH::setSupportingWmCheck(Window ID){
  XChangeProperty(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()),
  	  LWMUtils::getAtom("_NET_SUPPORTING_WM_CHECK"), XA_WINDOW, 32, PropModeReplace, 
  	  (const unsigned char*) &ID, 1); 
}
// ================================================
void EWMH::setNumberOfDesktops(long num){
  XChangeProperty(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()),
  	  LWMUtils::getAtom("_NET_NUMBER_OF_DESKTOPS"), XA_WINDOW, 32, PropModeReplace, 
  	  (const unsigned char*) &num, 1); 
}
// ================================================

