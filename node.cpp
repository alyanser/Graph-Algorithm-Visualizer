#include <QGraphicsScene>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsView>
#include <QPainter>
#include "node.hpp"

Node::Node(int row,int col,QGraphicsItem * parent) : QGraphicsObject(parent), type(Inactive), gridX(row), gridY(col){
         pathParent = nullptr;
         setAcceptDrops(true);
         setAcceptHoverEvents(true);
         setGraphicsItem(this);
}

void Node::setRunningState(const bool & newState){
         on = newState;
}

QRectF Node::boundingRect() const{
         return QRectF(0,0,24,23);
}

void Node::paint(QPainter * painter,const QStyleOptionGraphicsItem * option,QWidget * widget){
         Q_UNUSED(option);
         Q_UNUSED(widget);

         QBrush brush;
         brush.setStyle(Qt::SolidPattern);

         switch(type){
                  case Source : brush.setColor(Qt::green);break;
                  case Target : brush.setColor(Qt::black);break;
                  case Active : brush.setColor(Qt::white);break;
                  case Inactive : brush.setColor(Qt::blue);break;
                  case Visited : brush.setColor(Qt::yellow);break;
                  case Block : brush.setColor(Qt::gray);break;
                  case Inpath : brush.setColor(Qt::red);break;
                  default : assert(false);
         }
         painter->setBrush(brush);
         painter->setRenderHint(QPainter::Antialiasing);
         painter->drawEllipse(0,0,24,23);
}

QSizeF Node::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const{
         Q_UNUSED(which);
         Q_UNUSED(constraint);

         return QSizeF(24,23);
}

void Node::setGeometry(const QRectF & geometry){
         prepareGeometryChange();
         QGraphicsLayoutItem::setGeometry(geometry);
         setPos(geometry.topLeft());
}

void Node::setType(const State & newType){
         type = newType;
         bool dropValue = true;

         if(type == Source){
                  dropValue = false;
                  emit sourceSet();
         }else if(type == Target){
                  dropValue = false;
                  emit targetSet();
         }

         setAcceptDrops(dropValue);
         update();
}

void Node::setPathParent(Node * newParent){
         pathParent = newParent;
}

std::pair<int,int> Node::getCord() const{
         return {gridX,gridY};
}

Node * Node::getPathParent() const{
         return pathParent;
}

Node::State Node::getType() const{
         return type;
}

void Node::dragEnterEvent(QGraphicsSceneDragDropEvent * event){
         auto mimeData = event->mimeData();

         if(mimeData->hasText()){
                  const QString & text = mimeData->text();

                  if(text == "inverter"){
                           switch(type){
                                    case Source | Target : break;
                                    case Block : setType(Inactive);event->accept();break;
                                    default : setType(Block);event->accept();
                           }
                  }
         }
         QGraphicsObject::dragEnterEvent(event);
}

void Node::dropEvent(QGraphicsSceneDragDropEvent * event){
         auto mimeData = event->mimeData();
         if(mimeData->hasText()){
                  const QString & indicator = mimeData->text();

                  if(indicator == "fromSource"){
                           setType(Source);
                           event->accept();
                  }else if(indicator == "fromTarget"){
                           setType(Target);
                           event->accept();

                  }
         }
         QGraphicsItem::dropEvent(event);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent * event){
         if(on){ // there is an algorithm currently in running state
                  return;
         }
         auto dragger = new QDrag(this);
         auto mimeData = new QMimeData();
         dragger->setMimeData(mimeData);
         
         switch(type){
                  case Source : {
                           mimeData->setText("fromSource");
                           if(dragger->exec()){
                                    setType(Inactive);
                           }
                           break;
                  }
                  case Target : {
                           mimeData->setText("fromTarget");
                           if(dragger->exec()){
                                    setType(Inactive);
                           }
                           break;
                  }
                  default  : {
                           mimeData->setText("inverter");
                           dragger->exec();
                  }
         }
         QGraphicsItem::mousePressEvent(event);
}