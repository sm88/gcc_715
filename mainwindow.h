#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <vector>
#include "bnode.h"
#include "srtl-driver.hh"
#include <QPushButton>
#include <QSlider>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QListView>
#include <QModelIndex>
#include <QLabel>
#include "mytextedit.h"
#include <QRadioButton>
#include "mygraphicsscene.h"
#include "util.h"
#include "pattern_dto.h"

class MyTextEdit;
class MyGraphicsScence;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setNodeMap(std::map<int, std::vector<BNode*> > aMap);
    static const int ELLIPSE_HEIGHT = 40;
    static const int ELLIPSE_WIDTH = 40;
    srtl_driver* m_driver;
    void setIsDirty(bool aDirty);
    bool isDirty();
    void setIsSaved(bool aSaved);
    bool isSaved();    
    void performMutation(QPointF aPoint);
    PatternDTO* getSelectedIndexMap(){
        return util::NodeMap::m_mapVec[m_selectedIndex];
    }

    std::map<int, std::vector<BNode*> >& getMap(){
        return m_nodeMap;
    }
    void checkEditRadioButton(bool aCheked){
        m_edit->setChecked(aCheked);
    }

    void mutateBranch(std::map<int,vector<BNode*> >::iterator aM, std::vector<BNode*>::iterator aV, bool aIsCollapsed);

private slots:
    void on_m_browzeFileBut_clicked();

    void on_m_parseBut_clicked();

    void on_m_changeColorBut_clicked();

    void on_m_zoomSlider_valueChanged(int value);

    void on_m_listView_clicked(QModelIndex index);

    void on_m_edit_clicked(bool isClicked);

    void on_m_browse_clicked(bool isClicked);

    void on_m_new_clicked(bool isClicked);

    void on_m_save_clicked(bool isClicked);

private:    
    void createComponents();    
    int getWidthOfEllipseVector(vector<BNode*> aVec);
    void initComponents();
    void showMessage(string aMsg);
    void hightLightText();
    void save();
    bool isItemAtScenePos(BNode* aNode, QPointF aPoint);
    void cleanUI(bool aReadOnlyText);
    void nothingToSaveMessage();

    Ui::MainWindow *ui;
    MyGraphicsScence *m_scene;
    QGraphicsView* m_graphicsView;
    QSlider* m_zoomSlider;
    QPushButton* m_browzeFileBut;
    QPushButton* m_parseBut;
    QPushButton* m_changeColorBut;
    QListView* m_listView;
    QLabel* m_totalPatterns;
    QLabel* m_selectedPattern;
    MyTextEdit* m_textEdit;
    QRadioButton* m_edit;
    QRadioButton* m_browse;
    QRadioButton* m_new;
    QRadioButton* m_save;
    QButtonGroup* m_buttonGroup;
    int m_selectedIndex;
    std::map<int, std::vector<BNode*> > m_nodeMap;
    int m_pos;
    char* m_file;
    string m_patternName;
    int m_begin;
    int m_end;
    QString m_selectedFile;
    QString m_tempFile;
    bool m_isDirty;
    bool m_isSaved;
};

#endif // MAINWINDOW_H
