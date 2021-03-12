#ifndef ENTITIESSORTFILTERMODEL_H
#define ENTITIESSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

#include "src/entities/entity.h"

class EntitiesSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EntitiesSortFilterModel(QObject *parent = nullptr); 

    Q_INVOKABLE void addEntityFilter(quint8 entityType);
    Q_INVOKABLE Entity *entityAt(int i);
    Q_INVOKABLE void resetEntityFilter();
//    Q_INVOKABLE void sortModel();

private:
    QList<quint8> m_entityFilters;

    // QAbstractProxyModel interface
//public:
//    void setSourceModel(QAbstractItemModel *sourceModel) override;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // ENTITIESSORTFILTERMODEL_H
