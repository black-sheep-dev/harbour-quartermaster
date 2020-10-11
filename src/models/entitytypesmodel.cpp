#include "entitytypesmodel.h"

EntityTypesModel::EntityTypesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

void EntityTypesModel::addItem(const EntityTypeItem &item)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(item);
    endInsertRows();
}

void EntityTypesModel::setItems(const QList<EntityTypeItem> &items)
{
    beginResetModel();
    m_items.clear();
    m_items = items;
    endResetModel();
}

int EntityTypesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_items.count();
}

QVariant EntityTypesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const EntityTypeItem item = m_items.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return item.title;

    case TitleRole:
        return item.title;

    case DescriptionRole:
        return item.description;

    case IconRole:
        return item.icon;

    case TypeRole:
        return item.type;

    default:
        return QVariant();

    }
}

QHash<int, QByteArray> EntityTypesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[TitleRole]        = "title";
    roles[DescriptionRole]  = "description";
    roles[IconRole]         = "icon";
    roles[TypeRole]         = "type";

    return roles;
}
