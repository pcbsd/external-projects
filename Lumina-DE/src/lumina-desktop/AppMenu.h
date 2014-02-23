//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_APP_MENU_H
#define _LUMINA_DESKTOP_APP_MENU_H

#include <QMenu>
#include <QFileSystemWatcher>
#include <QString>
#include <QList>
#include <QTimer>
#include <QHash>
#include <QAction>
#include <QProcess>

#include <LuminaXDG.h>

class AppMenu : public QMenu{
	Q_OBJECT
public:
	AppMenu(QWidget *parent = 0);
	~AppMenu();
	
private:
	QFileSystemWatcher *watcher;
	QString appstorelink; //default *.desktop file to use as the "store" 
	QList<QMenu> MLIST;
	QHash<QString, QList<XDGDesktop> > APPS;
	
	void updateAppList(); //completely update the menu lists
	
private slots:
	void start(); //This is called in a new thread after initialization
	void watcherUpdate();
	void launchStore();
	void launchApp(QAction *act);
};

#endif

