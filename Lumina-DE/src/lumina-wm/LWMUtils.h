#ifndef _LUMINA_WM_UTILS_H
#define _LUMINA_WM_UTILS_H

class LWMUtil{

public:
	static Atom getAtom(char*);
	static bool getProperty(Atom property, Atom propertyType, unsigned char** propRet, unsigned long* num, long offset = 0, long length = 100) const;
};

#endif

