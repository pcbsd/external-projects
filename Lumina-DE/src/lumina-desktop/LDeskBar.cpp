//===========================================
//  Lumina-DE source code
//  Copyright (c) 2012, Ken Moore
//  Available under the 3-clause BSD license
//  See the LICENSE file for full details
//===========================================
#include "LDeskBar.h"

LDeskBar::LDeskBar(QWidget *parent) : QWidget(parent){
  this->setObjectName("desktoptoolbar");
  //Find the path to the desktop folder
  if(QFile::exists(QDir::homePath()+"/Desktop")){ desktopPath = QDir::homePath()+"/Desktop"; }
  else if(QFile::exists(QDir::homePath()+"/desktop")){ desktopPath = QDir::homePath()+"/desktop"; }
  else{ desktopPath=""; }
  //Setup the filter lists for the different types of files
  audioFilter <<"*.ogg"<<"*.mp3"<<"*.wav"<<"*.aif"<<"*.iff"<<"*.m3u"<<"*.m4a"<<"*.mid"<<"*.mpa"<<"*.ra"<<"*.wma";
  videoFilter <<"*.3g2"<<"*.3gp"<<"*.asf"<<"*.asx"<<"*.avi"<<"*.flv"<<"*.m4v"<<"*.mov"<<"*.mp4"<<"*.mpg"<<"*.rm"<<"*.srt"<<"*.swf"<<"*.vob"<<"*.wmv";
  pictureFilter <<"*.bmp"<<"*.dds"<<"*.gif"<<"*.jpg"<<"*.png"<<"*.psd"<<"*.thm"<<"*.tif"<<"*.tiff"<<"*.ai"<<"*.eps"<<"*.ps"<<"*.svg"<<"*.ico";
  //initialize the widget layout
  layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(2);
    layout->setAlignment(Qt::AlignCenter);
  this->setLayout(layout);
  //Setup the style sheet
  QString style = "background-color: transparent;";
  this->setStyleSheet(style);
  //initialize the desktop bar items
  initializeDesktop();
  //setup the directory watcher
  watcher = new QFileSystemWatcher(this);
  if(!desktopPath.isEmpty()){ 
    watcher->addPath(desktopPath); 
  }

  
}

LDeskBar::~LDeskBar(){
  if(!desktopPath.isEmpty()){
    watcher->removePath(desktopPath);
    disconnect(watcher);
  }
  delete watcher;
  
}

void LDeskBar::setHeight(int pix){
  //Reset the height of the widget and all objects
  this->resize(this->width(), pix);
  //Now resize all the special buttons
  QSize size(pix,pix);
  homeB->setIconSize(size);
  dirB->setIconSize(size);
  audioB->setIconSize(size);
  videoB->setIconSize(size);
  pictureB->setIconSize(size);
  fileB->setIconSize(size);
  //Now resize all the other buttons
  for(int i=0; i<exeList.length(); i++){
    exeList[i]->setIconSize(size);
    exeList[i]->defaultAction()->setIcon(QIcon(exeList[i]->defaultAction()->iconText()));
    //re-load the item in the layout for the item to get properly resized
    layout->removeWidget(exeList[i]);
    layout->addWidget(exeList[i]);
  }
}

void LDeskBar::start(){
  //Now update the desktop bar in a different thread
  QTimer::singleShot(1,this,SLOT(desktopChanged()) );
  connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(desktopChanged()) );
}
// =======================
//   PRIVATE FUNCTIONS
// =======================
void LDeskBar::initializeDesktop(){
  //Home button (important directories in the home dir - only created this once)
  homeB = new QToolButton(this);
    homeB->setPopupMode(QToolButton::InstantPopup);
    homeB->setDefaultAction(new QAction(QIcon(":/images/default-home.png"),"",this));
  homeM = new QMenu(this);
    connect(homeM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    QDir home = QDir::home();
    QFileInfoList homeDirs = home.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for(int i=0; i<homeDirs.length(); i++){
      if(homeDirs[i].fileName() == "bin"){} //ignore the binary directory
      else if(homeDirs[i].fileName().toLower() == "music"){ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-audiodir.png") ); }
      else if(homeDirs[i].fileName().toLower() == "videos"){ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-videodir.png") ); }
      else if(homeDirs[i].fileName().toLower() == "images"){ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-graphicsdir.png") ); }
      else if(homeDirs[i].fileName().toLower() == "documents"){ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-textdir.png") ); }
      else if(homeDirs[i].fileName().toLower() == "downloads"){ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-netdir.png") ); }
      else if(homeDirs[i].fileName().toLower() == "desktop"){ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-desktop.png") ); }
      else{ homeM->addAction( newAction(homeDirs[i].canonicalFilePath(), homeDirs[i].fileName(), ":/images/default-dir.png") ); }
    }
    homeB->setMenu(homeM);
    layout->addWidget(homeB);
  //Directories on the desktop
  dirB = new QToolButton(this);
    dirB->setPopupMode(QToolButton::InstantPopup);
    dirB->setDefaultAction(new QAction(QIcon(":/images/default-dir.png"),"",this));
  dirM = new QMenu(this);
    dirB->setMenu(dirM);
    layout->addWidget(dirB);
    connect(dirM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
  //Audio Files on the desktop
  audioB = new QToolButton(this);
    audioB->setPopupMode(QToolButton::InstantPopup);
    audioB->setDefaultAction(new QAction(QIcon(":/images/default-audiofile.png"),"",this));
  audioM = new QMenu(this);
    connect(audioM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    audioB->setMenu(audioM);
    layout->addWidget(audioB);
  //Video Files on the desktop
  videoB = new QToolButton(this);
    videoB->setPopupMode(QToolButton::InstantPopup);
    videoB->setDefaultAction(new QAction(QIcon(":/images/default-video.png"),"",this));
  videoM = new QMenu(this);
    connect(videoM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    videoB->setMenu(videoM);
    layout->addWidget(videoB);
  //Picture Files on the desktop
  pictureB = new QToolButton(this);
    pictureB->setPopupMode(QToolButton::InstantPopup);
    pictureB->setDefaultAction(new QAction(QIcon(":/images/default-image.png"),"",this));
  pictureM = new QMenu(this);
    connect(pictureM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    pictureB->setMenu(pictureM);
    layout->addWidget(pictureB);
  //Other Files on the desktop
  fileB = new QToolButton(this);
    fileB->setPopupMode(QToolButton::InstantPopup);
    fileB->setDefaultAction(new QAction(QIcon(":/images/default-file.png"),"",this));
  fileM = new QMenu(this);
    connect(fileM,SIGNAL(triggered(QAction*)),this,SLOT(ActionTriggered(QAction*)) );
    fileB->setMenu(fileM);
    layout->addWidget(fileB);
}

bool LDeskBar::readDesktopFile(QString path, QString &name, QString &iconpath){
  name.clear(); iconpath.clear();
  QFile file(path);
  QString locale = QLocale::system().name();
  bool ok = false;
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){ return ok; }
  QTextStream in(&file);
  QString tryexec; bool hidden=false;
  while(!in.atEnd()){
    QString line = in.readLine();
    line = line.simplified();
    QString var = line.section("=",0,0).simplified();
    QString loc = var.section("[",1,1).section("]",0,0).simplified(); // localization
    var = var.section("[",0,0).simplified(); //remove the localization
    QString val = line.section("=",1,1).simplified();
    //-------------------
    if(var=="Name"){ 
      if(name.isEmpty() && loc.isEmpty()){ name = val; }
      else if(loc == locale){ name = val; }
    }else if(var=="Icon"){ iconpath = val; }
    else if(var=="TryExec"){ tryexec = val; }
    else if(var=="NoDisplay" && !hidden){ hidden = (val.toLower()=="true"); }
    else if(var=="Hidden" && !hidden){ hidden = (val.toLower()=="true"); }
  }
  file.close();
  //Check the icon
  if( iconpath.isEmpty() || !QFile::exists(iconpath) ){
    iconpath = ":/images/default-application.png";	  
  }
  //Check for validity
  ok=true;
  if(hidden){ ok = false; }
  else if(!tryexec.isEmpty()){
    if(!tryexec.startsWith("/")){ ok = searchForExe(tryexec); }
    else{ ok = QFile::exists(tryexec); }
  }
  return ok;
}

QAction* LDeskBar::newAction(QString filepath, QString name, QString iconpath){
  QAction *act = new QAction(QIcon(iconpath), name, this);
    act->setWhatsThis(filepath);
    //act->setIconText(iconpath);
  return act;
}

QToolButton* LDeskBar::newLauncher(QString filepath, QString name, QString iconpath){
  QToolButton *tb = new QToolButton(this);
    tb->setDefaultAction( newAction(filepath, "", iconpath) );
    tb->setToolTip(name);
    connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(ActionTriggered(QAction*)) );
  return tb;
}

void LDeskBar::updateMenu(QMenu* menu, QFileInfoList files, bool trim){
  menu->clear();
  //re-create the menu (since it is hidden from view)
  for(int i=0; i<files.length(); i++){
    qDebug() << "New Menu Item:" << files[i].fileName();
    if(trim){ totals.removeAll(files[i]); }
    menu->addAction( newAction( files[i].canonicalFilePath(), files[i].fileName(), "") );
  }
}

bool LDeskBar::searchForExe(QString filename){
  if(filename.isEmpty()){ return false; }
  QDir dir;
  bool found = false;
  QStringList paths = QString(getenv("PATH")).split(":");
  for(int i=0; i<paths.length(); i++){
    if(QFile::exists(paths[i]+"/"+filename)){ found = true; break; }
  }
  return found;
}

// =======================
//     PRIVATE SLOTS
// =======================
void LDeskBar::ActionTriggered(QAction* act){
 //Open up the file with the appropriate application
 QString cmd = "lumina-open "+act->whatsThis();
 //
 
 qDebug() << "Open File:" << cmd;
 QProcess::startDetached(cmd);
}

void LDeskBar::desktopChanged(){
  if(!desktopPath.isEmpty()){
    QDir dir(desktopPath);
    totals = dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    //Update all the special menus (trimming the totals list as we go)
    updateMenu(dirM, dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name) );
    updateMenu(audioM, dir.entryInfoList( audioFilter, QDir::Files, QDir::Name) );
    updateMenu(videoM, dir.entryInfoList( videoFilter, QDir::Files, QDir::Name) );
    updateMenu(pictureM, dir.entryInfoList( pictureFilter, QDir::Files, QDir::Name) );
    //Now update the launchers
    QFileInfoList exe = dir.entryInfoList( QStringList() << "*.desktop", QDir::Files, QDir::Name );
      for(int i=0; i<exe.length(); i++){ totals.removeAll(exe[i]); } //remove these items from the totals
    // - verify that the current launchers are still valid
    for(int i=0; i<exeList.length(); i++){
      bool remove=true;
      QString junk1, junk2;
      if(exe.removeAll( QFileInfo(exeList[i]->whatsThis())) > 0){
      	//launcher still there - check validity
      	remove = !readDesktopFile(exeList[i]->whatsThis(), junk1, junk2);
      }
      if(remove){
        layout->removeWidget(exeList[i]);
        delete exeList.takeAt(i);
      }
    }
    //Now add any new launchers to the end
    for(int i=0; i<exe.length(); i++){
      QString name, iconpath;
      if( readDesktopFile(exe[i].canonicalFilePath(), name, iconpath) ){
        QToolButton *tb = newLauncher(exe[i].canonicalFilePath(), name, iconpath);
        exeList << tb;
        layout->addWidget(tb);
      }
    }
    //Now update the files menu with everything else that is left
    updateMenu(fileM, totals, false);
  }	
  //Setup the visibility of the special items
  homeB->setVisible( !homeM->isEmpty() );
  dirB->setVisible( !dirM->isEmpty() );
  audioB->setVisible( !audioM->isEmpty() );
  videoB->setVisible( !videoM->isEmpty() );
  pictureB->setVisible( !pictureM->isEmpty() );
  fileB->setVisible( !fileM->isEmpty() );
  //Clear the totals list (since no longer in use)
  totals.clear();
}
	
