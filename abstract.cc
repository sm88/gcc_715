/*--------------------------------------------------------------------------- *
 * srtl implementation in c++                                                 *
 *                                                                            *
 * This file contains class header definitions abstract pattern class and     *
 * supporting class.                                                          *
 *                                                                            *
 *                                                                            *
 * Change History                                                             *
 * -------------------------------------------------------------------------- *
 * Name        Date          Change Description                               *
 * Sheweta     14-Aug-13     Initial Version                                  *
 *                                                                            *
 *--------------------------------------------------------------------------- */
#include "abstract.hh"
#include "node.hh"
#include "srtl-driver.hh"

std::string AbstPattern::getPatName () 
{
    return pname;
}

void AbstPattern::setPatName (std::string V) 
{
    pname=V;
}

void AbstPattern::addStmt(ModeStmt s) 
{
    sList.push_back(s);
}

void AbstPattern::setSList (std::vector<ModeStmt>* s) 
{
    sList= * s;
}

void AbstPattern::setExtName (std::string V) 
{
    extname=V;
}

std::string AbstPattern::getExtName () 
{
    return extname; 
}

void AbstPattern:: setError()
{
   error = true;
}

bool AbstPattern::inError()
{
   return error;
}

void AbstPattern::setPatType(Type t){
    type = t;
}

void AbstPattern::setParentPattern(Pattern* p){
    AbstPattern* ab = static_cast <AbstPattern*> (p);
    std::vector<ModeStmt>* msv = ab->getStmts ();
    
    sList.insert (sList.begin (), msv->begin(), msv->end ());
}

Pattern* AbstPattern::getParentPattern () {
    return NULL;
}

void AbstPattern::createPattern(){
    //cerr<<"ABSTRACT"<<endl;
    std::vector<ModeStmt>* ms = this->getStmts ();
    if (type != regCons) {
        for (unsigned int i = 0; i < ms->size (); i++) {
            if(ms->at (i).getType () == ModeStmt::root) {
                if (i == 0) {
                    int children = srtl_driver::findRtlOperandSymTabEntry (
                        ms->at (i).getPatternName ());
                    if (children < 0) {
                        srtl_driver::debugS (srtl_driver::err,
                            "For Abst Pattern: " + this->getPatName ());
                        srtl_driver::debugS (srtl_driver::err,
                            "Invalid RTL Operand: " +
                            ms->at (i).getPatternName ());
                        return;
                    }
                    tree = new Node (ms->at (i).getPatternName (), false);
                } else {
                    Node* iNode = new Node (ms->at (i).
                        getPatternName (), false);
                    int children = srtl_driver::findRtlOperandSymTabEntry (
                        ms->at (i).getPatternName ());
                    if (children < 0) {
                        srtl_driver::debugS (srtl_driver::err,
                            "For Abst Pattern: " + this->getPatName ());
                        srtl_driver::debugS (srtl_driver::err,
                            "Invalid RTL Operand: " +
                            ms->at (i).getPatternName ());
                        delete iNode;
                        delete tree;
                        return;
                    }
                    tree->replaceNode (iNode, ms->at (i).getAccessTree ());
                }
            }
        }

        this->resolved = true;

    }
}

AbstPattern::~AbstPattern () {
}

AbstPattern::AbstPattern () {

 initialized=false;
}

