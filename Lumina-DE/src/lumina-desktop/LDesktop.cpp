//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDesktop.h"

LDesktop::LDesktop(int deskNum) : QWidget(){
  this->setWindowFlags( Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  this->setWindowTitle("luminadesktopinterface");
  desktop = new QDesktopWidget();
  //Create the Toolbar (QToolBar does not work unless in a QMainWindow)
  toolbar = new QHBoxLayout;
    toolbar->setContentsMargins(0,0,0,0);
    toolbar->setSpacing(2);
  //Now populate the toolbar
  SetupToolbar(); //always 22 pixels height (systray restriction)
  //Now setup the widget location/size
  this->setLayout(toolbar);
  this->setGeometry(0,0,desktop->availableGeometry(deskNum).width(), 22);
  //Start the automated widgets
  deskbar->start();
  connect(LSession::instance(), SIGNAL(WindowListEvent(XEvent*)), this, SLOT(newXEvent(XEvent*)) );
}

LDesktop::~LDesktop(){
  delete sysmenu;
  delete userTB;
  delete clock;
  //delete spacer
}

// =====================
//   PRIVATE FUNCTIONS 
// =====================
void LDesktop::SetupToolbar(){
  //Create the User Button
  sysmenu = new QMenu(this);
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
  deskbar = new LDeskBar(this);
    deskbar->setHeight(22);
    deskbar->start();
  clock = new LClock(this);
    clock->start();
  systray = new LSysTray(this);
    systray->start();
  mixer = new LMixerWidget(this);
    QTimer::singleShot(0,mixer,SLOT(start())); //don't wait for it to finish starting
  //Now add them all to the toolbar in the proper order
  toolbar->addWidget(userTB);
  toolbar->addWidget(deskbar);
  toolbar->addStretch(1);
  toolbar->addWidget(systray);
  toolbar->addWidget(mixer);
  toolbar->addWidget(clock);

}

// =====================
//     PRIVATE SLOTS 
// =====================

void LDesktop::SystemRun(){
  QString cmd = QInputDialog::getText(0,tr("Run Command"),tr("Command:"));
  if(cmd.isEmpty()){ return; }
  cmd = "("+cmd+") &";
  system(cmd.toUtf8());
}

void LDesktop::newXEvent(XEvent* event){
  //this->move(0,0);
  this->setGeometry(0,0,desktop->width(), 22);
}
