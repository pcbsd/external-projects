#include "SettingsMenu.h"

SettingsMenu::SettingsMenu() : QMenu(){
  this->setTitle( tr("Desktop Settings") );
  this->setIcon( LXDG::findIcon("configure","") );
  //Now setup the possible configuration options
  QAction *act = new QAction(LXDG::findIcon("preferences-desktop-screensaver",""), tr("Screensaver"), this);
	act->setWhatsThis("xscreensaver-demo");
	this->addAction(act);
	
}

SettingsMenu::~SettingsMenu(){
	
}