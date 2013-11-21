#include "lumina-wm/ICCCM.h"

const unsigned int ICCCM::DEFAULT_MIN_SIZE = 10;
const unsigned int ICCCM::DEFAULT_MAX_SIZE = 10000;

ICCCM::ICCCM(const Window& ID){
  AID = ID;
}

ICCCM::~ICCCM(){
}

// ===== Main Access Functions =====
// ==== Getters ====
QString ICCCM::getTitle() const{
 XTextProperty prop;
 XGetWMName(QX11Info::display(), AID, &prop);
 return XTextPropertyToQString(prop);
}

QString ICCCM::getShortTitle() const{
 XTextProperty prop;
 XGetWMIconName(QX11Info::display(), AID, &prop);
 return XTextPropertyToQString(prop);	
}

int ICCCM::getMinWidth() const{
  XSizeHints xsh = getSizeHints();
  if(xsh.flags & PMinSize){
    return xsh.min_width;
  }else{
    return DEFAULT_MIN_SIZE;	  
  }
}

int ICCCM::getMinHeight() const{
  XSizeHints xsh = getSizeHints();
  if(xsh.flags & PMinSize){
    return xsh.min_height;
  }else{
    return DEFAULT_MIN_SIZE;	  
  }	
}

int ICCCM::getMaxWidth() const{
  XSizeHints xsh = getSizeHints();
  if(xsh.flags & PMaxSize){
    return xsh.max_width;
  }else{
    return DEFAULT_MAX_SIZE;	  
  }	
}

int ICCCM::getMaxHeight() const{
  XSizeHints xsh = getSizeHints();
  if(xsh.flags & PMaxSize){
    return xsh.max_height;
  }else{
    return DEFAULT_MAX_SIZE;	  
  }	
}

bool ICCCM::overridePositionHints() const{
  XSizeHints xsh = getSizeHints();
  bool override = (xsh.flags & USPosition) //user specified position
  		|| (xsh.flags & PPosition) // program specified position
  		|| !(xsh.flags & PWinGravity); //not attached to something else
  return override;
}

int ICCCM::getInitialPosition() const{
  XSizeHints xsh = getSizeHints();
  if(xsh.flags & PWinGravity){ return xsh.win_gravity; }
  else{ return NorthWestGravity; }
}

int ICCCM::getInitialState() const{
  XWMHints* xwmh = XGetWMHints(QX11Info::display(), AID);
  int state;
  if(xwmh->flags & StateHint){ state = xwmh->initial_state; }
  else{ state = NormalState; }
  XFree(xwmh);
  return state;
}

	
XID ICCCM::getWindowGroup() const{
  XWMHints* xwmh = XGetWMHints(QX11Info::display(), AID);
  int grp;
  if(xwmh->flags & WindowGroupHint){ grp = xwmh->window_group; }
  else{ grp = 0; }
  XFree(xwmh);
  return grp;
}

QPixmap ICCCM::getIconPixmap() const{
  XWMHints* xwmh = XGetWMHints(QX11Info::display(), AID);
  QPixmap pix;
  if(xwmh->flags & IconPixmapHint){ 
    pix = QPixmap::fromX11Pixmap(xwmh->icon_pixmap); 
    if(xwmh->flags & IconMaskHint){
      pix.setMask(QBitmap(QPixmap::fromX11Pixmap(xwmh->iconmask)));	    
    }
  }
  XFree(xwmh);
  return pix;
}

	
// ==== Setters ====
void ICCCM::setFocus() const{
  XWMHints* xwmh = XGetWMHints(QX11Info::display(), AID);
  //Check if the App needs focus first
  bool needFocus = false;
  if(xwmh->flags & InputHint){ needFocus = xwmh->input; }
  //Now give the App focus if necessary
  if(needFocus){
    XSetInputFOcus(QX11Info::display(), AID, RevertToParent, CurrentTime);
  }
  //Set the protocol as necessary
  if( isSupportProtocol("WM_TAKE_FOCUS") ){ sendProtocol("WM_TAKE_FOCUS"); }
  XFree(xwmh);
}

void ICCCM::killApp() const{
  if( isSupportProtocol("WM_DELETE_WINDOW") ){ sendProtocol("WM_DELETE_WINDOW"); }
  else{ XKillClient(QX11Info::display(), AID); }
}

void ICCCM::changeWmState(int state) const{
  Atom satom = XInternAtom(QX11Info::display(), "WM_STATE",false);
  ulong data[2];
  data[0] = (ulong)state;
  data[1] = (ulong)None;
  XChangeProperty(QX11Info::display(), AID, satom, satom, 32, PropModeReplace, (uchar*)data, 2);
}

void ICCCM::setIconSizes(int size){
  XIconSize xis;
  xis.max_width = size;
  xis.max_height = size;
  xis.min_width = size;
  xis.min_height = size;
  xis.width_inc = 0;
  xis.height_inc = 0;
  XSetIconSizes(QX11Info::display(), AID, &xis, 1);
}
	
// ============== Private Utilities ===============
QString ICCCM::XTextPropertyToQString(XTextProperty tp) const{
  //Function to convert a XTextProperty into a QString for ease of use
  char** slist;
  int len;
  XTextPropertyToStringList(&tp, &slist, &len);
  QString output;
  if(len>0){
    output = slist[0]; //only take the first element in the list of strings
    XFreeStringList(slist); //now free the memory
  }
  return output;
}

XSizeHints ICCCM::getSizeHints() const{
  XSizeHints output;
  long len;
  XGetWMNormalHints(QX11Info::display(), AID, &output, &len); 
  return output;
}

bool ICCCM::isSupportProtocol(const char* atomname) const{
  bool issupport = false;
  //Get the Atom
  Atom atom = XInternAtom(QX11Info::display(), atomname, false); 
  //Now check if it is one of the support protocol atoms
  Atom *alist;
  int len;
  if( XGetWMProtocols(QS11Info::display(), AID, &alist, &len) ){
    for(int i=0; i<len; i++){
      if(alist[i] == atom){
        issupport = true;
        break;
      }
    }
    XFree(alist); //Free up the memory for the list
  }
  return issupport;
  
}

void ICCCM::sendProtocol(const char* protocol) const{
  XEvent ev;
  ev.xclient.type = ClientMessage;
  ev.xclient.window = AID;
  ev.xclient.message_type = XInternAtom(QX11Info::display(), "WM_PROTOCOLS",false);
  ev.xclient.format =  32;
  ev.xclient.data.l[0] = XInternAtom(QX11Info::display(), protocol, false);
  ev.xclient.data.l[1] = CurrentTime;
  XSendEvent(QX11Info::display(), AID, false, NULL, &ev);
}

