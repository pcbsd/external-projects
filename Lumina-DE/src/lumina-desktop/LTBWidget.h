//===========================================
//  Lumina-DE source code
//  Copyright (c) 2013, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_TOOLBAR_WIDGET_H
#define _LUMINA_TOOLBAR_WIDGET_H

#include <QToolButton>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QObject>
#include <QHBoxLayout>

enum STATES {IDLE, ACTIVE, INACTIVE, NOTIFICATION};

class LTBWidget : public QToolButton{
	Q_OBJECT
private:
	STATES cstate, pstate;

	void updateBackground(){
	  if(cstate == IDLE){ this->setBackgroundRole(QPalette::NoRole); }
	  else if(cstate == ACTIVE){ this->setBackgroundRole(QPalette::Button); }
	  else if(cstate == INACTIVE){ this->setBackgroundRole(QPalette::Dark); }
	  else if(cstate == NOTIFICATION){ this->setBackgroundRole(QPalette::Highlight); }
	}
	
signals:

	void wheelScroll(int change);
	
public:
	LTBWidget(QWidget* parent) : QToolButton(parent){
	  this->setStyleSheet( this->styleSheet()+" LTBWidget::menu-indicator{image: none;}");
	  cstate = IDLE;
	  //this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	  this->setPopupMode(QToolButton::InstantPopup);
	  this->setAutoRaise(true);

	}
	
	~LTBWidget(){ 
	}
	
	/*void setVisuals(QString txt, QIcon ico = QIcon()){
	  this->setText(txt);
	  this->setIcon(ico);
	  if(txt.isEmpty()){ this->setToolButtonStyle(Qt::ToolButtonIconOnly); }
	  else{ this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); }
	}*/
	
	void setState(STATES newstate){
	  if(newstate == NOTIFICATION){ pstate = cstate; }
	  else{ pstate = IDLE; }
	  cstate = newstate;
	  updateBackground();
	}
	
public slots:
	

protected:
	void wheelEvent(QWheelEvent *event){
	  int change = event->delta()/120; // 1/15th of a rotation (delta/120) is usually one "click" of the wheel
	  emit wheelScroll(change);
	}

};

#endif
