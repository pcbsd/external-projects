//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDesktop.h"

LDesktop::LDesktop(int deskNum) : QObject(){
  //this->setCentralWidget(new QWidget(this));
  //panel = new QWidget();
  //panel->setContentsMargins(0,0,0,0);
  //panel->setWindowFlags( Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  //panel->setFocusPolicy(Qt::NoFocus);
  //this->setContextMenuPolicy(Qt::NoContextMenu);
  //panel->setWindowTitle("");
  DPREFIX = "desktop-"+QString::number(deskNum)+"/";
  desktopnumber = deskNum;
  desktop = new QDesktopWidget();
  defaultdesktop = (deskNum== desktop->primaryScreen());
  //Setup the internal variables
  settings = new QSettings(QSettings::UserScope, "LuminaDE","desktopsettings", this);
  bgtimer = new QTimer(this);
    bgtimer->setSingleShot(true);
  //Create the Toolbar (QToolBar does not work unless in a QMainWindow)
  //toolBar = new QToolBar(this);
    //toolBar->setMovable(false);
  //Now populate the toolbar
  //SetupToolbar(); //always 22 pixels height (systray restriction)
  //this->addToolBar(Qt::TopToolBarArea, toolBar);
  //Now setup the widget location/size
  //this->centralWidget()->setVisible(false);
  //panel->setGeometry(0,0,desktop->availableGeometry(deskNum).width(), 22 ); //toolBar->height() );
  //Start the update processes
  QTimer::singleShot(1,this, SLOT(UpdateBackground()) );
  QTimer::singleShot(1,this, SLOT(UpdatePanels()) );
  //Make sure the toolbar is in the upper-left corner
  //panel->move(0,0);
}

LDesktop::~LDesktop(){
}

// =====================
//   PRIVATE FUNCTIONS 
// =====================
/*void LDesktop::SetupToolbar(){
  panel->setContentsMargins(0,0,0,0);
  //Create the User Button
  userTB = new LUserButtonPlugin(this);
  toolBar->addWidget(userTB);

  deskbar = new LDeskBar(toolBar); //gets added right now
    deskbar->start();
  clock = new LClock(this);
    clock->start();
  systray = new LSysTray(this);
    systray->start();
  
  QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
  //Now add them all to the toolbar in the proper order
  toolBar->addWidget(spacer);
  toolBar->addWidget(systray);
  toolBar->addWidget(clock);

}*/

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
  QString display = QString( getenv("DISPLAY") );
  display = display.section(".",0,0)+desktopnumber; //only the current screen
  QString cmd = "xv +24 -maxp -display "+display+" -rmode 5 -quit \""+bgFile+"\"";
  QProcess::startDetached(cmd);
  //Now reset the timer for the next change (if appropriate)
  if(bgL.length() > 1){
    //get the length of the timer (in minutes)
    int min = settings->value(DPREFIX+"background/minutesToChange",5).toInt();
    //reset the internal timer
    if(bgtimer->isActive()){ bgtimer->stop(); }
    bgtimer->start(min*60000); //convert from minutes to milliseconds
  }
}
