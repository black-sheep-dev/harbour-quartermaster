#include "sortfiltermodel.h"

SortFilterModel::SortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(Qt::DisplayRole);
    setFilterRole(Qt::DisplayRole);
}

void SortFilterModel::sortModel()
{
    sort(0, Qt::AscendingOrder);
}

void SortFilterModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &SortFilterModel::sortModel);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &SortFilterModel::sortModel);

    QSortFilterProxyModel::setSourceModel(sourceModel);

    sortModel();
}
