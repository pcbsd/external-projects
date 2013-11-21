#include "lumina-wm/LApp.h"

LApp::LApp(const Window& ID){
  //Initialize the private variables
  AID = ID;
  icccm = new ICCCM(ID);
  ewmh = new EWMH(ID);
}

LApp::~LApp(){
  //Delete the complex private variables
  delete icccm;
  delete ewmh;
}

// ===== Main Interface Functions =====
// ==== Getters ====
int LApp::getX() const{
  XWindowAttributes xa;
  XGetWindowAttributes(QX11Info::display(), AID, &xa);
  return xa.x;	
}

int LApp::getY() const{
  XWindowAttributes xa;
  XGetWindowAttributes(QX11Info::display(), AID, &xa);
  return xa.y;	
}

unsigned int LApp::getWidth() const{
  XWindowAttributes xa;
  XGetWindowAttributes(QX11Info::display(), AID, &xa);
  return xa.width;	
}

unsigned int LApp::getHeight() const{
  XWindowAttributes xa;
  XGetWindowAttributes(QX11Info::display(), AID, &xa);
  return xa.height;	
}

unsigned int LApp::getBorderWidth() const{
  XWindowAttributes xa;
  XGetWindowAttributes(QX11Info::display(), AID, &xa);
  return xa.border_width;
}

unsigned int LApp::getMinWidth() const{
  return icccm->getMinWidth();
}

unsigned int LApp::getMinHeight() const{
  return icccm->getMinHeight();
}

unsigned int LApp::getMaxWidth() const{
  return icccm->getMaxWidth();	
}

unsigned int LApp::getMaxHeight() const{
  return icccm->getMaxHeight();
}

	
int LApp::getInitialPosition() const{
  return icccm->getInitialPosition();
}

int LApp::getInitialState() const{
  return icccm->getInitialState();
}

XID LApp::getWindowGroup() const{
  return icccm->getWindowGroup();
}

Atom LApp::getWindowType() const{
  return ewmh->getWindowType();
}

	
QString LApp::getTitle() const{
  QString title = ewmh->getVisibleTitle(); // EWMH visible title first
  if( title.isEmpty() ){ title = ewmh->getTitle(); }   // EWMH Title
  if( title.isEmpty() ){ title = icccm->getTitle(); }  // ICCCM Title
  return title;
}

QString LApp::getShortTitle() const{
  return icccm->getShortTitle();
}

QPixmap LApp::getIconPixmap() const{
  QPixmap pix = ewmh->getIconPixmap(); //Try EWMH first
  if(pix.isNull()){ pix = icccm->getIconPixmap(); } //Then try ICCCM
  return pix;
}

	
// ==== Setters ====
void LApp::setX(int x){
  XWindowChanges xc;
  xc.x = x;
  XConfigureWindow(QX11Info::display(), this->AID, CWX, &xc);
}

void LApp::setY(int y){
  XWindowChanges xc;
  xc.y = y;
  XConfigureWindow(QX11Info::display(), this->AID, CWY, &xc);
}

void LApp::setWidth(unsigned int width){
  XResizeWindow(QX11Info::display(), AID, width, getHeight());
}

void LApp::setHeight(unsigned int height){
  XResizeWindow(QX11Info::display(), AID, getWidth(), height);
}

void LApp::setBorderWidth(int width){
  XSetWindowBorderWidth(QX11Info::display(), AID, width);
}

void LApp::setVisible(bool show){
  if(show){ XMapWindow(QX11Info::display(), AID); }
  else{ XUnmapWindow(QX11Info::display(), AID); }
}

void LApp::changeWmState(int state){
  icccm->changeWmState(state);
}

	
void LApp::setFocus(){
  icccm->setFocus();
}

void LApp::killApp(){
  iccm->killApp();
}

	
// ==== Information ====
bool LApp::overridePositionHints() const{
  return icccm->overridePositionHints();	
}

