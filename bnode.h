#ifndef BNode_H__
#define BNode_H__
#include "node.hh"

class BNode
{
private:
    Node* m_node;
    int m_x;
    int m_y;
    int m_uid;
    int m_width;
    int m_height;
    bool m_isCollapsed;
    bool m_isMode;
    bool m_isConstraint;
    bool m_isPredicate;
    bool m_isType;
    std::string m_mode;
    std::string m_constraint;
    std::string m_predicate;
    std::string m_type;
    BNode* m_parent;
    int m_sX;
    int m_sY;
    bool m_isDoubleClicked;

    void init(Node* aNode);    

public:
    std::vector<BNode*> children;
    BNode(Node* aNode);

    BNode();

    Node* getNode();    

    int getX();
    void setX(int aX);

    int getY();
    void setY(int aY);

    int getSX();
    void setSX(int aX);

    int getSY();
    void setSY(int aY);

    int getId();
    void setId(int aId);

    int getWidth();
    void setWidth(int aWidth);

    int getHeight();
    void setHeight(int aHeight);

    bool isCollapsed();
    void setIsCollapsed(bool aIsCollapsed);

    bool isDoubleClicked();
    void setIsDoubleClicked(bool aIsDoubleClicked);

    bool isMode();
    void setIsMode(bool aIsMode);

    bool isConstraint();
    void setIsConstraint(bool aIsConstraint);

    bool isPredicate();
    void setIsPredicate(bool aIsPredicate);

    bool isType();
    void setIsType(bool aIsType);

    std::string getMode();
    void setMode(std::string aMode);

    std::string getConstraint();
    void setConstraint(std::string aConstraint);

    std::string getPredicate();
    void setPredicate(std::string aPredicate);

    std::string getType();
    void setType(std::string aType);

    BNode* getParent();
    void setParent(BNode* aParent);

    std::string getAppropriateText();

    std::string getClippedText();

    bool isAttributeNode();

    ~BNode();
};
#endif
