#include "mytextedit.h"
#include "mainwindow.h"
#include <QKeyEvent>

MyTextEdit::MyTextEdit(QWidget *aParent){
    m_parent = dynamic_cast<MainWindow*>(aParent);
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *aEvent){
    if(m_parent != NULL){
        m_parent->setIsDirty(true);
        m_parent->checkEditRadioButton(true);
    }
}

