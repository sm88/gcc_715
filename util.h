#ifndef UTIL_H
#define UTIL_H
#include "srtl-driver.hh"
#include <map>
#include <vector>
#include "bnode.h"
#include "node.hh"
#include "pattern_dto.h"
#include <QThread>

namespace util {
    srtl_driver* getDriver(int argc, char* argv[]);

    class ThreadedParser:public QThread{
    private:
        char* m_args[20];
        int m_argc;
    public:
        ThreadedParser(int aArgNum, char** aArgs);
        void run();
    };

    class NodeMap{
    private:
        static void createNodeMap(int aLevel, Node* aNode);
        static bool isTreeEnd(std::vector<BNode*> &aVec);
        static std::deque<Operand> m_ops;
        static std::map<int, std::vector<BNode*> > m_nodeMap;
        static void setOpAttrributes(Operand aOp, std::vector<BNode*> &aVec, BNode* aNode);
        static void setOpAttrributes(Operand op, BNode* aNode);

    public:
        static void doDepthFirstSearch(Node* aNode);
        static std::map<int, std::vector<BNode*> > getNodeMap();
        static void setOpDeque(std::deque<Operand> aDeque);
        static void createNodeMap(Node* aRoot);
        static std::vector<PatternDTO*> m_mapVec;
        static void display(Node* aNode);
        static void initData();
    };
}
#endif // UTIL_H
