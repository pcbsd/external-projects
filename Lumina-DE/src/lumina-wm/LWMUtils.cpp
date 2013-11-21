//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "lumina-wm/LWMUtils.h"

Atom LWMUtils::getAtom(char* aname){
  return XInternAtom(QX11Info::display(), aname, false); 
}

bool LWMUtils::getProperty(Atom property, Atom propertyType, unsigned char** propRet, 
			unsigned long* num, long offset, long length) const{
  Atom aout;
  int size;
  unsigned long bytes;
  int ret = XGetWindowProperty(QX11Info::display(), AID, property, offset, length,
  	  			false, propertyType, &aout, &size, num, &bytes, propRet);
  bool ok = ( (ret == Success) && (*num >= 1) );
}

