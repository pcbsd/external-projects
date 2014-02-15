//===========================================
//  Lumina-DE source code
//  Copyright (c) 2013, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_TOOLBAR_WIDGET_H
#define _LUMINA_TOOLBAR_WIDGET_H

#include <QLabel>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QObject>
#include <QHBoxLayout>

enum STATES {IDLE, ACTIVE, INACTIVE, NOTIFICATION};

class LTBWidget : public QWidget{
	Q_OBJECT
private:
	QTimer *timedown;
	QMenu *menu;
	QLabel *ICON, *TEXT;
	STATES cstate, pstate;

	void updateBackground(){
	  if(cstate == IDLE){ this->setBackgroundRole(QPalette::NoRole); }
	  else if(cstate == ACTIVE){ this->setBackgroundRole(QPalette::Button); }
	  else if(cstate == INACTIVE){ this->setBackgroundRole(QPalette::Dark); }
	  else if(cstate == NOTIFICATION){ this->setBackgroundRole(QPalette::Highlight); }
	}
	
signals:
	void clicked();
	void longClicked();
	void wheelScroll(int change);
	
public:
	LTBWidget(QWidget* parent) : QWidget(parent){
	  this->setContentsMargins(0,0,0,0);
	  ICON = new QLabel(this);
	    ICON->setScaledContents(true);
	  TEXT = new QLabel(this);
	    TEXT->setWordWrap(true);
	  QHBoxLayout *layout = new QHBoxLayout(this);
	    layout->setContentsMargins(0,0,0,0);
	    layout->addWidget(ICON);
	    layout->addWidget(TEXT);
	    ICON->setVisible(false);
	    TEXT->setVisible(false);
	  this->setLayout(layout);
	  timedown = new QTimer(this);
		timedown->setSingleShot(true);
		QObject::connect(timedown, SIGNAL(timeout()), this, SIGNAL(longClicked()) );
	}
	~LTBWidget(){ 
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
	void setIcon(QIcon icon){
	  if(icon.isNull()){ ICON->setVisible(false); }
	  else{
	    ICON->setPixmap(icon.pixmap(this->height()));
	    ICON->setVisible(true);
	  }
	}
	void setText(QString text){
	  if(text.isEmpty()){ TEXT->setVisible(false); }
	  else{
	    TEXT->setText(text);
	    TEXT->setVisible(true);
	  }
	}
	

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
	void wheelEvent(QWheelEvent *event){
	  int change = event->delta()/120; // 1/15th of a rotation (delta/120) is usually one "click" of the wheel
	  emit wheelScroll(change);
	}
	void enterEvent(QEvent *event){
	  if(cstate == NOTIFICATION){ cstate = pstate; } //return to non-notification state
	  if(cstate == IDLE){ this->setBackgroundRole(QPalette::Highlight); }
	  event->accept();	
	}
	void exitEvent(QEvent *event){
	  updateBackground();
	  event->accept();
	}
};

#endif
