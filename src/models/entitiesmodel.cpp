#include "entitiesmodel.h"

EntitiesModel::EntitiesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int EntitiesModel::entitiesCount() const
{
    return m_entities.count();
}

Entity *EntitiesModel::entityById(const QString &entityId)
{
    for (auto e : m_entities) {
        auto entity = qobject_cast<Entity *>(e);
        if (entity->entityId() == entityId)
            return entity;
    }

    return nullptr;
}

Entity *EntitiesModel::entityAt(int index)
{
    if (index < 0 || index >= rowCount(QModelIndex()))
        return nullptr;

    return m_entities.at(index);
}

Entity *EntitiesModel::entityAt(const QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;

    return m_entities.at(index.row());
}

QList<Entity *> EntitiesModel::entities() const
{
    return m_entities;
}

bool EntitiesModel::isEmpty() const
{
    return m_entities.isEmpty();
}

void EntitiesModel::setParentMode(bool enable)
{
    m_parentMode = enable;
}
void EntitiesModel::addEntity(Entity *entity)
{
    if (entity == nullptr)
        return;

    beginInsertRows(QModelIndex(), m_entities.count(), m_entities.count());
    if (m_parentMode)
        entity->setParent(this);

    m_entities.append(entity);
    connect(entity, &Entity::changed, this, &EntitiesModel::onEntityChanged);
    endInsertRows();

    emit changed();
}

void EntitiesModel::setEntities(const QList<Entity *> &entities)
{
    beginResetModel();
    if (m_parentMode) {
        qDeleteAll(m_entities.begin(), m_entities.end());
        m_entities.clear();
    }

    m_entities = entities;

    for (auto entity : m_entities) {
        if (m_parentMode)
            entity->setParent(this);

        connect(entity, &Entity::changed, this, &EntitiesModel::onEntityChanged);
    }
    endResetModel();

    emit changed();
}


void EntitiesModel::reset()
{
    beginResetModel();
    if (!m_entities.isEmpty()) {
        qDeleteAll(m_entities.begin(), m_entities.end());
        m_entities.clear();
    }
    endResetModel();
}

void EntitiesModel::onEntityChanged()
{
    auto entity = qobject_cast<Entity *>(sender());

    if (entity == nullptr)
        return;

    const int idx = m_entities.indexOf(entity);

    if (idx < 0) {
        addEntity(entity);
    } else {
        const QModelIndex index = QAbstractListModel::createIndex(idx, 0, entity);
        emit dataChanged(index, index);
    }

    emit changed();
}

int EntitiesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_entities.count();
}

QVariant EntitiesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto entity = m_entities.at(index.row());

    switch (role) {
    case NameRole:
        return entity->name();

    case AttributesRole:
        return entity->attributes();

    case ContextRole:
        return entity->context();

    case EntityIdRole:
        return entity->entityId();

    case SupportedFeaturesRole:
        return entity->supportedFeatures();

    case StateRole:
        return entity->state().toString();

    case TypeRole:
        return int(entity->type());

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EntitiesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[AttributesRole]           = "attributes";
    roles[ContextRole]              = "context";
    roles[EntityIdRole]             = "entityId";
    roles[SupportedFeaturesRole]    = "supportedFeatures";
    roles[NameRole]                 = "name";
    roles[StateRole]                = "entityState";
    roles[TypeRole]                 = "type";

    return roles;
}
