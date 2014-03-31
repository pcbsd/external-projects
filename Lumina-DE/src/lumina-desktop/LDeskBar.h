//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_DESKBAR_H
#define _LUMINA_DESKTOP_DESKBAR_H

#include <QWidget>
//#include <QDir>
//#include <QFile>
//#include <QTextStream>
#include <QString>
//#include <QStringList>
//#include <QToolButton>
#include <QAction>
#include <QMenu>
#include <QProcess>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QHBoxLayout>
#include <QIcon>
#include <QToolBar>
#include <QDebug>

#include <LuminaXDG.h>
#include "LTBWidget.h"

class LDeskBar : public QObject{
	Q_OBJECT
public:
	LDeskBar(QToolBar* parent=0);
	~LDeskBar();
	
	//void setHeight(int pix);
	void start();
	
private:
	//QHBoxLayout *layout;
	QString desktopPath;
	QFileSystemWatcher *watcher;
	QToolBar *TB;
	//Special toolbuttons and menus
	LTBWidget *appB, *fileB, *dirB;
	QAction *appA, *fileA, *dirA; //actions for the buttons in the toolbar
	QMenu *appM, *dirM, *audioM, *videoM, *pictureM, *fileM, *otherM;
	QStringList audioFilter, videoFilter, pictureFilter;
	QFileInfoList totals;
	//QList<QToolButton*> exeList;
	
	void initializeDesktop();
	//bool readDesktopFile(QString path, QString &name, QString &iconpath);
	
	QAction* newAction(QString filepath, QString name, QString iconpath);
	QAction* newAction(QString filepath, QString name, QIcon icon);
	//QToolButton* newLauncher(QString filepath, QString name, QString iconpath);
	void updateMenu(QMenu* menu, QFileInfoList files, bool trim = true);
	//bool searchForExe(QString filename);
	
private slots:
	void ActionTriggered(QAction* act);
	void desktopChanged();
	
};
#endif

