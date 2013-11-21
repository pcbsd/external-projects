#ifndef _LUMINA_WM_LAPP_H
#define _LUMINA_WM_LAPP_H

#include "lumina-wm/ICCCM.h"
#include "lumina-wm/EWMH.h"

class LApp{

public:
	LApp(const Window& ID);
	~LApp();
	
	//Main Interface Functions
	// - Getters
	int getX() const;
	int getY() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getBorderWidth() const;
	unsigned int getMinWidth() const;
	unsigned int getMinHeight() const;
	unsigned int getMaxWidth() const;
	unsigned int getMaxHeight() const;
	int getInitialPosition() const;
	int getInitialState() const; //Withdrawn, Normal, Iconic
	XID getWindowGroup() const;
	Atom getWindowType() const;
	
	QString getTitle() const;
	QString getShortTitle() const;
	QPixmap getIconPixmap() const;
	
	// - Setters
	void setX(int);
	void setY(int);
	void setWidth(unsigned int);
	void setHeight(unsigned int);
	void setBorderWidth(int);
	void setVisible(bool);
	void changeWmState(int); //Withdrawn, Normal, Iconic
	
	void setFocus();
	void killApp();
	
	// - Information
	bool overridePositionHints() const;
	
	
private:
	Window AID; //application ID
	
	//ICCCM Standards Compliance: 
	// -- http://www.tronche.com/gui/x/icccm/
	ICCCM* icccm;
	//EMWM Standards Compliance:
	// -- http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html#id2721765
	EWMH* ewmh;

};

#endif
