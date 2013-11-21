#include "lumina-wm/LFrame.h"

LFrame::LFrame(QWidget *parent) : QFrame(parent){
  hovSec = NONE:
  initialSetup();
  qApp->installEventFilter(this);
  connect(tool_close, SIGNAL(clicked()), this, SIGNAL(closeClicked()) );
  connect(tool_min, SIGNAL(clicked()), this, SIGNAL(minimizeClicked()) );
  connect(tool_max, SIGNAL(clicked()), this, SIGNAL(maximizeClicked()) );
}

LFrame::~LFrame(){
  delete toolbar, tool_min, tool_max, tool_close, label_icon, label_title;
}
//===================
// == PUBLIC FUNCTIONS ==
//===================
// ----- getters -----
int LFrame::getX(){
  return this->x();
}

int LFrame::getY(){
  return this->y();
}

unsigned int LFrame::getWidth(){
  return this->width();
}

unsigned int LFrame::getHeight(){
  return this->height();
}


// ----- setters ------
void LFrame::setVisible(bool visible){
  this->setVisible(visible);
}

void LFrame::setX(int x){
  this->move( x, this->y() );
}

void LFrame::setY(int y){
  this->move( this->x(), y );
}

void LFrame::setWidth(unsigned int w){
  this->resize(w, this->height());
  resetToolbar();
}

void LFrame::setHeight(unsigned int h){
  this->resize(this->width(), h);
  resetToolbar();
}

void LFrame::setTitle(QString title){
  label_title->setText(title);
}

void LFrame::setIconPixmap( const QPixmap& pixmap){
  label_icon->setPixmap( pixmap );
}

void LFrame::setActive(bool active){
  label_icon->setEnabled(active);
  label_title->setEnabled(active);
}

void LFrame::updateTheme(){
  //Setup the frame width
  int totWidth = frameWidth;
  this->setLineWidth(totWidth/2);
  this->setMidLineWidth( totWidth - this->lineWidth() );
  //Setup the toolbar location
  LWMSET::Position loc = LWMSET::controlEdge;
  if(loc == LWMSET::TOP){ tbSec = NORTH; }
  else if(loc == LWMSET::BOTTOM){ tbSec = SOUTH; }
  else if(loc == LWMSET::LEFT){ tbSec = WEST; }
  else if(loc == LWMSET::RIGHT){ tbSec = EAST; }
  else if(loc == LWMSET::DISABLED){ tbSec = NONE; }
  else{ tbSec = NORTH; }
  //Setup the toolbar size
  toolbar->setIconSize( QSize(LWMSET::iconSize, LWMSET::iconSize) );
  
  //Load the items onto the toolbar
  QStringList items = LWMSET::frameCFG.split("%");
  toolbar.clear();
  for(int i=0; i<items.length(); i++){
    QString item = items[i].toLower().simplified();
    if(item == "icon"){ toolbar->addWidget(label_icon); }
    else if(item == "spacer"){ 
	    QWidget *spacer = new QWidget();
	    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	    toolbar->addWidget(spacer);
    }else if(item == "title"){ toolbar->addWidget(label_title); }
    else if(item == "min"){ toolbar->addWidget(tool_min); }
    else if(item == "max"){ toolbar->addWidget(tool_max); }
    else if(item == "close"){ toolbar->addWidget(tool_close); }
  }
  //Set the stylesheet for the frame
  this->setStyleSheet(LWMSET::frameStyleSheet);
  //Now adjust all the toolbar sizing appropriately
  resetToolbar();
	
	
}

//===================
// == PRIVATE FUNCTIONS ==
//===================
void LFrame::initialSetup(){
  label_title = new QLabel();
	label_title->setTextFormat(Qt::PlainText);
	
  label_icon = new QLabel();
  tool_min = new QToolButton();
  tool_max = new QToolButton();
  tool_close = new QToolButton();
  toolbar = new QToolBar();
	toolbar->setFloatable(false);
	toolbar->setMovable(false);
	toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

  //Now update the widget based on the current theme
  updateTheme();
}

void LFrame::resetToolbar(){
  //Make sure the toolbar is in the proper spot and length
  int t = this->frameWidth();
  int x = this->x(); int y = this->y();
  int tby = toolbar->iconSize()->height();
  int tbx = toolbar->iconSize()->width();
  switch (tbSide){
    case NORTH:
      toolbar->setOrientation(Qt::Horizontal);
      toolbar->move(x+t,y+t);
      toolbar->resize( this->width() - (2*t), tby );
      toolbar->setVisible(true);
      break;
    case SOUTH:
      toolbar->setOrientation(Qt::Horizontal);
      toolbar->move(x+t,y+this->height() - (2*t) - tby);
      toolbar->resize( this->width() - (2*t), tby );
      toolbar->setVisible(true);
      break;
    case WEST:
      toolbar->setOrientation(Qt::Vertical);
      toolbar->move(x+t,y+t);
      toolbar->resize( tbx, this->height() - (2*t) );
      toolbar->setVisible(true);
      break;
    case EAST:
      toolbar->setOrientation(Qt::Vertical);
      toolbar->move(x+this->width() - (2*t) - tbx,y+t);
      toolbar->resize( tbx, this->height() - (2*t) );
      toolbar->setVisible(true);
      break;	    
    default:
      toolbar->setVisible(false);
  }
}

MSDIR LFrame::getHoverSection(int x, int y){
  //get the frame sizes
  int w = this->width();
  int h = this->height();
  int t = this->frameWidth();
  //Determine which section the cursor is in
  MSDIR section = NONE;
  if( x<=t  ){ //left side of frame
     if( y <= h/3 ){ section = NORTHWEST; }
     else if( y < (2*h)/3 ){ section = WEST; }
     else{ section = SOUTHWEST; }
  }else if( x >=(w-t) ){ //right side of frame
     if( y <= h/3 ){ section = NORTHEAST; }
     else if( y < (2*h)/3 ){ section = EAST; }
     else{ section = SOUTHEAST; }	  
  }else if( y<=t ){ //top of frame
     if( x <= w/3 ){ section = NORTHWEST; }
     else if( x < (2*w)/3 ){ section = NORTH; }
     else{ section = NORTHEAST; }	  
  }else if( y >= (h-t) ){ //bottom of frame
     if( x <= w/3 ){ section = SOUTHWEST; }
     else if( x < (2*w)/3 ){ section = SOUTH; }
     else{ section = SOUTHEAST; }	  
  }
  return section;
}

//=====================
// == PROTECTED FUNCTIONS ==
//=====================
void LFrame::mousePressEvent(QMouseEvent* event){
  //set the flag that it is moving, and what section the mouse was clicked in
  MOVING = true;
  hovSec = getHoverSection(event->x(), event->y()); //set this for access if clicked
  if( (hovSec == NONE) && (QApplication::widgetAt(me->globalX(), me->globalY()) == label_title ){
    hovSec = TITLE;
  }
}

void LFrame::mouseMoveEvent(QMouseEvent* event){
  int x = this->x();
  int y = this->y();
  int w = this->width();
  int h = this->height();
  switch ( hovSec )
   {
      case NORTHWEST:
	int dy = y - event->globalPos().y();
        int dx = x - event->globalPos().x();
        this->move(x-dx, y-dy);
        emit resized(dx, dy);
        break;
      case NORTH:
	int dy = y - event->globalPos().y();
        this->move(x, y-dy); //only move y origin
        emit resized(0,dy); //
        break;
      case NORTHEAST:
	int dy = y - event->globalPos().y();
        int dx = x + w - event->globalPos().x(); // <0 if larger
        this->move(x, y-dy); //only move y origin
        emit resized(-dx, dy);
        break;
      case EAST:
        int dx = x + w - event->globalPos().x(); // <0 if larger
        emit resized(-dx, 0); 
        break;
      case SOUTHEAST:
	int dy = y + h - event->globalPos().y(); // <0 if larger
        int dx = x + w - event->globalPos().x(); // <0 if larger
        emit resized(-dx, -dy);
        break;
      case SOUTH:
	int dy = y + h - event->globalPos().y(); // <0 if larger
        emit resized(0, -dy);
        break;
      case SOUTHWEST:
	int dy = y + h - event->globalPos().y(); // <0 if larger
        int dx = x - event->globalPos().x(); // <0 if larger
        this->move(x-dx, y); //only move x origin
        emit resized(-dx, -dy);
        break;
      case WEST:
        int dx = x + w - event->globalPos().x(); // <0 if larger
        this->move(x-dx, y); //only move x origin
        emit resized(-dx, 0);
        break;
      case TITLE:
	
      default:
	//do nothing
    }
}

void LFrame::mouseReleaseEvent(QMouseEvent* event){
  hovSec = NONE; //reset the current mouse direction
  MOVING=false;
}

bool LFrame::eventFilter(QObject* obj, QEvent* event){
  if(event->type() == QEvent::MouseMove && ){
    QMouseEvent *me = static_cast<QMouseEvent*>(event);
    //Set Mouse icon appropriately
    if( !MOVING ){
      switch ( getHoverSection(me->x(), me->y()) )
     {
      case NORTHWEST:
	this->setCursor(Qt::SizeFDiagCursor); break;
      case NORTH:
	this->setCursor(Qt::SizeVerCursor); break;
      case NORTHEAST:
	this->setCursor(Qt::SizeBDiagCursor); break;
      case EAST:
	this->setCursor(Qt::SizeHorCursor); break;
      case SOUTHEAST:
	this->setCursor(Qt::SizeFDiagCursor); break;
      case SOUTH:
	this->setCursor(Qt::SizeVerCursor); break;
      case SOUTHWEST:
	this->setCursor(Qt::SizeBDiagCursor); break;
      case WEST:
	this->setCursor(Qt::SizeHorCursor); break;
      default:
	this->setCursor(Qt::ArrowCursor);
      }
    }else if( movSec == TITLE){
      this->setCursor(Qt::SizeAllCursor);
    }
  }
  return false; //make sure the event is not stopped here
}

