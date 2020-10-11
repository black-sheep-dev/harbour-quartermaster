#include "entitiessortfiltermodel.h"

#include "entitiesmodel.h"

EntitiesSortFilterModel::EntitiesSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(EntitiesModel::NameRole);
    setFilterRole(EntitiesModel::NameRole);
}

Entity *EntitiesSortFilterModel::entityAt(int i)
{
    const QModelIndex idx = this->index(i, 0, QModelIndex());

    if (!idx.isValid())
        return nullptr;

    auto *model = qobject_cast<EntitiesModel *>(sourceModel());

    return model->entityAt(mapToSource(idx));
}

void EntitiesSortFilterModel::sortModel()
{
    sort(0, Qt::AscendingOrder);
}

void EntitiesSortFilterModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    auto *model = qobject_cast<EntitiesModel *>(sourceModel);
    connect(model, &EntitiesModel::changed, this, &EntitiesSortFilterModel::sortModel);

    QSortFilterProxyModel::setSourceModel(sourceModel);

    sortModel();
}
