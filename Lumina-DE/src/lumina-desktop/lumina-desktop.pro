
QT       += core gui network

TARGET = Lumina-DE
target.path = /usr/local/bin

TEMPLATE = app


SOURCES += main.cpp \
	WMProcess.cpp \
	LSession.cpp \
	LDesktop.cpp \
	LClock.cpp \
	LSysTray.cpp \
	LDeskBar.cpp \
	AppMenu.cpp
	


HEADERS  += Globals.h \
	WMProcess.h \
	LSession.h \
	LDesktop.h \
	LClock.h \
	LSysTray.h \
	LDeskBar.h \
	LTBWidget.h \
	AppMenu.h

FORMS    += 

RESOURCES+= Lumina-DE.qrc

desktop.files = Lumina-DE.desktop
desktop.path = /usr/local/share/xsessions

icons.files = Lumina.png
icons.path = /usr/local/share/pixmaps

defaulticons.files = default-icons/
defaulticons.path = /usr/local/share/Lumina-DE/

defaultbackground.files = images/desktop-background.jpg
defaultbackground.path = /usr/local/share/Lumina-DE/

LIBS     += -lLuminaUtils -lX11 -lXrender

TRANSLATIONS =  i18n/Lumina-DE_af.ts \
                i18n/Lumina-DE_ar.ts \
                i18n/Lumina-DE_az.ts \
                i18n/Lumina-DE_bg.ts \
                i18n/Lumina-DE_bn.ts \
                i18n/Lumina-DE_bs.ts \
                i18n/Lumina-DE_ca.ts \
                i18n/Lumina-DE_cs.ts \
                i18n/Lumina-DE_cy.ts \
                i18n/Lumina-DE_da.ts \
                i18n/Lumina-DE_de.ts \
                i18n/Lumina-DE_el.ts \
                i18n/Lumina-DE_en_GB.ts \
                i18n/Lumina-DE_en_ZA.ts \
                i18n/Lumina-DE_es.ts \
                i18n/Lumina-DE_et.ts \
                i18n/Lumina-DE_eu.ts \
                i18n/Lumina-DE_fa.ts \
                i18n/Lumina-DE_fi.ts \
                i18n/Lumina-DE_fr.ts \
                i18n/Lumina-DE_fr_CA.ts \
                i18n/Lumina-DE_gl.ts \
                i18n/Lumina-DE_he.ts \
                i18n/Lumina-DE_hi.ts \
                i18n/Lumina-DE_hr.ts \
                i18n/Lumina-DE_hu.ts \
                i18n/Lumina-DE_id.ts \
                i18n/Lumina-DE_is.ts \
                i18n/Lumina-DE_it.ts \
                i18n/Lumina-DE_ja.ts \
                i18n/Lumina-DE_ka.ts \
                i18n/Lumina-DE_ko.ts \
                i18n/Lumina-DE_lt.ts \
                i18n/Lumina-DE_lv.ts \
                i18n/Lumina-DE_mk.ts \
                i18n/Lumina-DE_mn.ts \
                i18n/Lumina-DE_ms.ts \
                i18n/Lumina-DE_mt.ts \
                i18n/Lumina-DE_nb.ts \
                i18n/Lumina-DE_nl.ts \
                i18n/Lumina-DE_pa.ts \
                i18n/Lumina-DE_pl.ts \
                i18n/Lumina-DE_pt.ts \
                i18n/Lumina-DE_pt_BR.ts \
                i18n/Lumina-DE_ro.ts \
                i18n/Lumina-DE_ru.ts \
                i18n/Lumina-DE_sk.ts \
                i18n/Lumina-DE_sl.ts \
                i18n/Lumina-DE_sr.ts \
                i18n/Lumina-DE_sv.ts \
                i18n/Lumina-DE_sw.ts \
                i18n/Lumina-DE_ta.ts \
                i18n/Lumina-DE_tg.ts \
                i18n/Lumina-DE_th.ts \
                i18n/Lumina-DE_tr.ts \
                i18n/Lumina-DE_uk.ts \
                i18n/Lumina-DE_uz.ts \
                i18n/Lumina-DE_vi.ts \
                i18n/Lumina-DE_zh_CN.ts \
                i18n/Lumina-DE_zh_HK.ts \
                i18n/Lumina-DE_zh_TW.ts \
                i18n/Lumina-DE_zu.ts

dotrans.path=/usr/local/share/Lumina-DE/i18n/
dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm /usr/local/share/Lumina-DE/i18n/

INSTALLS += target desktop icons defaulticons defaultbackground dotrans
