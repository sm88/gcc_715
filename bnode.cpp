#include <iostream>
#include "bnode.h"
#include <ctime>
#include <cstdlib>
#include <sstream>

BNode::BNode(Node* aNode){
    init(aNode);
}

BNode::BNode(){
    init(NULL);
}

void BNode::init(Node *aNode){
    m_node = aNode;
    m_uid = clock();
    m_parent = NULL;
    m_constraint = "";
    m_predicate = "";
    m_mode = "";
    m_isCollapsed = false;
    m_isConstraint = false;
    m_isMode = false;
    m_isPredicate = false;
    m_isDoubleClicked = false;
}

Node* BNode::getNode(){
    return m_node;
}

int BNode::getX(){
    return m_x;
}

void BNode::setX(int aX){
    m_x = aX;
}

int BNode::getY(){
    return m_y;
}

void BNode::setY(int aY){
    m_y = aY;
}

int BNode::getSX(){
    return m_sX;
}

void BNode::setSX(int aX){
    m_sX = aX;
}

int BNode::getSY(){
    return m_sY;
}

void BNode::setSY(int aY){
    m_sY = aY;
}

int BNode::getId(){
    return m_uid;
}

void BNode::setId(int aId){
    m_uid = aId;
}

int BNode::getWidth(){
    return m_width;
}

void BNode::setWidth(int aWidth){
    m_width = aWidth;
}

int BNode::getHeight(){
    return m_height;
}

void BNode::setHeight(int aHeight){
    m_height = aHeight;
}

bool BNode::isCollapsed(){
    return m_isCollapsed;
}

void BNode::setIsCollapsed(bool aIsCollapsed){
    m_isCollapsed = aIsCollapsed;
}

bool BNode::isDoubleClicked(){
    return m_isDoubleClicked;
}

void BNode::setIsDoubleClicked(bool aIsDoubleClicked){
    m_isDoubleClicked = aIsDoubleClicked;
}

BNode* BNode::getParent(){
    return m_parent;
}

bool BNode::isMode(){
    return m_isMode;
}

void BNode::setIsMode(bool aIsMode){
    m_isMode = aIsMode;
}

bool BNode::isConstraint(){
    return m_isConstraint;
}

void BNode::setIsConstraint(bool aIsConstraint){
    m_isConstraint = aIsConstraint;
}

bool BNode::isPredicate(){
    return m_isPredicate;
}

void BNode::setIsPredicate(bool aIsPredicate){
    m_isPredicate = aIsPredicate;
}

bool BNode::isType(){
    return m_isType;
}

void BNode::setIsType(bool aIsType){
    m_isType = aIsType;
}

std::string BNode::getMode(){
    return m_mode;
}

void BNode::setMode(std::string aMode){
    m_mode = aMode;
}

std::string BNode::getConstraint(){
    return m_constraint;
}

void BNode::setConstraint(std::string aConstraint){
    m_constraint = aConstraint;
}

std::string BNode::getPredicate(){
    return m_predicate;
}

void BNode::setPredicate(std::string aPredicate){
    m_predicate = aPredicate;
}

std::string BNode::getAppropriateText(){
    std::string s = "";
    std::ostringstream sstream;
    Node* innerNode=getNode();
    if(innerNode->isLeafNode()){
        if(isMode())
            s += getMode() + "\n";
        if(isConstraint())
            s += getConstraint() + "\n";
        if(isPredicate())
            s += getPredicate() + "\n";
        if(isType())
            s += getType();        
        //sstream << s << " " << getId();
        return s;//sstream.str();
    }
    else{
        sstream << getNode()->getName(); //<< " " << m_node->getId();
        return sstream.str();
    }
}

std::string BNode::getType(){
    return m_type;
}

void BNode::setType(std::string aType){
    m_type = aType;
}

void BNode::setParent(BNode* aParent){
    m_parent = aParent;
}

std::string BNode::getClippedText(){
    std::string s = getAppropriateText();
    if(isAttributeNode()){
        return s.substr(0,s.find("::")+3)+"...";
    }
    return s;
}

bool BNode::isAttributeNode(){
    std::string s = getAppropriateText();
    if(s.find("::") != std::string::npos)
        return true;
    return false;
}

BNode::~BNode(){
    delete m_node;
}
