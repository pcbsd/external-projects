//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_DESKBAR_H
#define _LUMINA_DESKTOP_DESKBAR_H

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QToolButton>
#include <QMenu>
#include <QProcess>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QHBoxLayout>
#include <QIcon>
#include <QDebug>

class LDeskBar : public QWidget{
	Q_OBJECT
public:
	LDeskBar(QWidget* parent=0);
	~LDeskBar();
	
	void setHeight(int pix);
	void start();
	
private:
	QHBoxLayout *layout;
	QString desktopPath;
	QFileSystemWatcher *watcher;
	//Special toolbuttons and menus
	QToolButton *homeB, *dirB, *audioB, *videoB, *pictureB, *fileB;
	QMenu *homeM, *dirM, *audioM, *videoM, *pictureM, *fileM;
	QStringList audioFilter, videoFilter, pictureFilter;
	QFileInfoList totals;
	QList<QToolButton*> exeList;
	
	void initializeDesktop();
	bool readDesktopFile(QString path, QString &name, QString &iconpath);
	
	QAction* newAction(QString filepath, QString name, QString iconpath);
	QToolButton* newLauncher(QString filepath, QString name, QString iconpath);
	void updateMenu(QMenu* menu, QFileInfoList files, bool trim = true);
	bool searchForExe(QString filename);
	
private slots:
	void ActionTriggered(QAction* act);
	void desktopChanged();
	
};
#endif

