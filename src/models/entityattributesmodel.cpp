#include "entityattributesmodel.h"

EntityAttributesModel::EntityAttributesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

Entity *EntityAttributesModel::entity() const
{
    return m_entity;
}

void EntityAttributesModel::setEntity(Entity *entity)
{
    if (m_entity == entity)
        return;

    beginResetModel();
    m_entity = entity;
    endResetModel();

    emit entityChanged(m_entity);
}

int EntityAttributesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (m_entity == nullptr)
        return 0;

    return m_entity->attributes().count();
}

QVariant EntityAttributesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (m_entity == nullptr)
        return QVariant();

    switch (role) {
    case KeyRole:
        return m_entity->attributes().keys().at(index.row());

    case ValueRole:
        return m_entity->attributes().values().at(index.row()).toString();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EntityAttributesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[KeyRole]      = "key";
    roles[ValueRole]    = "value";

    return roles;
}
