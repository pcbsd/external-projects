#ifndef _LUMINA_DESKTOP_USER_MENU_H
#define _LUMINA_DESKTOP_USER_MENU_H

#include <QMenu>
#include <QAction>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>

#include "LTBWidget.h"
#include "AppMenu.h"
#include "Globals.h"

#include "LuminaXDG.h"

class LUserButton : public LTBWidget{
	Q_OBJECT
public:
	LUserButton(QWidget *parent = 0);
	~LUserButton();

private:
	AppMenu *appMenu; //System Applications submenu
	QDir *homedir;
	QMenu *closeMenu; //Lumina Close Submenu
	QMenu *mainMenu; 
	QSettings *settings;

	void UpdateMenu();

	void goToDir(QString);

private slots:
	void buttonClicked(){
	  UpdateMenu();
	  mainMenu->popup(this->mapToGlobal(QPoint(0,this->height())) );
	}
	
	void menuButtonClicked(QAction *act){
	  if(act->parent() != mainMenu){ return; }
	  if(!act->whatsThis().isEmpty()){
	    goToDir( act->whatsThis() );
	  }
	}
	
	void Logout();
	void Restart();
	void Shutdown();
};
#endif