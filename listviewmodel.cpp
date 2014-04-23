#include "listviewmodel.h"
#include <QVariant>
#include <cstdio>

ListViewModel::ListViewModel(const std::vector<PatternDTO*>& aPatterns, QObject *parent) :
    QAbstractListModel(parent)
{
    m_patterns = aPatterns;
}

int ListViewModel::rowCount(const QModelIndex &parent) const{
    return m_patterns.size();
}

QVariant ListViewModel::data(const QModelIndex &index, int role) const{
    //validity of index
    if (!index.isValid())
        return QVariant();
    if(index.row() >= m_patterns.size())
        return QVariant();

    if(role == Qt::DisplayRole){
        char buffer[1000];
        sprintf(buffer, "%s", (m_patterns.at(index.row()))->getName().c_str());
        return QVariant(buffer);
    }
    else
        return QVariant();
}

PatternDTO* ListViewModel::getSelectedModelObject(int aIndex){
    if(aIndex < m_patterns.size())
        return m_patterns[aIndex];
}

int ListViewModel::getVectorSize(){
    return m_patterns.size();
}
