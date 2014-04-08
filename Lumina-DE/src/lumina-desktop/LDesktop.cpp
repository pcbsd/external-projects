//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDesktop.h"

LDesktop::LDesktop(int deskNum) : QObject(){
	
  DPREFIX = "desktop-"+QString::number(deskNum)+"/";
  desktopnumber = deskNum;
  desktop = new QDesktopWidget();
  defaultdesktop = (deskNum== desktop->primaryScreen());
  xoffset = 0;
  for(int i=0; i<desktopnumber; i++){
    xoffset += desktop->screenGeometry(i).width();
  }
  //Setup the internal variables
  settings = new QSettings(QSettings::UserScope, "LuminaDE","desktopsettings", this);
  bgtimer = new QTimer(this);
    bgtimer->setSingleShot(true);
 
  bgWindow = new QWidget(0);
	bgWindow->setObjectName("bgWindow");
	LX11::SetAsDesktop(bgWindow->winId());
	bgWindow->setGeometry(xoffset,0,desktop->screenGeometry().width(), desktop->screenGeometry().height());
  //Start the update processes
  QTimer::singleShot(1,this, SLOT(UpdateBackground()) );
  QTimer::singleShot(1,this, SLOT(UpdatePanels()) );

}

LDesktop::~LDesktop(){
}

// =====================
//     PRIVATE SLOTS 
// =====================
void LDesktop::UpdatePanels(){
  qDebug() << " - Update Panels";
  int panels = settings->value(DPREFIX+"panels", 0).toInt();
  if(panels==0 && defaultdesktop){ panels=1; } //need at least 1 panel on the primary desktop
  for(int i=0; i<panels; i++){
    if(i<PANELS.length()){
      qDebug() << " -- Update panel "<< i;
      //panel already exists - just update it
      QTimer::singleShot(1, PANELS[i], SLOT(UpdatePanel()) );
    }else{
      qDebug() << " -- Create panel "<< i;
      //New panel
      PANELS << new LPanel(settings, desktopnumber, i);
    }
  }
  
}

void LDesktop::UpdateBackground(){
  //Get the current Background
  QString cbg = settings->value(DPREFIX+"background/current", "").toString();
  //Get the list of background(s) to show
  QStringList bgL = settings->value(DPREFIX+"background/filelist", "").toStringList();
    //Remove any invalid files
    for(int i=0; i<bgL.length(); i++){ 
      if( !QFile::exists(bgL[i]) || bgL[i].isEmpty()){ bgL.removeAt(i); i--; } 
    }
  //Determine which background to use next
  int index = bgL.indexOf(cbg);
  if( (index < 0) || (index >= bgL.length()-1) ){ index = 0; } //use the first file
  else{ index++; } //use the next file in the list
  QString bgFile;
  if( bgL.isEmpty() && cbg.isEmpty()){ bgFile = "default"; }
  else if( bgL.isEmpty() && QFile::exists(cbg) ){ bgFile = cbg; }
  else if( bgL.isEmpty() ){ bgFile = "default"; }
  else{ bgFile = bgL[index]; }
  //Save this file as the current background
  settings->setValue(DPREFIX+"background/current", bgFile);
  if( (bgFile.toLower()=="default")){ bgFile = "/usr/local/share/Lumina-DE/desktop-background.jpg"; }
  //Now set this file as the current background
  //QString display = QString( getenv("DISPLAY") );
  //display = display.section(".",0,0)+"."+desktopnumber; //only the current screen
  QString style = "QWidget#bgWindow{ border-image:url(%1) stretch;}";
  style = style.arg(bgFile);
  bgWindow->setStyleSheet(style);
  bgWindow->show();
  //QString cmd = "xv +24 -maxp -rmode 5 -quit \""+bgFile+"\"";
  //QProcess::startDetached(cmd);
  //Now reset the timer for the next change (if appropriate)
  if(bgL.length() > 1){
    //get the length of the timer (in minutes)
    int min = settings->value(DPREFIX+"background/minutesToChange",5).toInt();
    //reset the internal timer
    if(bgtimer->isActive()){ bgtimer->stop(); }
    bgtimer->start(min*60000); //convert from minutes to milliseconds
  }
}
