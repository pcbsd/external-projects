//===========================================
//  Lumina-DE source code
//  Copyright (c) 2013, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_TOOLBAR_WIDGET_H
#define _LUMINA_TOOLBAR_WIDGET_H

#include <QAbstractButton>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QEvent>

class LTBWidget : public QAbstractButton{
	Q_OBJECT
public:
	enum STATES {IDLE, ACTIVE, INACTIVE, NOTIFICATION};
	LTBWidget(QWidget* parent) : QAbstractButton(parent){
	  timedown = new QTimer(this);
		timedown->setSingleShot(true);
		connect(timedown, SIGNAL(timeout()), this, SIGNAL(longClicked()) );
	}
	~LTBWidget(){ 
	  delete timedown; 
	}
	
	void setMenu(QMenu *addmenu){ menu = addmenu; }
	void setLongClickTime( int ms ){ timedown->setInterval(ms); }
	void setState(STATES newstate){
	  if(newstate == NOTIFICATION){ pstate = cstate; }
	  else{ pstate = IDLE; }
	  cstate = newstate;
	  updateBackground();
	}
	
public slots:
	void showMenu(){ menu->popup( this->pos()); }
	
private:
	QTimer *timedown;
	QMenu *menu;
	STATES cstate, pstate;

	void updateBackground(){
	  if(cstate == IDLE){ this->setBackgroundRole(QPalette::NoRole); }
	  else if(cstate == ACTIVE){ this->setBackgroundRole(QPalette::Button); }
	  else if(cstate == INACTIVE){ this->setBackgroundRole(QPalette::Dark); }
	  else if(cstate == NOTIFICATION){ this->setBackroundRole(QPalette::Highlight); }
	}
	
signals:
	void longClicked();

protected:
	void mousePressEvent(QMouseEvent *event){ 
		timedown->start(); 
		event->accept();
	}
	void mouseReleaseEvent(QMouseEvent *event){ 
		if(timedown->isActive()){ emit clicked(); }
		timedown->stop(); 
		event->accept();
	}
	void enterEvent(QEvent *event){
	  if(cstate == NOTIFICATION){ cstate = pstate; } //return to non-notification state
	  if(cstate == IDLE){ this->setBackgroundRole(QPalette::Light); }
	  event->accept();	
	}
	void exitEvent(QEvent *event){
	  updateBackground();
	  event->accept();
	}
};

#endif
