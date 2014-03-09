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
	QList<QX11EmbedContainer*> trayIcons;
	
	
	
private slots:
	void addTrayIcon(WId win);
	void updateStatus();
	void trayAppClosed();
	
};

#endif
