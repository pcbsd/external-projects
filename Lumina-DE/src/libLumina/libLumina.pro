
QT       += core

TARGET=LuminaUtils
target.path = /usr/local/lib

DESTDIR= $$_PRO_FILE_PWD_/

TEMPLATE	= lib
LANGUAGE	= C++
VERSION		= 1

HEADERS	+= LuminaXDG.h \
	LuminaUtils.h

SOURCES	+= LuminaXDG.cpp \
	LuminaUtils.cpp

include.path=/usr/local/include/
include.files=LuminaXDG.h \
	LuminaUtils.h

INSTALLS += target include 

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
