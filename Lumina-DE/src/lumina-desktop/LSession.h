//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_SESSION_H
#define _LUMINA_DESKTOP_SESSION_H

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QX11Info>

#include "Globals.h"

//LibLumina X11 class
#include <LuminaX11.h>

//X includes (these need to be last due to Qt compile issues)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrender.h>

//SYSTEM TRAY STANDARD DEFINITIONS
#define _NET_SYSTEM_TRAY_ORIENTATION_HORZ 0
#define _NET_SYSTEM_TRAY_ORIENTATION_VERT 1
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2


class LSession : public QApplication{
	Q_OBJECT
public:
	LSession(int &argc, char **argv);
	~LSession();
	
	virtual bool x11EventFilter(XEvent *event);
	
	//System Tray Utilities
	static bool StartupSystemTray();
	static bool CloseSystemTray();
	
private slots:
	//system tray functions
	void parseClientMessageEvent(XClientMessageEvent *event);

signals:
	void NewSystemTrayApp(WId); //WinID
	void WindowListEvent();
	
};

#endif
