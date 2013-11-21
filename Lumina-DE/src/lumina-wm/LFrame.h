#ifndef _LUMINA_WM_FRAME_H
#define _LUMINA_WM_FRAME_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QFrame>
#include <QCursor>

#include "LWMSET.h"

class LFrame : public QFrame{
	Q_OBJECT
	
public:
	LFrame(QWidget *parent = 0);
	~LFrame();
	//getters
	int getX();
	int getY();
	unsigned int getWidth();
	unsigned int getHeight();

	//setters
	void setVisible(bool);
	void setX(int);
	void setY(int);
	void setWidth(unsigned int);
	void setHeight(unsigned int);

	void setTitle(QString);
	void setIconPixmap( const QPixmap& pixmap);
	
	void setActive(bool);

private:
	enum MSDIR { NONE, TITLE, NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST}
	void initialSetup();
	MSDIR getHoverSection(int x, int y);
	MSDIR hovSec; //hover section
	
	QLabel *label_title, *label_icon;
	QToolButton *tool_min, *tool_max, *tool_close;
	QToolBar *toolbar;
	MSDIR tbSec;

protected:
	void mousePressEvent(QMouseEvent* event); //setup where mouse is clicked
	void mouseMoveEvent(QMouseEvent* event); //move or resize display (while mouse down)
	void mouseReleaseEvent(QMouseEvent* event); //reset mouse clicked variables
	bool eventFilter(QObject*, QEvent*); //setup mouse icons

signals:
	void resized(int,int);
	void minimizeClicked();
	void maximizeClicked();
	void closeClicked();
};

#endif