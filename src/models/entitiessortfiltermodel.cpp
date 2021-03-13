#include "entitiessortfiltermodel.h"

#include "entitiesmodel.h"

EntitiesSortFilterModel::EntitiesSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(EntitiesModel::NameRole);
    setDynamicSortFilter(true);
}

void EntitiesSortFilterModel::addEntityFilter(quint8 entityType)
{
    if (m_entityFilters.contains(entityType))
        return;

    m_entityFilters.append(entityType);
    invalidate();
}

Entity *EntitiesSortFilterModel::entityAt(int i)
{
    const QModelIndex idx = this->index(i, 0, QModelIndex());

    if (!idx.isValid())
        return nullptr;

    auto model = qobject_cast<EntitiesModel *>(sourceModel());

    return model->entityAt(mapToSource(idx));
}

void EntitiesSortFilterModel::resetEntityFilter()
{
    m_entityFilters.clear();
    invalidate();
}

bool EntitiesSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (this->sourceModel() == nullptr)
        return false;

    if (m_entityFilters.isEmpty())
        return true;

    auto index = this->sourceModel()->index(source_row, 0, source_parent);
    if (!index.isValid())
        return false;

    auto type = index.data(EntitiesModel::TypeRole).toUInt();

    return m_entityFilters.contains(type);
}
