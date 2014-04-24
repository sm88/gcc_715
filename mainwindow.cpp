#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPoint>
#include <iostream>
#include <QPen>
#include <vector>
#include <map>
#include <cstdio>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFileDialog>
#include <QMessageBox>
#include "util.h"
#include <QGridLayout>
#include <QListView>
#include <QSizePolicy>
#include <listviewmodel.h>
#include <QTextStream>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>
#include <QString>
#include <QTextCharFormat>
#include <QGraphicsRectItem>
#include <QSplitter>
#include <QList>
#include <QRadioButton>
#include <QButtonGroup>
#include <QEvent>
#include "mygraphicsscene.h"

using namespace std;
using namespace util;

QBrush _brush(Qt::cyan);
QBrush _brush2(Qt::yellow);
QPen _pen(Qt::black);

//actions

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_selectedIndex = -1;
    m_begin = -1;
    m_end = -1;
    m_tempFile = tr("temp.specmd");
    setIsDirty(false);
    setIsSaved(true);
    initComponents();
}

void MainWindow::initComponents(){
    m_graphicsView = new QGraphicsView(this);
    m_scene = new MyGraphicsScence(this);
    m_zoomSlider = new QSlider(Qt::Horizontal,this);
    m_browzeFileBut = new QPushButton("Browze File", this);
    m_parseBut = new QPushButton("Parse File", this);
    m_changeColorBut = new QPushButton("Switch Color", this);
    m_listView = new QListView;
    m_totalPatterns = new QLabel("Total Patterns: ", this);
    m_textEdit = new MyTextEdit(this);
    m_edit = new QRadioButton(QString::fromStdString("Edit"), this);
    m_browse = new QRadioButton(QString::fromStdString("Browse"), this);
    m_new = new QRadioButton(QString::fromStdString("New"), this);
    m_save = new QRadioButton(QString::fromStdString("Save"), this);
    m_buttonGroup = new QButtonGroup(this);

    //set properties
    m_zoomSlider->setFixedWidth(250);
    m_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    m_zoomSlider->setTickPosition(QSlider::TicksBothSides);
    m_textEdit->setDisabled(true);    
    m_textEdit->setMinimumWidth(150);
    m_listView->setMinimumWidth(150);
    m_textEdit->setMinimumHeight(150);
    m_listView->setMinimumHeight(150);
    m_graphicsView->setMinimumWidth(400);

    //configure interactive elements
    m_browse->setDisabled(false);
    m_new->setDisabled(false);
    m_edit->setDisabled(true);
    m_save->setDisabled(true);
    m_browzeFileBut->setDisabled(true);
    m_parseBut->setDisabled(true);
    m_changeColorBut->setDisabled(true);
    m_textEdit->setReadOnly(true);

    //embed stuff
    m_graphicsView->setScene(m_scene);
    m_buttonGroup->addButton(m_edit);
    m_buttonGroup->addButton(m_browse);
    m_buttonGroup->addButton(m_new);
    m_buttonGroup->addButton(m_save);


    //set layout
    QGridLayout* grid = new QGridLayout;
    this->centralWidget()->setLayout(grid);

    grid->addWidget(m_new, 0, 0);
    grid->addWidget(m_save, 0, 1);
    grid->addWidget(m_edit, 0, 2);
    grid->addWidget(m_browse, 0, 3);
    grid->addWidget(m_zoomSlider, 0, 4);
    grid->addWidget(m_browzeFileBut, 0, 5);
    grid->addWidget(m_parseBut, 0, 6);
    grid->addWidget(m_changeColorBut, 0, 7);

    grid->addWidget(m_totalPatterns,2,0,1,3);
    //grid->addWidget(m_selectedPattern,2,1,1,4);

    QSplitter* horiz = new QSplitter(this);
    QSplitter* vert = new QSplitter(Qt::Vertical, this);
    horiz->addWidget(vert);
    horiz->addWidget(m_graphicsView);

    QList<int> list;
    list.push_back(250);
    list.push_back(750);
    horiz->setSizes(list);

    //set left column
    vert->addWidget(m_listView);
    vert->addWidget(m_textEdit);

    list.clear();
    list.push_back(250);
    list.push_back(750);
    vert->setSizes(list);

    grid->addWidget(horiz,4,0,20,-1);

    //set state    
    m_zoomSlider->setValue(50);
    m_zoomSlider->setDisabled(true);
    m_parseBut->setDisabled(true);
    m_changeColorBut->setDisabled(true);        
    _pen.setWidth(2);

    //set actions
    connect(m_browzeFileBut, SIGNAL(clicked()), this, SLOT(on_m_browzeFileBut_clicked()));
    connect(m_parseBut, SIGNAL(clicked()), this, SLOT(on_m_parseBut_clicked()));
    connect(m_changeColorBut, SIGNAL(clicked()), this, SLOT(on_m_changeColorBut_clicked()));
    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(on_m_zoomSlider_valueChanged(int)));
    connect(m_listView, SIGNAL(clicked(QModelIndex)), this, SLOT(on_m_listView_clicked(QModelIndex)));
    connect(m_edit, SIGNAL(clicked(bool)), this, SLOT(on_m_edit_clicked(bool)));
    connect(m_browse, SIGNAL(clicked(bool)), this, SLOT(on_m_browse_clicked(bool)));
    connect(m_new, SIGNAL(clicked(bool)), this, SLOT(on_m_new_clicked(bool)));
    connect(m_save, SIGNAL(clicked(bool)), this, SLOT(on_m_save_clicked(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getWidthOfEllipseVector(vector<BNode*> aVec){
    int max = -1;
    string s;
    for(std::vector<BNode*>::iterator iter = aVec.begin(); iter != aVec.end(); iter++){
        BNode* node = *iter;
        int len = node->isAttributeNode()?node->getClippedText().length():node->getAppropriateText().length();
        cout << node->getAppropriateText() << endl;
        max = max > len ? max : len;
    }

    //find size of largest text in pixels
    for(int i = 0; i < max; i++)
        s += '0';
    QGraphicsTextItem* item = new QGraphicsTextItem;
    item->setPlainText(s.c_str());
    return (item->boundingRect().width() + MainWindow::ELLIPSE_WIDTH)*aVec.size();
}

void MainWindow::hightLightText(){
    QTextDocument* doc= m_textEdit->document();
    QTextBlock block = doc->findBlockByLineNumber(m_pos-1);
    QTextCursor cursor = m_textEdit->textCursor();
    QTextCharFormat textFmt;
    QFont font;
    font.setPixelSize(10);
    font.setFamily(QString::fromStdString("monospace"));

    //clear old selection
    if(m_begin != -1){
        textFmt.setBackground(Qt::white);
        font.setBold(false);
        textFmt.setFont(font);
        cursor.setPosition(m_begin, QTextCursor::MoveAnchor);
        cursor.setPosition(m_end, QTextCursor::KeepAnchor);
        cursor.setCharFormat(textFmt);
    }

    cursor.setPosition(block.position());
    QString str = QString::fromStdString(m_patternName);
    cursor = doc->find(str,cursor);
    //m_textEdit->setTextCursor(cursor);

    //highlight text
    int beginPos = cursor.position();

    str = QString::fromStdString("concrete ");
    int endPos = doc->find(str,cursor).position();

    str = QString::fromStdString("abstract ");
    int endPos2 = doc->find(str,cursor).position();

    if(endPos == -1 && endPos2 == -1){
        cursor.movePosition(QTextCursor::End);
        endPos = cursor.position();
    }
    else if(endPos == -1){
        endPos = endPos2 - 9;   //for abstract/concrete keyword length plus a space
    }

    else if(endPos2 != -1){
        endPos = (endPos > endPos2) ? endPos2 : endPos;
        endPos -= 9;
    }
    else{
        endPos -= 9;    //only endPos had valid value and not at end
    }

    textFmt.setBackground(Qt::yellow);
    font.setBold(true);
    textFmt.setFont(font);
    cursor.setPosition(beginPos-m_patternName.length()-9, QTextCursor::MoveAnchor);
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);
    cursor.setCharFormat(textFmt);
    m_begin = beginPos-m_patternName.length()-9;
    m_end = endPos;
    cursor.setPosition((m_begin+m_end)/2, QTextCursor::MoveAnchor); //so that highlighted portion is visible
    m_textEdit->setTextCursor(cursor);
}

void MainWindow::createComponents(){
    int winWidth = this->width();

    //start making tree
    int x = winWidth/2;
    int y = 10;
    int rX = 0;
    bool isFirst = true;
    int totalWidth = 0;
    //char buffer[100];
    bool isOpEllipse = false;

    delete m_scene;
    m_scene = new MyGraphicsScence(this);
    m_graphicsView->setScene(m_scene);

    hightLightText();

    if(m_selectedIndex != -1){
        QGraphicsTextItem* item = new QGraphicsTextItem();
        item->setPlainText(QString::fromStdString("Selected Pattern: "+(util::NodeMap::m_mapVec[m_selectedIndex])->getName()));
        item->setPos(0,0);
        m_scene->addItem(item);
    }

    for(std::map<int, vector<BNode*> >::iterator m = m_nodeMap.begin(); m != m_nodeMap.end(); m++){
        isFirst = true;
        for(std::vector<BNode*>::iterator v = m->second.begin(); v != m->second.end(); v++){
            BNode* node = *v;
            if(node->isCollapsed()){    //dont draw collapsed stuff
               if(!node->isDoubleClicked()){
                   continue;
               }
            }

            //set text first, will be used for size calculation.
            QGraphicsTextItem* text = new QGraphicsTextItem;
            string appText = node->getAppropriateText();

            if(appText == "")
                continue;

            text->setPlainText(appText.c_str());

            int textHeight = MainWindow::ELLIPSE_HEIGHT;

            if(node->isAttributeNode()){ //constraint stuff
                text->setPlainText(node->getClippedText().c_str());
                isOpEllipse = true;
                //text->setToolTip(appText.c_str());
            }

            int textSize = text->boundingRect().width();

            //draw
            if(m->first == 0){    //root
                node->setX(x);
                node->setY(y);
                node->setWidth(textSize + MainWindow::ELLIPSE_WIDTH);
                rX = x;
            }
            else{
                if(isFirst){
                    totalWidth = getWidthOfEllipseVector(m->second);
                    x = (rX - totalWidth/2);
                    isFirst = false;
                }
                else{
                    x += totalWidth/(m->second.size());
                }

                int pX = (node->getParent())->getX();
                int pY = (node->getParent())->getY();
                node->setX(x);
                node->setY(y);
                node->setWidth(textSize + MainWindow::ELLIPSE_WIDTH);

                //edges
                m_scene->addLine(x + node->getWidth()/2, y, pX + (node->getParent())->getWidth()/2, pY + MainWindow::ELLIPSE_HEIGHT, _pen);
            }

            if(isOpEllipse){    //attribute rect
                QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, node->getWidth(), textHeight);
                rect->setBrush(_brush2);
                rect->setPen(_pen);

                rect->setToolTip(appText.c_str());
                m_scene->addItem(rect);
                isOpEllipse = false;
            }

            else{
                QGraphicsEllipseItem* item = m_scene->addEllipse(x, y, node->getWidth(), textHeight, _pen, _brush);
                QPointF point = item->mapToScene(x,y);
                //cerr << point.x() << "\tX\t" << point.y() << endl;
                node->setSX(point.x());
                node->setSY(point.y());
                node->setHeight(textHeight);

                if(node->isCollapsed() && node->isDoubleClicked()){
                    //double ellipse
                    m_scene->addEllipse(x-3, y-3, node->getWidth() + 6, textHeight + 6, _pen);
                }
            }

            //print the text in nodes
            text->setPos(x + 20, y + 10);
            m_scene->addItem(text);
        }
        y += 100;
    }
}

void MainWindow::on_m_parseBut_clicked()
{
    m_scene->clear();

    if(isDirty()){
        m_selectedFile = m_tempFile;
        QFile file(m_selectedFile);
        file.open(QFile::WriteOnly|QFile::Text);
        QTextStream out(&file);
        out << m_textEdit->document()->toPlainText();
        out.flush();
        file.close();
    }

    m_listView->setModel(NULL); //clear the list
    m_textEdit->document()->clear();
    m_begin = -1;
    m_end = -1;

    char buffer[1000];
    char *args[20];
    args[0] = strdup("srtl");
    args[1] = strdup("-s");
    args[2] = strdup("-o");
    args[3] = strdup("out.txt");
    args[4] = strdup(m_selectedFile.toStdString().c_str());
    util::ThreadedParser obj(5, args);
    obj.start();
    obj.wait();

    m_parseBut->setDisabled(false);
    ListViewModel* model = new ListViewModel(util::NodeMap::m_mapVec, this);
    m_listView->setModel(model);

    sprintf(buffer, "Total Patterns: %d", model->getVectorSize());
    m_totalPatterns->setText(buffer);

    QFile qfile(m_selectedFile);
    if(!qfile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Log Reader"),
                             tr("Cannot read file %1:\n%2")
                             .arg(m_selectedFile)
                             .arg(qfile.errorString()));
        return;
    }
    QTextStream in(&qfile);
    m_textEdit->setPlainText(in.readAll());
    QFont font;
    font.setPixelSize(10);
    font.setFamily(QString::fromStdString("monospace"));
    m_textEdit->setFont(font);
    qfile.close();
    m_edit->setDisabled(false);
    m_textEdit->setDisabled(false);
    m_textEdit->setReadOnly(true);
    m_browse->setChecked(true);
    m_changeColorBut->setDisabled(false);
}

void MainWindow::on_m_changeColorBut_clicked()
{
    QColor col[] = {Qt::gray, Qt::yellow, Qt::green, Qt::cyan};
    QColor col2[] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::darkRed};
    _pen.setColor(col2[rand()%4]);    
    _brush.setColor(col[rand()%4]);
    _brush2.setColor(col[rand()%4]);
    while (_brush.color() == _brush2.color()) {
        _brush.setColor(col[rand()%4]);
        _brush2.setColor(col[rand()%4]);
    }
    createComponents();
}

void MainWindow::cleanUI(bool aReadOnlyText){
    m_listView->setModel(NULL);
    m_scene->clear();

    m_textEdit->document()->clear();
    m_textEdit->setDisabled(false);
    m_textEdit->setReadOnly(aReadOnlyText);

    m_save->setDisabled(false);
    m_parseBut->setDisabled(false);
    update();

    setIsDirty(false);
}

void MainWindow::on_m_browzeFileBut_clicked()
{
    save();
    if(isSaved())
        cleanUI(true);

    QString file = QFileDialog::getOpenFileName(m_browzeFileBut,
                                                QString::fromStdString("Find specmd file"),
                                                QString::fromStdString("/home/"),
                                                tr("Specmd (*.specmd);; All Files (*.*)"));
    //trigger parser
    std::string str = file.toUtf8().constData();
    if(str.c_str() == NULL || str == "")
        return;
    m_selectedFile = tr(str.c_str());
    m_parseBut->setDisabled(false);
}

void MainWindow::on_m_listView_clicked(QModelIndex index){
    ListViewModel* model = static_cast<ListViewModel*>(m_listView->model());

    PatternDTO* dto = (model->getSelectedModelObject(index.row()));
    m_nodeMap = dto->getMap();
    m_pos = dto->getLineStart();
    m_patternName = dto->getName();
    m_selectedIndex = index.row();
    m_zoomSlider->setDisabled(false);

    createComponents();
}

void MainWindow::showMessage(string aMsg){
    QMessageBox msgBox;
    msgBox.setText(aMsg.c_str());
    msgBox.exec();
}

void MainWindow::on_m_zoomSlider_valueChanged(int value)
{    
    double val = (value - 50)/5;
    if(val < 0)
        m_graphicsView->setTransform(QTransform::fromScale(-1/val,-1/val));
    else
        m_graphicsView->setTransform(QTransform::fromScale(1+val,1+val));
}

void MainWindow::on_m_edit_clicked(bool isClicked){
    if(isClicked){
        m_textEdit->setDisabled(false);
        m_textEdit->setReadOnly(false);
        m_save->setDisabled(false);
    }
}

void MainWindow::on_m_browse_clicked(bool isClicked){
    if(isClicked){
        save();
        m_browzeFileBut->setDisabled(false);
        m_textEdit->setReadOnly(true);
    }
}

void MainWindow::save(){
    if(isDirty()){
        int reply;
        reply = QMessageBox::question(m_new,"Unsaved","Unsaved contents detected, do you want to save?",QMessageBox::Yes, QMessageBox::No);
        if(reply == QMessageBox::Yes)
            on_m_save_clicked(true);
        else
            setIsSaved(true);   //user does not want to save
    }
}

void MainWindow::on_m_new_clicked(bool isClicked){
    if(isClicked){
        save();
        if(isSaved())
            cleanUI(false);
    }
}

void MainWindow::nothingToSaveMessage(){
    if(m_textEdit->document()->toPlainText() == NULL || m_textEdit->document()->toPlainText() == ""){
        QMessageBox::warning(m_save,tr("Nothing to save"),tr("No text found!"));
        setIsDirty(false);
        return;
    }
}

void MainWindow::on_m_save_clicked(bool isClicked){
    if (isClicked){
        nothingToSaveMessage();

        if(isDirty()){
            QString file = QFileDialog::getSaveFileName(m_save,
                                                        QString::fromStdString("Save specmd file"),
                                                        QString::fromStdString("/home/"),
                                                        tr("Specmd (*.specmd);; All Files (*.*)"));

            std::string str = file.toUtf8().constData();
            if(str.c_str() == NULL || str == ""){
                setIsSaved(false);   //user clicked cancel or closed
                return;
            }
            m_selectedFile = tr(str.c_str());
            m_parseBut->setDisabled(false);

            QFile fd(file);
            fd.open(QFile::WriteOnly|QFile::Text);
            QTextStream out(&fd);
            out << m_textEdit->document()->toPlainText();
            out.flush();
            fd.close();

            //info
            QMessageBox::information(m_save,tr("Saved"),tr("Successfully saved!"));
            m_textEdit->setReadOnly(true);
            m_browse->setChecked(true);
            setIsDirty(false);
            setIsSaved(true);
        }
    }
}

void MainWindow::setIsDirty(bool aDirty){
    m_isDirty = aDirty;
}

bool MainWindow::isDirty(){
    return m_isDirty;
}

void MainWindow::setIsSaved(bool aSaved){
    m_isSaved = aSaved;
}

bool MainWindow::isSaved(){
    return m_isSaved;
}

void MainWindow::mutateBranch(std::map<int,vector<BNode*> >::iterator aM, std::vector<BNode*>::iterator aV, bool aIsCollapsed){
    (*aV)->setIsCollapsed(!aIsCollapsed);    //set the clicked node
    (*aV)->setIsDoubleClicked(true);

    //propagate
    for(std::map<int,vector<BNode*> >::iterator m = ++aM; m != m_nodeMap.end(); m++){
        for(std::vector<BNode*>::iterator v = (m->second).begin(); v != (m->second).end(); v++){
            BNode* node = *v;
            if(node != NULL && node->getAppropriateText() != "" && node->getParent() != NULL){
                node->setIsCollapsed(node->getParent()->isCollapsed());
                node->setIsDoubleClicked(false);
            }
        }
    }

    createComponents();
}

bool MainWindow::isItemAtScenePos(BNode* aNode, QPointF aPoint){
    int xStart = aNode->getSX();
    int xEnd = aNode->getSX() + aNode->getWidth();
    int yStart = aNode->getSY();
    int yEnd = aNode->getSY() + aNode->getHeight();

    if(aPoint.x() >= xStart && aPoint.x() <= xEnd){
        if(aPoint.y() >= yStart && aPoint.y() <= yEnd)
            return true;
    }

    return false;
}

void MainWindow::performMutation(QPointF aPoint){
    for(std::map<int,std::vector<BNode*> >::iterator i = m_nodeMap.begin(); i != m_nodeMap.end(); i++){
        for(std::vector<BNode*>::iterator i2 = (i->second).begin(); i2 != (i->second).end(); i2++){
            BNode* node = (*i2);
            if(node != NULL && isItemAtScenePos(node, aPoint)){
                mutateBranch(i, i2, node->isCollapsed());
                return;
            }
        }
    }
}

