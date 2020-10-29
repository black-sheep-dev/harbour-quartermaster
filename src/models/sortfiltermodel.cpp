#include "sortfiltermodel.h"

SortFilterModel::SortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(Qt::DisplayRole);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(Qt::DisplayRole);
}

void SortFilterModel::setSortOrder(Qt::SortOrder order)
{
    m_sortOrder = order;
    sortModel();
}

void SortFilterModel::sortModel()
{
    sort(0, m_sortOrder);
}

void SortFilterModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &SortFilterModel::sortModel);
    connect(sourceModel, &QAbstractItemModel::modelReset, this, &SortFilterModel::sortModel);

    QSortFilterProxyModel::setSourceModel(sourceModel);

    sortModel();
}
