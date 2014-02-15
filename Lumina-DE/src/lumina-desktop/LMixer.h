//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#ifndef _LUMINA_DESKTOP_MIXER_H
#define _LUMINA_DESKTOP_MIXER_H

//This is just a graphical front-end to the FreeBSD "mixer" command
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>
#include <QToolButton>
#include <QSlider>
#include <QWheelEvent>
#include <QTimer>

#include <LuminaUtils.h>
#include <LuminaXDG.h>

#include "LTBWidget.h"

class LMixerWidget : public LTBWidget{
	Q_OBJECT
public:
	LMixerWidget(QWidget *parent = 0);
	~LMixerWidget();
	
public slots:
	void start();
	
private:
	int CVOL;
	bool isMuted;
	//QMenu *menu;
	QSlider *slider;
	QToolButton *mute;
	QTimer *timer;
	QWidgetAction *sliderA, *muteA;
	
	//Interface adjustment
	void updateIcon();
	//Quick mixer volume interface
	void setVolume(int vol);
	void getVolume();
	
private slots:
	void updateMixer();
	void sliderChanged();
	void muteClicked();
	void wheelChanged(int delta);
	
//protected:
	//void wheelEvent(QWheelEvent *event); //mouse wheel events
	//void mouseReleaseEvent(QMouseEvent *event); //mouse release event (widget clicked)
};
#endif

