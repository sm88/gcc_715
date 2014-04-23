#ifndef LISTVIEWMODEL_H
#define LISTVIEWMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <map>
#include <QModelIndex>
#include <bnode.h>
#include "pattern_dto.h"

class ListViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ListViewModel(const std::vector<PatternDTO*>& aPatterns, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    PatternDTO* getSelectedModelObject(int aRow);
    int getVectorSize();

signals:

public slots:

private:
    std::vector<PatternDTO*> m_patterns;
};

#endif // LISTVIEWMODEL_H
