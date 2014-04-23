#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"
#include <QPointF>

//forward decl
class MainWindow;

class MyGraphicsScence:public QGraphicsScene{
private:
    MainWindow* m_parent;

public:
    MyGraphicsScence(QWidget* aParent);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYGRAPHICSSCENE_H
