//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LMixer.h"

LMixerWidget::LMixerWidget(QWidget *parent) : QToolButton(parent){
  //this->setStyleSheet( "LMixerWidget { background-color: transparent; border: none; padding: none} LMixerWidget::menu-indicator{ image: none; }");
  this->setPopupMode(QToolButton::InstantPopup);
  this->setToolButtonStyle(Qt::ToolButtonIconOnly);
  this->setAutoRaise(true);
  this->setContentsMargins(0,0,0,0);
  this->setFixedSize(22,22);
}

LMixerWidget::~LMixerWidget(){

}

void LMixerWidget::start(){
  //this->setScaledContents(true); //just to make sure the icon is scaled properly
  //Initial setup for the widget
  slider = new QSlider(this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setTickInterval(25);
    slider->setOrientation(Qt::Vertical);
  sliderA = new QWidgetAction(this);
    sliderA->setDefaultWidget(slider);
  mute = new QToolButton(this);
    mute->setText(tr("Mute"));
  muteA = new QWidgetAction(this);
    muteA->setDefaultWidget(mute);
  menu = new QMenu(this);
    menu->addAction( sliderA );
    menu->addAction( muteA );
  this->setMenu(menu);
  //Get the initial values of the mixer
  getVolume();
  //Setup the update timer
  timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(50); //50 ms delay to prevent calling the backend too frequently
  //Connect the signals/slots
  connect(timer, SIGNAL(timeout()), this, SLOT(updateMixer()) );
  connect(slider, SIGNAL(actionTriggered(int)), this, SLOT(sliderChanged()) );
  connect(mute, SIGNAL(clicked()), this, SLOT(muteClicked()) );
}


// =======================
//    PRIVATE FUNCTIONS
// =======================
void LMixerWidget::updateIcon(){
  //Update the mute button icon
  QString DID = "/usr/local/share/Lumina-DE/default-icons/"; //default-icon-dir
  if(isMuted){ 
    mute->setIcon( LXDG::findIcon("audio-volume-high", DID+"audio-volume-high.png") ); 
    this->setToolTip("0%");
  }else{ 
    mute->setIcon( LXDG::findIcon("audio-volume-muted", DID+"audio-volume-muted.png") ); 
    this->setToolTip(QString::number(CVOL)+"%");
  }
  //Update the main widget icon
  if(isMuted){
    this->setIcon( LXDG::findIcon("audio-volume-muted", DID+"audio-volume-muted.png") );  
  }else if(slider->value() < 33){
    this->setIcon( LXDG::findIcon("audio-volume-low", DID+"audio-volume-low.png") ); 	  
  }else if(slider->value() < 66){
    this->setIcon( LXDG::findIcon("audio-volume-medium", DID+"audio-volume-medium.png") );   
  }else{
    this->setIcon( LXDG::findIcon("audio-volume-high", DID+"audio-volume-high.png") ); 
  }
  this->setIconSize(QSize(20,20));
}

void LMixerWidget::setVolume(int vol){
  //Make sure it is between 0-100
  if(vol < 0){ vol = 0; }
  else if(vol > 100){ vol = 100; }
  //Now run the command
  QString cmd = "mixer vol "+QString::number(vol);
  system(cmd.toUtf8());
}

void LMixerWidget::getVolume(){
  QString cmd = "mixer -S";
  //run cmd
  QStringList out = LUtils::getCmdOutput(cmd);
  for(int i=0; i<out.length(); i++){
    if(out[i].section(":",0,0) == "vol"){
      CVOL = out[i].section(":",1,1).toInt(); //assume the left/right balance is equal, so use the left
      break;
    }
  }
  slider->setValue(CVOL);
  isMuted = (CVOL <= 0);
  updateIcon();
}


// =======================
//      PRIVATE SLOTS
// =======================
void LMixerWidget::updateMixer(){
  int vol = slider->value();
  setVolume(vol);
  CVOL = vol;
  if(vol <= 0){ isMuted = true; }
  else{ isMuted = false; }
  updateIcon();
}

void LMixerWidget::sliderChanged(){
  timer->start(); //ping the timer to make sure 
}

void LMixerWidget::muteClicked(){
  if(CVOL == 0){ CVOL = 50; } //muted by slider - default to half volume	  
  if(isMuted){
    //Unmute the mixer
    setVolume(CVOL);
    slider->setValue(CVOL);
    isMuted = false;
  }else{
    //Mute the mixer (leaving CVOL as-is)
    setVolume(0);
    slider->setValue(0);
    isMuted = true;
  }
  updateIcon();
}

// ======================
//      PROTECTED
// ======================
void LMixerWidget::wheelEvent(QWheelEvent *event){
  //Change the current volume
  int change = event->delta()/40; // 3% volume change per 1/15th of a rotation (delta*3/120)
  if(isMuted && change<=0){} //do nothing - already muted
  else if(isMuted){ slider->setValue(change); sliderChanged(); } //unmute
  else{ slider->setValue( slider->value() + change ); sliderChanged(); } //difference from current setting
  event->accept();
}

/*void LMixerWidget::mouseReleaseEvent(QMouseEvent *event){
  //Show the menu
  menu->popup(event->globalPos());
  event->accept();
}*/

