//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_LDESKTOP_H
#define _LUMINA_DESKTOP_LDESKTOP_H

#include <QCoreApplication>
#include <QToolBar>
#include <QToolButton>
#include <QDesktopWidget>
#include <QMenu>
#include <QInputDialog>
#include <QProcess>
#include <QTimer>
#include <QMainWindow>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QPoint>

#include "Globals.h"
#include "LClock.h"
#include "LDeskBar.h"
#include "LUserButton.h"

#include "LSysTray.h" //must come last due to X library calls

class LDesktop : public QMainWindow{
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
	QToolBar *toolBar;
	
	//ToolBar widgets
	LUserButton *userTB;
	LSysTray *systray;
	LClock *clock;
	//QMenu *sysmenu;
	LDeskBar *deskbar;
	
	void SetupToolbar();
	
private slots:
	void SystemRun();
	void UpdateBackground();

protected:
	void moveEvent(QMoveEvent *event);
	
};
#endif
