#include "mygraphicsscene.h"
#include "mainwindow.h"
#include <QGraphicsItem>
#include "util.h"
#include "pattern_dto.h"
#include "bnode.h"
#include <QPointF>

MyGraphicsScence::MyGraphicsScence(QWidget* aParent):QGraphicsScene(aParent){
    m_parent = dynamic_cast<MainWindow*>(aParent);
}

void MyGraphicsScence::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *aEvent){
    //cerr << "Double Clicked" << aEvent->scenePos().x() << " " << aEvent->scenePos().y();
    m_parent->performMutation(aEvent->scenePos());
    QGraphicsScene::mouseDoubleClickEvent(aEvent);
}
