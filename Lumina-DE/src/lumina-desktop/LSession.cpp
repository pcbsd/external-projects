//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LSession.h"

LSession::LSession(int &argc, char ** argv) : QApplication(argc, argv){
  this->setApplicationName("Lumina Desktop Environment");
  this->setApplicationVersion("0.0.1");
  this->setOrganizationName("LuminaDesktopEnvironment");
  this->setQuitOnLastWindowClosed(false); //since the LDesktop's are not necessarily "window"s
}

LSession::~LSession(){

}

bool LSession::x11EventFilter(XEvent *event){
  //Make sure the system tray can process the event
  emit SystemTrayEvent(event);
  //Make sure the window list can process the event
  emit WindowListEvent(event);
  //Now continue on with the event handling
  return false;
}

