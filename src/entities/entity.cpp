#include "entity.h"

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

Entity::Entity(const QJsonObject &json, QObject *parent) :
    QObject(parent)
{
    setJson(json);
}

Entity::~Entity()
= default;

void Entity::setJson(const QJsonObject &json)
{
    if (json.isEmpty())
        return;

    setEntityId(json.value(QStringLiteral("entity_id")).toString());
    if (m_entityId.isEmpty())
        return;

    setState(json.value(QStringLiteral("state")).toVariant());

    // parse sensor type
    const QString type = m_entityId.split(".").first();

    if (type == QStringLiteral("alarm_control_panel"))
        setType(Alarm);
    else if (type == QStringLiteral("automation"))
        setType(Automation);
    else if (type == QStringLiteral("binary_sensor"))
        setType(BinarySensor);
    else if (type == QStringLiteral("camera"))
        setType(Camera);
    else if (type == QStringLiteral("climate"))
        setType(Climate);
    else if (type == QStringLiteral("device_tracker"))
        setType(DeviceTracker);
    else if (type == QStringLiteral("group"))
        setType(Group);
    else if (type == QStringLiteral("light"))
        setType(Light);
    else if (type == QStringLiteral("media_player"))
        setType(MediaPlayer);
    else if (type == QStringLiteral("person"))
        setType(Person);
    else if (type == QStringLiteral("sensor"))
        setType(Sensor);
    else if (type == QStringLiteral("sun"))
        setType(Sun);
    else if (type == QStringLiteral("switch"))
        setType(Switch);
    else if (type == QStringLiteral("weather"))
        setType(Weather);
    else if (type == QStringLiteral("zone"))
        setType(Zone);
    else
        setType(Unkown);

    // parse attributes
    const QJsonObject attributes = json.value(QStringLiteral("attributes")).toObject();
    setName(attributes.value(QStringLiteral("friendly_name")).toString());
    setAttributes(attributes.toVariantMap());
    setFeatures(attributes.value(QStringLiteral("supported_features")).toInt(0));

    // parse context
    setContext(json.value(QStringLiteral("context")).toObject().toVariantMap());
}

QVariantMap Entity::attributes() const
{
    return m_attributes;
}

QVariantMap Entity::context() const
{
    return m_context;
}

QString Entity::entityId() const
{
    return m_entityId;
}

int Entity::features() const
{
    return m_features;
}

QString Entity::name() const
{
    return m_name;
}

QVariant Entity::state() const
{
    return m_state;
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
    emit attributesChanged(m_attributes);
}

void Entity::setContext(const QVariantMap &context)
{
    if (m_context == context)
        return;

    m_context = context;
    emit contextChanged(m_context);
}

void Entity::setEntityId(const QString &id)
{
    if (m_entityId == id)
        return;

    m_entityId = id;
    emit entityIdChanged(m_entityId);
}

void Entity::setFeatures(int features)
{
    if (m_features == features)
        return;

    m_features = features;
    emit featuresChanged(m_features);
}

void Entity::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void Entity::setState(const QVariant &state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
}

void Entity::setType(Entity::EntityType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}
