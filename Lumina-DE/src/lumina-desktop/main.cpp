//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================

#include <QDebug>
#include <QMainWindow>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QDesktopWidget>
#include <QSettings>

#include "WMProcess.h"
#include "Globals.h"
#include "LDesktop.h"
#include "LSession.h"

#include <LuminaXDG.h> //from libLuminaUtils

QFile logfile(QDir::homePath()+"/.lumina.log");
void MessageOutput(QtMsgType type, const char *msg){
  QString txt;
  switch(type){
  case QtDebugMsg:
  	  txt = QString("Debug: %1").arg(msg);
  	  break;
  case QtWarningMsg:
  	  txt = QString("Warning: %1").arg(msg);
  	  break;
  case QtCriticalMsg:
  	  txt = QString("CRITICAL: %1").arg(msg);
  	  break;
  case QtFatalMsg:
  	  txt = QString("FATAL: %1").arg(msg);
  	  break;
  }

  QTextStream out(&logfile);
  out << txt;
  if(!txt.endsWith("\n")){ out << "\n"; }
}
int main(int argc, char ** argv)
{
    //Setup any pre-QApplication initialization values
    LXDG::setEnvironmentVars();
    setenv("DESKTOP_SESSION","LUMINA",1);
    setenv("XDG_CURRENT_DESKTOP","LUMINA",1);
    //Setup the log file
    qDebug() << "Lumina Log File:" << logfile.fileName();
    if(logfile.exists()){ logfile.remove(); } //remove any old one
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    //Startup the Application
    LSession a(argc, argv);
    //Setup Log File
    qInstallMsgHandler(MessageOutput);
    //Setup the QSettings
    bool firstrun = false;
    if(!QFile::exists(QDir::homePath()+"/.lumina")){ firstrun = true; }
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath()+"/.lumina/settings");
    qDebug() << "Initializing Lumina";
    //Start up the Window Manager
    qDebug() << " - Start Window Manager";
    WMProcess WM;
    WM.startWM();
    //Now start the desktop
    qDebug() << " - Start Desktop";
    //QDesktopWidget DW;
    //for(int i=0; i<DW.screenCount(); i++){
      LDesktop *w = new LDesktop();
      //if( i==0 ){
        QObject::connect(&WM, SIGNAL(WMShutdown()), w, SLOT(SystemLogout()) );
        QObject::connect(w, SIGNAL(Finished()),&a,SLOT(closeAllWindows()) );
      //}
      w->move(0,0);
      w->show();
    //}
    a.processEvents();
    int retCode = a.exec();
    qDebug() << "Stopping the window manager";
    WM.stopWM();
    logfile.close();
    qDebug() << "Finished Closing Down Lumina";
    return retCode;
}
