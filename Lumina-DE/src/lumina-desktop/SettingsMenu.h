#ifndef _LUMINA_DESKTOP_SETTINGS_MENU_H
#define _LUMINA_DESKTOP_SETTINGS_MENU_H

#include <QMenu>
#include <QProcess>
#include <QAction>

#include <LuminaXDG.h>

class SettingsMenu : public QMenu{
	Q_OBJECT
public:
	SettingsMenu();
	~SettingsMenu();
		
private slots:
	void runApp(QAction* act){
	  QProcess::startDetached(act->whatsThis());
	}
};

#endif