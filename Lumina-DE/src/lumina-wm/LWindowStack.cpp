#include "LWindowStack.h"

LWindowStack::LWindowStack(){
  //initialize the internal variables
  APPHASH = new QHash<Window, LWindow*>;
  FRAMEHASH = new QHash<Window, LWindow*>;
  STACKLIST = new QList<LWindow*>;
  MAPLIST = new QList<LWindow*>;
  activeWindow = 0;
}

LWindowStack::~LWindowStack(){
  delete APPHASH, FRAMEHASH, STACKLIST, MAPLIST;
}

//==================
//     PRIVATE
//==================
void LWindowStack::stackXWindows(){
  //Run the XLib command for restacking the windows
  
  int winNum = STACKLIST->size();
  Window winList[winNum];
  //List should be in order from Top->Bottom
  for(int i=0; i<winNum; i++){
    if(STACKLIST->at(i)->hasFrame()){ winList[i] = STACKLIST->at(i)->getFrameID(); }
    else{ winList[i] = STACKLIST->at(i)->getAppID(); } 	  
  }
  XRestackWindows(QX11Display::display(), winList, winNum);
}

//==================
//     PUBLIC
//==================
// == Application windows ==
bool LWindowStack::appExists(Window ID){
  return APPHASH.contains(ID);
}

void LWindowStack::addApp(Window ID, LWindow* win){
  APPHASH->insert(ID,win);
}

void LWindowStack::removeApp(Window ID){
  APPHASH->remove(ID);
}

LWindow* LWindowStack::getWindowByAppID(Window ID){
  return APPHASH->value(ID,0); //0 is the fallback value
}

// == Frame windows ==
bool LWindowStack::frameExists(Window ID){
  return FRAMEHASH.contains(ID);
}

void LWindowStack::addFrame(Window ID, LWindow* win){
  FRAMEHASH->insert(ID,win);
}

void LWindowStack::removeFrame(Window ID, LWindow* win){
  FRAMEHASH->remove(ID);
}

LWindow* LWindowStack::getWindowByFrameID(Window ID){
  return FRAMEHASH->value(ID,0); //0 is the fallback value
}

// == General Window Management ==
void LWindowStack::manageWindow(LWindow* win){
  MAPLIST->removeOne(win);
  MAPLIST->append(win);
  EWMH::setClientList(MAPLIST);
  EWMH::setClientStackList(STACKLIST);
  stackXWindows();
}

void LWindowStack::unmanageWindow(LWindow* win){
  MAPLIST->removeOne(win);
  STACKLIST->removeOne(win);
  EWMH::setClientList(MAPLIST);
  EWMH::setClientStackList(STACKLIST);
  stackXWindows();
}

void LWindowStack::restackWindow(LWindow* win){
  STACKLIST->removeOne(win); //remove it from the list (if available)
  //Put the window in the list
  if(win->isTopWindow()){
    STACKLIST->prepend(win); //put at the front of the list  
  }else if( win->isBottomWindow() ){
    STACKLIST->apppend(win);  //put at the end of the list
  }else{
    //Make sure it is lower than all the top level windows
    for(int i=0; i<StackList->size(); i++){
      if( !STACKLIST->at(i)->isTopWindow() ){
        STACKLIST->insert(i, win);
        break;
      }
    }
  }
  //Restack the windows
  stackXWindows();
  //Update the stacked list info
  EWMH::setClientStackList(STACKLIST);
}

void LWindowStack::setActiveWindow(LWindow* win){
  if(activeWindow == win){ return; }
  //de-activate the currently active window
  if(activeWindow != 0){ activeWindow->setActive(false); }
  //Now activate the new window
  activeWindow = win;	
  if(activeWindow != 0){ activeWindow->setActive(true); }
  //Now update the WM hints
  if(activeWindow != 0){ EWMH::setActiveWindow(win->getAppID()); }
  else{ EWMH::setActiveWindow(0); }
}

const LWindow* LWindowStack::getTopWindow(){
  const LWindow *win = 0;
  for(int i=0; i<STACKLIST->size(); i++){
    //Make sure it is not always on the top/bottom and is visible
    if(!STACKLIST->at(i)->isTopWindow() && !STACKLIST->at(i)->isBottomWindow()
    	    && (STACKLIST->at(i)->getState() != IconicState) ){
      win = STACKLIST->at(i);
      break;
    }
  }
  return win;
}

