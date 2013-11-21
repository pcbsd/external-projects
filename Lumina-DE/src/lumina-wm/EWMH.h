#ifndef _LUMINA_WM_EWMH_H
#define _LUMINA_WM_EWMH_H

#include "lumina-wm/LWMUtils.h"

class EWMH{

public:
	EWMH(const Window& ID);
	~EWMH();
	
	QString getTitle() const;
	QString getVisibleTitle() const;
	QPixmap getIconPixmap() const;
	
	Atom getWindowType() const;
	
	//General EWMH setters
	static void setSupportedHints();
	static void setClientList(QList<LWindow*> list);
	static void setClientStackList(QList<LWindow*> list);
	
private:
	Window* AID;
	
};
#endif
