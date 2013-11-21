#ifndef _LUMINA_WM_WINDOW_H
#define _LUMINA_WM_WINDOW_H

#include "lumina-wm/LApp.h"
#include "lumina-wm/LFrame.h"

class LWindow : public QObject{
	Q_OBJECT

public:	
	LWindow(const Window& ID);
	~LWindow();
	
	//Main Access Functions
	// - Getters
	int getX();
	int getY();
	unsigned int getWidth();
	unsigned int getHeight();
	int getState();
	Window getAppID() const;
	Window getFrameID() const;
	XID getWindowGroup() const;
	
	// - Setters
	void setX(int);
	void setY(int);
	void setWidth(unsigned int);
	void setHeight(unsigned int);
	void setState(int);
        void rmFrame();
	void addFrame();
	void setActive(bool);
	
	// - Information
	Atom getWindowType();
	bool isTopWindow();
	bool isBottomWindow();
	bool isTrayApp();
	bool isTopLevelApp();
	bool ignoreWindow();
	bool hasFrame();
	
	
public slots:
	void resizeWindow(int width, int height);
	void moveWindow(int x, int y);
	void minimize();
	void maximize();
	void close();
	void forceClose();
	void giveFocus();	
	void refreshFrame(); //re-draw the frame (in case settings change)
	
private:
	Window AID; //X Window (app) ID
	Window FID; //X Window (frame) ID
	LFrame* frame; //Qt border for the X Window
	LApp* app; //Actual X Window
	int state; //internal state of the window (Normal, Iconic, Withdrawn)

	bool needsFrame();
        void initialWindowSetup();

	//Saved Location info for restoring a window
	int s_x, s_y, s_w, s_h;
	bool isMaximized;
	
};

#endif
