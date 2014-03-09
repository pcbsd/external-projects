//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDesktop.h"

LDesktop::LDesktop(int deskNum) : QMainWindow(){
  this->setCentralWidget(new QWidget(this));
  this->setWindowFlags( Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  this->setFocusPolicy(Qt::NoFocus);
  this->setContextMenuPolicy(Qt::NoContextMenu);
  this->setWindowTitle("LuminaDesktopPanel");
  desktop = new QDesktopWidget();
  //Setup the internal variables
  settings = new QSettings(QSettings::UserScope, "LuminaDE","desktop"+QString::number(deskNum), this);
  bgtimer = new QTimer(this);
    bgtimer->setSingleShot(true);
  //Create the Toolbar (QToolBar does not work unless in a QMainWindow)
  toolBar = new QToolBar(this);
    toolBar->setMovable(false);
  //Now populate the toolbar
  SetupToolbar(); //always 22 pixels height (systray restriction)
  this->addToolBar(Qt::TopToolBarArea, toolBar);
  //Now setup the widget location/size
  this->centralWidget()->setVisible(false);
  this->setGeometry(0,0,desktop->availableGeometry(deskNum).width(), 22 ); //toolBar->height() );
  //Start the automated widgets
  UpdateBackground();
  //Make sure the toolbar is in the upper-left corner
  this->move(0,0);
}

LDesktop::~LDesktop(){
}

// =====================
//   PRIVATE FUNCTIONS 
// =====================
void LDesktop::SetupToolbar(){
  //Create the User Button
  userTB = new LUserButton(this);
  //appmenu = new AppMenu(this);
  /*
  sysmenu = new QMenu(this);
    sysmenu->addMenu(appmenu);
    sysmenu->addAction( QIcon(":/images/default-application.png"), tr("Run..."),this,SLOT(SystemRun()) );
    sysmenu->addAction( QIcon(":/images/default-logout.png"),tr("Logout"),this,SLOT(SystemLogout()) );
    sysmenu->addAction( QIcon(":/images/default-restart.png"),tr("Restart"),this,SLOT(SystemRestart()) );
    sysmenu->addAction( QIcon(":/images/default-shutdown.png"),tr("Shutdown"),this,SLOT(SystemShutdown()) );
  userTB = new QToolButton(this);
    userTB->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    userTB->setText(SYSTEM::user());
    userTB->setIcon(QIcon(":/images/default-user.png"));
    userTB->setMenu(sysmenu);
    userTB->setPopupMode(QToolButton::InstantPopup);
  */
    
  deskbar = new LDeskBar(this);
    //deskbar->setHeight(22);
    deskbar->start();
  clock = new LClock(this);
    clock->start();
  systray = new LSysTray(this);
    systray->start();
  
  QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
  //Now add them all to the toolbar in the proper order
  toolBar->addWidget(userTB);
  toolBar->addWidget(deskbar);
  toolBar->addWidget(spacer);
  toolBar->addWidget(systray);
  toolBar->addWidget(clock);

}

// =====================
//     PRIVATE SLOTS 
// =====================

void LDesktop::SystemRun(){
  qDebug() << "New Run Dialog";
  QString cmd = QInputDialog::getText(0,"Run Command","Command:");
  qDebug() << " - Run Dialog Finished";
  if(cmd.isEmpty()){ return; }
  qDebug() << "Running command:" << cmd;
  QProcess::startDetached(cmd);
}

void LDesktop::UpdateBackground(){
  //Get the current Background
  QString cbg = settings->value("background/current", "").toString();
  //Get the list of background(s) to show
  QStringList bgL = settings->value("background/filelist", "").toStringList();
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
  settings->setValue("background/current", bgFile);
  if( (bgFile.toLower()=="default")){ bgFile = "/usr/local/share/Lumina-DE/desktop-background.jpg"; }
  //Now set this file as the current background
  QString cmd = "xv +24 -rmode 5 -quit \""+bgFile+"\"";
  QProcess::startDetached(cmd);
  //Now reset the timer for the next change (if appropriate)
  if(bgL.length() > 1){
    //get the length of the timer (in minutes)
    int min = settings->value("background/minutesToChange",5).toInt();
    //reset the internal timer
    if(bgtimer->isActive()){ bgtimer->stop(); }
    bgtimer->start(min*60000); //convert from minutes to milliseconds
  }
}

// -----------------------
//   PROTECTED FUNCTIONS
// -----------------------
void LDesktop::moveEvent(QMoveEvent *event){
  if(event->pos() != QPoint(0,0)){
    this->move(0,0); //In case the WM does not keep it in the corner
  }
}

