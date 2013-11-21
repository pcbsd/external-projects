#include "lumina-wm/LWindow.h"

LWindow::LWindow(const Window& ID) : QObject(){
  AID = ID;
  app = new LApp(ID);
  frame = NULL;
  FID = 0;
  state = WithdrawnState;
	
  app->setBorderWidth(0); //no X border, frame will handle it
}

LWindow::~LWindow(){
  delete app;
  if( hasFrame() ){ delete frame; }
}

//===================================
//====== Main Access Functions ======
//===================================
// ==== Getters ====
int LWindow::getX(){
  if( hasFrame() ){ return frame->getX(); }
  else{ return app->getX(); }
}

int LWindow::getY(){
  if( hasFrame() ){ return frame->getY(); }
  else{ return app->getY(); }	
}

unsigned int LWindow::getWidth(){
  if( hasFrame() ){ return frame->getWidth(); }
  else{ return app->getWidth(); }
}

unsigned int LWindow::getHeight(){
  if( hasFrame() ){ return frame->getHeight(); }
  else{ return app->getHeight(); }
}

	
// - Setters
void LWindow::setX(int x){
  if( hasFrame() ){ frame->setX(x); }
  else{ app->setX(x); }
}

void LWindow::setY(int y){
  if( hasFrame() ){ frame->setY(y); }
  else{ app->setY(y); }
}

void LWindow::setWidth(unsigned int width){
  if( hasFrame() ){ 
	  int borders = LWMSET::frameWidth*2;
	  if((LWMSET::controlEdge == LWMSET::LEFT) || (LWMSET::controlEdge == LWMSET::RIGHT) ){
	    borders += LWMSET::iconSize;
	  }
	  app->setWidth( width - borders);
	  frame->setWidth( app->getWidth() + borders); //make sure the app didn't do something funny in resizing
  }else{ app->setWidth(width); }	
}

void LWindow::setHeight(unsigned int height){
  if( hasFrame() ){ 
	  int borders = LWMSET::frameWidth*2;
	  if((LWMSET::controlEdge == LWMSET::TOP) || (LWMSET::controlEdge == LWMSET::BOTTOM) ){
	    borders += LWMSET::iconSize;
	  }
	  app->setHeight( height - borders);
	  frame->setHeight( app->getHeight() + borders); //make sure the app didn't do something funny in resizing
  }else{ app->setHeight(height); }	
}
	
void LWindow::setState(int newstate){
  if( state == WithdrawnState && (newstate == NormalState || newstate == IconicState) ){
    //Withdrawn -> Normal or Iconic (visible/invisible, app starting up)
    initialWindowSetup();
    if( newstate == NormalState ){
	if( hasFrame() ){ frame->setVisible(true); }
	app->setVisible(true);
    }else{ //iconic
	if( hasFrame() ){ frame->setVisible(false); }
	app->setVisible(false);
    }
	    
    
  }else if( (state == NormalState || state == IconicState) && newstate == WithdrawnState ){
    //Normal or Iconic -> Withdrawn (no frame, app closing down)
    if( hasFrame() ){ frame->setVisible(false); }

  }else if( state == NormalState && newstate == IconicState ){
    //Normal ->Iconic (invisible, but still active)
    if( hasFrame() ){ frame->setVisible(false); }
    app->setVisible(false);	  
  }else if( state == IconicState && newstate == NormalState ){
    //Iconic -> Normal (visible and active)
    if( hasFrame() ){ frame->setVisible(true); }
    app->setVisible(true);
  }else{ return; } //do nothing otherwise (same or invalid state)
  
  //Now change the app state and save it
  app->changeWmState(newstate);
  state = newstate;
  
}

void LWindow::rmFrame(){
  if( hasFrame() ){
    delete frame;
    frame = NULL; //make sure that it is reset to the proper "nothing" value
  }
}

void LWindow::addFrame(){
  //Check that it doesn't have one and still needs it
  if( !hasFrame() && needsFrame() ){
	  //Create the frame and set the sizes
	  frame = new LFrame();
	  setX(app->getX());
	  setY(app->getY());
	  if( (LWMSET::controlEdge == LWMSET::LEFT) || (LWMSET::controlEdge == LWMSET::RIGHT) ){
	    setWidth( app->getWidth() + (LWMSET::frameWidth*2) + LWMSET::iconSize );
	  }else{
	    setWidth( app->getWidth() + (LWMSET::frameWidth *2) );
	  }
	  if( (LWMSET::controlEdge == LWMSET::TOP) || (LWMSET::controlEdge == LWMSET::BOTTOM) ){
	    setHeight( app->getHeight() + (LWMSET::frameWidth*2) + LWMSET::iconSize );
	  }else{
	    setHeight( app->getHeight() + (LWMSET::frameWidth *2) );
	  }
	  //Now save the new frame ID
	  FID = frame->winID();
	  //Now attach the application to the frame as a child
	  XReparentWindow(QX11Info::display(), AID, FID, 
	  //Connect the signals from the frame's buttons to the applications slots
	  connect(frame, SIGNAL(resized(int,int)), this, SLOT(resizeWindow(int,int)) );
	  connect(frame, SIGNAL(minimizeClicked()), this, SLOT(minimize()) );
	  connect(frame, SIGNAL(maximizeClicked()), this, SLOT(maximize()) );
	  connect(frame, SIGNAL(closeClicked()), this, SLOT(close()) );
	  //Add the window to the X Save Set
	  xAddToSaveSet(QX11Info::display(), AID);
  }
}

void LWindow::setActive(bool active){
  if(hasFrame()){
    frame->setActive(active);	  
  }
}
// ==== Information ====
Atom LWindow::getWindowType(){
  return app->getWindowType();
}

bool LWindow::isTopWindow(){
  Atom WA = app->getWindowType();
  bool top=false;
  //Check for specific window types that always need to stay on top
  if( WA == LWMUtil::getAtom("_NET_WM_WINDOW_TYPE_DOCK") ){ top=true; }
  else if( WA == LWMUtil::getAtom("_NET_WM_WINDOW_TYPE_SPLASH")){ top=true; }
  return top;
}

bool LWindow::isBottomWindow(){
  Atom WA = app->getWindowType();
  bool top=false;
  //Check for specific window types that always need to stay on bottom
  if( WA == LWMUtil::getAtom("_NET_WM_WINDOW_TYPE_DESKTOP") ){ top=true; }
  return top;	
}

bool LWindow::isTrayApp(){
  return false; //not implemented yet
}

bool LWindow::isTopLevelApp(){
  return false; //not implemented yet
}

bool LWindow::ignoreWindow(){
  //Check if the window should be ignored by the WM
  XWindowAttributes xwa;
  XGetWindowAttributed(QX11Info::display(), AID, &xwa);
  return xwa.override_redirect;
}

bool LWindow::hasFrame(){
  return (frame != NULL);
}

int LWindow::getState(){
  return state;	
}

Window LWindow::getAppID() const{
  return AID;
}

Window LWindow::getFrameID() const{
  return FID;
}

XID LWindow::getWindowGroup() const{
  return app->getWindowGroup();
}

// ===== PUBLIC SLOTS =====
void LWindow::resizeWindow(int width, int height){
  //NOTE: Inputs are CHANGE to width and height (+/-)
  int nval,min,max;
  //update width
  if(width != 0){
    nval = width + frame->getWidth();
    min = app->getMinWidth();
    max = app->getMaxWidth();
    if(nval >= min && nval <= max){ setWidth(nval); }
    else if(nval >= min){ setWidth(max); }
    else if(nval <= max){ setWIdth(min); }
  }
  //update height
  if(height != 0){
    nval = height + frame->getHeight();
    min = app->getMinHeight();
    max = app->getMaxHeight();
    if(nval >= min && nval <= max){ setHeight(nval); }
    else if(nval >= min){ setHeight(max); }
    else if(nval <= max){ setHeight(min); }
  }
  isMaximized=false;
}

void LWindow::moveWindow(int x, int y){
  maxx = QApplication::desktop()->width();
  maxy = QApplication::desktop()->height();
  if( x<maxx ){ setX(x); }
  if( y<maxy ){ setY(y); }
}

void LWindow::minimize(){
  setState(IconicState);
}

void LWindow::maximize(){
  if(isMaximized){
    //Already maximized - restore to previous size
    setX(s_x); setY(s_y);
    setWidth(s_w); setHeight(s_h);
    isMaximized=false;
  }else{
    //Save the current location info
    if( hasFrame() ){
      s_x=frame->getX(); s_y=frame->getY();
      s_w=frame->getWidth(); s_h=frame->getHeight();
    }else{
      s_x=app->getX(); s_y=app->getY();
      s_w=app->getWidth(); s_h=app->getHeight();	    
    }
    //Now start the maximization process
    amaxW = app->getMaxWidth();
    amaxH = app->getMaxHeight();
    dmaxW = QApplication::desktop()->width();
    dmaxH = QApplication::desktop()->height();
    //maximize width if possible
    if(amaxW >= dmaxW){
      setX(0);
      setWidth(dmaxW);
    }else{
      if( app->getX() > (dmaxW-amaxW) ){
        //move window to the left a bit to make it fit
        setX(dmaxW-amaxW);
      }
      setWidth(amaxW); //set it to the maximum allowed width
    }
    //maximize height if possible
    if(amaxH >= dmaxH){
      setY(0);
      setHeight(dmaxH);
    }else{
      if( app->getY() > (dmaxH-amaxH) ){
        //move window up a bit to make it fit
        setY(dmaxH-amaxH);
      }
      setHeight(amaxH); //set it to the maximum allowed height
    }
    isMaximized=true; //set the flag that it is currently maximized
  }
}

void LWindow::close(){
  app->killApp();
}

void LWindow::forceClose(){
  //not implemented yet
}

void LWindow::giveFocus(){
  app->setFocus();
}

void LWindow::refreshFrame(){
  rmFrame();
  addFrame();
}
	
// ===== PRIVATE FUNCTIONS =====
bool LWindow::needsFrame(){
  //Get the type of window we have
  Atom WA = app->getWindowType();
  //Check for special cases where we don't need a frame
  bool need = true;
  if( WA == LWMUtil::getAtom("_NET_WM_WINDOW_TYPE_SPLASH")){ need = false; } //splash screens
  else if(WA == LWMUtil::getAtom("_NET_WM_WINDOW_TYPE_DESKTOP")){ need = false; } //desktop
  else if(WA == LWMUtil::getAtom("_NET_WM_WINDOW_TYPE_DOCK")){ need = false; } //docks
  //Now return the result
  return need;
}

void LWindow::initialWindowSetup(){
  if( hasFrame() ){
    frame->setTitle(app->getTitle());
    frame->setIconPixmap(app->getIconPixmap());
  }
  
  if( !app->overridePositionHints() ){
    switch (app->getInitialPosition()){
    case NorthWestGravity:
	setX(0); 
        setY(0); 
        break;
    case NorthGravity:
	setX( (QApplication::desktop()->width()/2) - (getWidth()/2) );
        setY(0);
        break;
    case NorthEastGravity:
	setX( QApplication::desktop()->width() - getWidth() );
        setY(0);
        break;
    case WestGravity:
	setX(0); 
        setY( (QApplication::desktop()->height()/2) - (getHeight()/2) ); 
        break;
    case CenterGravity:
	setX( (QApplication::desktop()->width()/2) - (getWidth()/2) );
        setY( (QApplication::desktop()->height()/2) - (getHeight()/2) ); 
        break;
    case EastGravity:
	setX( QApplication::desktop()->width() - getWidth() );
        setY( (QApplication::desktop()->height()/2) - (getHeight()/2) ); 
        break;
    case SouthWestGravity:
	setX(0); 
        setY( QApplication::desktop()->height() - getHeight() );  
        break;
    case SouthGravity:
	setX( (QApplication::desktop()->width()/2) - (getWidth()/2) );
        setY( QApplication::desktop()->height() - getHeight() );  
        break;
    case SouthEastGravity:
	setX( QApplication::desktop()->width() - getWidth() );
        setY( QApplication::desktop()->height() - getHeight() );  
        break;
    }
  }
  
}
