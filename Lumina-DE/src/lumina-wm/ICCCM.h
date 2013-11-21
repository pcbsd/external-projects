#ifndef _LUMINA_WM_ICCCM_H
#define _LUMINA_WM_ICCCM_H

class ICCCM{

public:
	ICCCM(const Window& AID);
	~ICCCM();
	
	//Main Access Functions
	// - Getters
	QString getTitle() const; //WM_NAME
	QString getShortTitle() const; //WM_ICON_NAME
	int getMinWidth() const;  //in Pixels
	int getMinHeight() const; //in Pixels
	int getMaxWidth() const;  //in Pixels
	int getMaxHeight() const; //in Pixels
	bool overridePositionHints() const;
	int getInitialPosition() const; //Gravity Compass Positions
	int getInitialState() const; //Withdrawn, Normal, Iconic
	
	XID getWindowGroup() const;
	QPixmap getIconPixmap() const;
	
	// - Setters
	void setFocus() const;
	void killApp() const;
	void changeWmState(int) const;
	void setIconSizes();
	
private:
	//Internal Variables
	Window AID;
	static const unsigned int DEFAULT_MIN_SIZE;
	static const unsigned int DEFAULT_MAX_SIZE;
	//Conversion/Utility Functions
	QString textPropertyToString(XTextProperty) const;
	XSizeHints getSizeHints() const;
	bool isSupportProtocol(const char* atomname) const;
	void sendProtocol(const char* protocol) const;
	
};

#endif
