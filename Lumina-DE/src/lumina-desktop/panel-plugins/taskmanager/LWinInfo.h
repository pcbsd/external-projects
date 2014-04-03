#ifndef _LUMINA_DESKTOP_WINDOW_INFO_H
#define _LUMINA_DESKTOP_WINDOW_INFO_H

#include <QString>

#include "LSession.h"
#include "Globals.h" //For the STATES enumeration definition

class LWinInfo{
private:
	WId window;

public:
	LWinInfo(WId id){
	  window = id;
	}
	~LWinInfo(){};
	
	//The current window ID
	WId windowID(){
	  return window;
	}
	
	//Information Retrieval
	 // Don't cache these results because they can change regularly
	QString  name(){
	  return LSession::WindowName(window);
	}
	
	QString visibleName(){
	  return LSession::WindowVisibleName(window);
	}

	QString icon(){
	  return LSession::WindowIconName(window);
	}
	
	QString visibleIcon(){
	  return LSession::WindowVisibleIconName(window);
	}
	
	STATES status(){
	  //Check for if the window requires attention
	  if(LSession::WindowRequiresAttention(window)){
	    return STATES::Notification;
	  }else if(
	}
};

#endif