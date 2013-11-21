#ifndef _LUMINA_WM_LWINDOWSTACK_H
#define _LUMINA_WM_LWINDOWSTACK_H

#include <QHash>

#include "LWindow.h"

class LWindowStack{

public:
	LWindowStack();
	~LWindowStack();
	
	//Application windows
	bool appExists(Window ID);
	void addApp(Window ID, LWindow* win);
	void removeApp(Window ID);
	LWindow* getWindowByAppID(Window ID);
	//Frame windows
	bool frameExists(Window ID);
	void addFrame(Window ID, LWindow* win);
	void removeFrame(Window ID, LWindow* win);
	LWindow* getWindowByFrameID(Window ID);
	//General Window Management
	void manageWindow(LWindow* win);
	void unmanageWindow(LWindow* win);
	void restackWindow(LWindow* win);
	void setActiveWindow(LWindow* win);
	const LWindow* getTopWindow();
	
private:
	QHash<Window, const LWindow*> *APPHASH, *FRAMEHASH; 
	QList<const LWindow*> *STACKLIST, *MAPLIST;
	const LWindow *activeWindow;
	
	void stackXWindows();
	
};

#endif

