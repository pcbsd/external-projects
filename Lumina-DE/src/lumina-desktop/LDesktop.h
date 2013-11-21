//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_LDESKTOP_H
#define _LUMINA_DESKTOP_LDESKTOP_H

#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDesktopWidget>
#include <QMenu>
#include <QInputDialog>
#include <QStatusBar>
#include <QTimer>

#include "Globals.h"
#include "LClock.h"
#include "LDeskBar.h"
#include "LMixer.h"
#include "LSysTray.h" //must come last due to X library calls

class LDesktop : public QWidget{
	Q_OBJECT
public:
	LDesktop(int deskNum=0);
	~LDesktop();
	
public slots:
	void SystemLogout(){ emit Finished(); }
	void SystemRestart(){ SYSTEM::restart(); emit Finished(); }
	void SystemShutdown(){ SYSTEM::shutdown(); emit Finished(); }
	
private:
	QHBoxLayout *toolbar;
	QToolButton *userTB;
	LSysTray *systray;
	LClock *clock;
	QMenu *sysmenu;
	QDesktopWidget *desktop;
	QVBoxLayout *layout;
	LDeskBar *deskbar;
	LMixerWidget *mixer;
	
	void SetupToolbar();
	
private slots:
	void SystemRun();
	void newXEvent(XEvent*);
	
signals:
	void Finished();
};
#endif
