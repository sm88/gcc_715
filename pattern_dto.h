#ifndef PATTERN_DTO_H
#define PATTERN_DTO_H
#include <map>
#include <vector>
#include "bnode.h"

class PatternDTO{
private:
    std::map<int, std::vector<BNode*> > m_map;
    int m_lineStart;
    std::string m_name;

public:
    std::map<int, std::vector<BNode*> > getMap(){
        return m_map;
    }

    void setMap(std::map<int, std::vector<BNode*> > map){
        m_map = map;
    }

    int getLineStart(){
        return m_lineStart;
    }

    void setLineStart(int aLineNo){
        m_lineStart = aLineNo;
    }

    std::string getName(){
        return m_name;
    }

    void setName(std::string aName){
        m_name = aName;
    }
};

#endif // PATTERN_DTO_H
