//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_SYSTRAY_H
#define _LUMINA_DESKTOP_SYSTRAY_H

//Qt includes
#include <QFrame>
#include <QHBoxLayout>
#include <QDebug>
#include <QX11Info>
#include <QX11EmbedContainer>

//Lumina includes
#include "LSession.h"

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


class LSysTray : public QWidget{
	Q_OBJECT
public:
	LSysTray(QWidget *parent = 0);
	~LSysTray();

	void start();
	void stop();
	
private:
	QHBoxLayout *layout;
	bool isRunning;
	Window trayID;
	QList<QX11EmbedContainer*> trayIcons;
	
	void addTrayIcon(Window win);
	
private slots:
	void parseX11Event(XEvent *event);
	void parseClientMessageEvent(XClientMessageEvent *event);
	void updateStatus();
	void trayAppClosed();
	
};

#endif
