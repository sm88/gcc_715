/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class header definitions for list pattern class and     *
 * supporting class.                                                          *
 *                                                                            *
 *                                                                            *
 * Change History                                                             *
 * -------------------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Sheweta     14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */
/*--------------------------------------------------------------------------- *
 * This class stores the lists of elements. It supports methods to append and *
 * display the final string of elements.                                      *
 *--------------------------------------------------------------------------- */ 

#ifndef __ABSTRACT_H_INCLUDED__
#define __ABSTRACT_H_INCLUDED__

#include "pattern.hh"
#include "node.hh"

class AbstPattern : public Pattern {
private:
    std::vector<ModeStmt> sList;
    std::string pname;
    std::string extname;
    bool error;
    Node* tree;
public:
    bool initialized;

    bool resolved;
    AbstPattern(std::string name){ 
        pname = name;
        error = false;
        resolved = false;
    }
    std::string getPatName();
    void setPatName(std::string V);
    void addStmt(ModeStmt s);
    void addStmts (std::vector<ModeStmt> msv) {
        sList.insert (sList.end (), msv.begin (), msv.end ());
    }
    std::vector<ModeStmt>* getStmts () {
        return &sList;
    }
    void setExtName(std::string V);
    std::string getExtName();
    void setSList(std::vector <ModeStmt> * s);
    void setError();
    bool inError();
    void setPatType(Type t);
    Type getPatType () {
        return abstract;
    }
    void setParentPattern(Pattern*);
    Pattern* getParentPattern();
    void createPattern();

    // temporary Debug statements.
    int getSListSize () {
        return sList.size ();
    }

    Node* getTree(){
        return tree;
    }

   AbstPattern();
    ~AbstPattern ();
};

#endif

