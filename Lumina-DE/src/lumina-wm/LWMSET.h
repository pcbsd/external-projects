#ifndef _LUMINA_WM_SETTINGS_H
#define _LUMINA_WM_SETTINGS_H

#include <QString>

namespace LWMSET{
	enum Position { DISABLED, TOP, BOTTOM, LEFT, RIGHT } // 
	//Sizes (all in pixels)
	int frameWidth = 2;
	int iconSize = 10; //determines title bar size
	Position controlEdge = TOP; //which edge to put the title bar on
	
	//Locations
	QString frameCFG = "%icon%spacer%title%spacer%min%max%close"; //title bar organization
	
	//Configuration
	QString frameStyleSheet = "";
	
	//Icons
	QPixmap closeIcon, minIcon, maxIcon;
}