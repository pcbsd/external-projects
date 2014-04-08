//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_LDESKTOP_H
#define _LUMINA_DESKTOP_LDESKTOP_H

#include <QCoreApplication>


#include <QSettings>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QTimer>


#include "LPanel.h"
#include "Globals.h"

class LDesktop : public QObject{
	Q_OBJECT
public:
	LDesktop(int deskNum=0);
	~LDesktop();
	
public slots:
	void SystemLogout(){ QCoreApplication::exit(0); }
	void SystemRestart(){ SYSTEM::restart(); QCoreApplication::exit(0); }
	void SystemShutdown(){ SYSTEM::shutdown(); QCoreApplication::exit(0); }
	
private:
	QSettings *settings;
	QTimer *bgtimer;
	QDesktopWidget *desktop;
	QString DPREFIX;
	int desktopnumber;
	int xoffset;
	bool defaultdesktop;
	QList<LPanel*> PANELS;
	QWidget *bgWindow;
	
private slots:
	void UpdatePanels();

	void UpdateBackground();

};
#endif
