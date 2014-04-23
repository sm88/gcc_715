#include "util.h"
#include "srtl-driver.hh"
#include <cstring>
#include <iostream>
#include "node.hh"
#include "bnode.h"
#include <map>
#include <vector>
#include <ctime>
#include <pattern.hh>

srtl_driver* util::getDriver(int argc, char *argv[]){
    bool displayStatsFlag = false;
    srtl_driver* driver = new srtl_driver();

    driver->setLogLevel (srtl_driver::err);

    if (argc == 1) {
        std::cerr << "Format: srtl [-s] [-o outputFileName] inputFilename" << std::endl;
        return NULL;
    }

    if (!strcmp(argv[1],"-s")) {
        if (argc == 3 || argc == 5) {
            driver->initializeDotFile ();
            if (argc == 3) {
                string s (argv[2]);
                s.append (".md");
                driver->setOutFileName (&s);
                driver->initializeOutFile ();
                if(driver->parse (argv[2]))
                    std::cout << driver->result <<std::endl;
            } else {
                driver->setOutFileName (new string (argv[3]));
                driver->initializeOutFile ();
                if(driver->parse (argv[4]))
                    std::cout << driver->result <<std::endl;
            }
            displayStatsFlag = true;
        } else {
            std::cerr << "Invalid file name." << std::endl;
        }
    } else if (!strcmp (argv[1], "-o")) {
        if (argc == 4) {
            string s (argv[2]);
            driver->setOutFileName (&s);
            driver->initializeOutFile ();
                if(driver->parse (argv[3]))
                    std::cout << driver->result <<std::endl;
        } else {
            std::cerr << "Invalid Flags." << std::endl;
            std::cerr << "Format: srtl [-s] [-o outputFileName] inputFilename" << std::endl;
        }
    } else {
        string s (argv[1]);
        s.append (".md");
        driver->setOutFileName (&s);
        std::cout << "Invalid file name :" << argv[1] << std::endl;
        driver->initializeOutFile ();
        if(driver->parse (argv[1]))
            std::cout << driver->result <<std::endl;
    }

    /*
     * Calculate the statistics
     */
    if (displayStatsFlag)
        driver->calculateAndDisplayStats ();

    std::cout << std::endl;
    return driver;
}

//initialize static memeber vars
std::map<int, std::vector<BNode*> > util::NodeMap::m_nodeMap;
std::vector<PatternDTO*> util::NodeMap::m_mapVec;
std::deque<Operand> util::NodeMap::m_ops;
int dfsFlag = 0;

util::ThreadedParser::ThreadedParser(int aArgNum, char** aArgs){
    Node::setCounter(0);
    util::NodeMap::initData();
    m_argc = aArgNum;
    for(int i=0; i<aArgNum; i++){
        m_args[i] = new char[30];
        strcpy(m_args[i],aArgs[i]);
    }
}

void util::ThreadedParser::run(){
    srtl_driver* driver = util::getDriver(m_argc, m_args);
    std::map<std::string, Pattern*> tab = driver->getSymTab();
    for(std::map<std::string, Pattern*>::iterator iter = tab.begin(); iter != tab.end(); iter++){
        ConcretePattern* p = dynamic_cast<ConcretePattern*>(tab[iter->first]);
        AbstPattern* ap = dynamic_cast<AbstPattern*>(tab[iter->first]);
        if(p != NULL){
            Node* tree = p->getTree();
            p->trav(tree);
            cerr << p->getName() << " " << p->getLineStart() << " " << endl;//p->getLineEnd() << endl;
            util::NodeMap::display(tree);

            std::vector<Operand> opVec = p->getOperands();
            std::deque<Operand> deck(opVec.begin(), opVec.end());
            util::NodeMap::setOpDeque(deck);
            util::NodeMap::createNodeMap(tree);
            std::map<int, std::vector<BNode*> > nodeMap = util::NodeMap::getNodeMap();
            std::map<int, std::vector<BNode*> > tempMap(nodeMap.begin(), nodeMap.end());    //copy the map

            PatternDTO* dto = new PatternDTO();
            dto->setMap(tempMap);
            dto->setName(p->getName());
            dto->setLineStart(p->getLineStart());
            util::NodeMap::m_mapVec.push_back(dto);
            dfsFlag = 0;
            util::NodeMap::doDepthFirstSearch(tree);
        }
        if(ap!=NULL && ap->resolved){
            Node* tree = ap->getTree();
            //cerr << "++++++"<<ap->getPatName() << " " << ap->getLineStart() << " " << endl;//p->getLineEnd() << endl;
            util::NodeMap::display(tree);
            util::NodeMap::createNodeMap(tree);
            std::map<int, std::vector<BNode*> > nodeMap = util::NodeMap::getNodeMap();
            std::map<int, std::vector<BNode*> > tempMap(nodeMap.begin(), nodeMap.end());    //copy the map

            PatternDTO* dto = new PatternDTO();
            dto->setMap(tempMap);
            dto->setName(ap->getPatName());
            dto->setLineStart(ap->getLineStart());
            util::NodeMap::m_mapVec.push_back(dto);
        }
    }
}

void util::NodeMap::initData(){
    util::NodeMap::m_nodeMap.clear();
    util::NodeMap::m_mapVec.clear();
    util::NodeMap::m_ops.clear();
}

void util::NodeMap::doDepthFirstSearch(Node* aNode){
    if(m_ops.size() == 0)
        return;

    if(aNode->isLeafNode()){
        Operand op = m_ops.front();
        m_ops.pop_front();
        while(op.getType() == Operand::bracketOpen){
            if(m_ops.size() == 0)
                return ;
            op = m_ops.front();
            m_ops.pop_front();
        }

        if(op.getType() == Operand::bracketClose){
            dfsFlag = 1;
            return;
        }

        BNode* b = aNode->container;
        setOpAttrributes(op,b);
    }

    std::vector<Node*> vec = aNode->getChildren();
    for(std::vector<Node*>::iterator iter = vec.begin(); iter != vec.end(); iter++){
        if((*iter) != NULL && strcmp((*iter)->getName().c_str(), "___")){
            doDepthFirstSearch(*iter);
            if(dfsFlag == 1 && !(aNode->getName() == "match_operator" || aNode->getName() == "unspec" ||
                    aNode->getName() == "parallel" || aNode->getName() == "unspec_volatile" ||
                    aNode->getName() == "match_op_dup" || aNode->getName() == "match_operator" || aNode->getName() == "parallel" ||
                    aNode->getName() == "match_op_dup" || aNode->getName() == "sequence")){
                        return;
            }
            else{
                dfsFlag = 0;
            }
        }
    }
}

std::map<int, std::vector<BNode*> > util::NodeMap::getNodeMap(){
    return m_nodeMap;
}

void util::NodeMap::setOpDeque(std::deque<Operand> aDeque){
    m_ops = aDeque;
}

void util::NodeMap::createNodeMap(Node* aRoot){
    m_nodeMap.clear();
    if(aRoot != NULL){
        createNodeMap(0, aRoot);
    }
}

void util::NodeMap::createNodeMap(int aLevel, Node* aNode){
    if(aLevel == 0 && m_nodeMap.size() == 0){    //handle root
        if(aNode != NULL){
            BNode* newNode = new BNode(aNode);
            aNode->container = newNode;
            std::vector<BNode*> vec;
            vec.push_back(newNode);
            m_nodeMap[aLevel] = vec;
        }
    }
    else{
        std::vector<BNode*> vec;
        for(std::vector<BNode*>::iterator iter = m_nodeMap[aLevel].begin(); iter != m_nodeMap[aLevel].end(); iter++){
            if((*iter)->getNode() != NULL){
                std::vector<Node*> children = (*iter)->getNode()->getChildren();
                for(std::vector<Node*>::iterator iter2 = children.begin(); iter2 != children.end(); iter2++){
                    if((*iter2) != NULL && strcmp((*iter2)->getName().c_str(), "___")){
                        BNode* newNode = new BNode(*iter2);
                        (*iter2)->container = newNode;
                        newNode->setParent(*iter);
                        vec.push_back(newNode);
                    }
                }
            }
        }
        aLevel++;
        m_nodeMap[aLevel] = vec;
    }
    if(!isTreeEnd(m_nodeMap[aLevel]))
        createNodeMap(aLevel, aNode);
}

bool util::NodeMap::isTreeEnd(std::vector<BNode*> &aVec){
    for(std::vector<BNode*>::iterator iter = aVec.begin(); iter != aVec.end(); iter++){
        if((*iter)->getNode() != NULL && ((*iter)->getNode())->getChildren().size() != 0)
            return false;
    }
    return true;
}

void util::NodeMap::display(Node *aRoot){
    if(aRoot->isLeafNode()){
        return;
    }
    std::vector<Node*> children = aRoot->getChildren();
    for(std::vector<Node*>::iterator it = children.begin(); it != children.end(); it++){
        //if((*it)->getName() != "___")
         //   cerr << (*it)->getId() << "\t" << (*it)->getParent()->getId() << endl;
        display(*it);
    }
    cerr << endl;
}

void util::NodeMap::setOpAttrributes(Operand op, BNode* newNode){
    //constraint
    if(op.getConstraint() != ""){
        //BNode* newNode = new BNode();
        newNode->setConstraint("Cons::"+op.getConstraint());
        newNode->setIsConstraint(true);
        //aVec.push_back(newNode);
    }

    //predicate
    if(op.getPredicate() != ""){
        //BNode* newNode = new BNode();
        newNode->setPredicate("Pred::"+op.getPredicate());
        newNode->setIsPredicate(true);
        //aVec.push_back(newNode);
    }

    //mode
    if(op.getMode() != ""){
        //BNode* newNode = new BNode();
        newNode->setMode("Mode::"+op.getMode());
        newNode->setIsMode(true);
        //aVec.push_back(newNode);
    }

    //type
    string s = "";
    //BNode* newNode = new BNode();
    switch(op.getType()){
        case Operand::pmc:
            s += "pmc";
            break;

        case Operand::dup:
            s += "dup";
            break;

        case Operand::num:
            s += "num";
            break;

        case Operand::reg:
            s += "reg";
            break;

        case Operand::regNum:
            s += "regNum";
            break;

        case Operand::extraReg:
            s += "extraReg";
            break;

        case Operand::constSpec:
            s += "constSpec";
            break;

        case Operand::scratch:
            s += "scratch";
            break;

        case Operand::eqAttr:
            s += "eqAttr";
            break;

        case Operand::matchOperator:
            s += "matchOperator";
            break;

        case Operand::fixedReg:
            s += "fixedReg";
            break;

        case Operand::integer:
            s += "integer";
            break;

        case Operand::string:
            s += "string";
            break;

        case Operand::bracketOpen:
            s += "bracketOpen";
            break;

        case Operand::bracketClose:
            s += "bracketClose";
            break;
    }

    if(s != ""){
        s = "Type::" + s;
        newNode->setIsType(true);
        newNode->setType(s);
    }
}
