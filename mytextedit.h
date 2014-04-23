#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H
#include <QTextEdit>
#include <QWidget>
#include "mainwindow.h"
class MainWindow;

class MyTextEdit:public QTextEdit{
private:
    MainWindow* m_parent;
public:
    MyTextEdit(QWidget *aParent);
    void keyReleaseEvent(QKeyEvent* aEvent);
};

#endif // MYTEXTEDIT_H
