#include "entity.h"

#include <QDebug>

Entity::Entity(QObject *parent) :
    QObject(parent),
    m_entityId(QString()),
    m_name(QString())
{

}

Entity::Entity(const Entity &other) :
    QObject(other.parent()),
    m_attributes(other.attributes()),
    m_context(other.context()),
    m_entityId(other.entityId()),
    m_name(other.name()),
    m_state(other.state()),
    m_type(other.type())
{

}

bool Entity::hasFeature(quint16 feature) const
{
    qDebug() << m_supportedFeatures;
    qDebug() << (m_supportedFeatures & feature);

    bool has = (m_supportedFeatures & feature) == feature;

    return has;
}

const QVariantMap &Entity::attributes() const
{
    return m_attributes;
}

const QVariantMap &Entity::context() const
{
    return m_context;
}

const QString &Entity::entityId() const
{
    return m_entityId;
}

const QString &Entity::name() const
{
    return m_name;
}

const QVariant &Entity::state() const
{
    return m_state;
}

quint16 Entity::supportedFeatures() const
{
    return m_supportedFeatures;
}

Entity::EntityType Entity::type() const
{
    return m_type;
}

void Entity::setAttributes(const QVariantMap &attributes)
{
    if (m_attributes == attributes)
        return;

    m_attributes = attributes;
    emit attributesChanged();

    parseAttributes();

    emit changed();
}

void Entity::setContext(const QVariantMap &context)
{
    if (m_context == context)
        return;

    m_context = context;
    emit contextChanged();

    emit changed();
}

void Entity::setEntityId(const QString &id)
{
    if (m_entityId == id)
        return;

    m_entityId = id;
    emit entityIdChanged();
}

void Entity::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
    emit changed();
}

void Entity::setState(const QVariant &state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged();
    emit changed();
}

void Entity::setSupportedFeatures(quint16 supportedFeatures)
{
    if (m_supportedFeatures == supportedFeatures)
        return;

    m_supportedFeatures = supportedFeatures;
    emit supportedFeaturesChanged();
    emit changed();
}

void Entity::setType(Entity::EntityType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged();
}

void Entity::parseAttributes()
{

}
