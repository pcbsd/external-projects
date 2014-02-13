//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LClock.h"

LClock::LClock(QWidget *parent) : QLabel(parent){
  //Setup the widget
  this->setAlignment(Qt::AlignCenter);

  //Setup the timer
  timer = new QTimer();
  timer->setInterval(1000); //update once a second
  connect(timer,SIGNAL(timeout()), this, SLOT(updateTime()) );
  updateTime();
}

LClock::~LClock(){
  timer->stop();
  delete timer;
}

void LClock::start(){
  timer->start();	
}
void LClock::updateTime(){
  QDateTime CT = QDateTime::currentDateTime();
  //Now update the display
  this->setText(CT.toString("h:mm AP")+"\n"+CT.toString("M/d/yy"));
  this->setToolTip(CT.toString("ddd M/d/yy"));
}
