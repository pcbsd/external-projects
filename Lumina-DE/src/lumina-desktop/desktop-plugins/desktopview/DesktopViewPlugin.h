//===========================================
//  Lumina-DE source code
//  Copyright (c) 2014, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
//  This class is the interface to load all the different desktop plugins
//===========================================
#ifndef _LUMINA_DESKTOP_VIEW_PLUGIN_H
#define _LUMINA_DESKTOP_VIEW_PLUGIN_H

class DesktopViewPlugin : public LDPlugin{
	Q_OBJECT
public:
	DesktopViewPlugin(QWidget *parent = 0);
	~DesktopViewPlugin();

private:

};
#endif