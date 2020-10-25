#include "entitiesprovider.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonDocument>
#include <QJsonObject>

EntitiesProvider::EntitiesProvider(QObject *parent) :
    QObject(parent),
    m_typesModel(new EntityTypesModel(this))
{

}

void EntitiesProvider::setApi(HomeassistantApi *api)
{
    if (!api)
        return;

    m_api = api;
    connect(m_api, &HomeassistantApi::dataAvailable, this, &EntitiesProvider::onDataAvailable);
}

void EntitiesProvider::callService(const QString &domain, const QString &service, const QString &entityId, const QJsonObject &data)
{
    m_api->callService(domain, service, entityId, data);
}

EntitiesModel *EntitiesProvider::model(const int &type)
{
    return m_models.value(Entity::EntityType(type), nullptr);
}

EntityTypesModel *EntitiesProvider::typesModel()
{
    return m_typesModel;
}

Entity *EntitiesProvider::entityById(const QString &entityId)
{
    auto *model = m_models.value(getEntityType(entityId), nullptr);

    if (!model)
        return nullptr;

    return model->entityById(entityId);
}

QString EntitiesProvider::getEntityIcon(quint16 type) const
{
    switch (type) {
    case Entity::Alarm:
        return QStringLiteral("image://theme/icon-m-alarm");

    case Entity::Automation:
        return QStringLiteral("image://theme/icon-m-toy");

    case Entity::Camera:
        return QStringLiteral("image://theme/icon-m-video");

    case Entity::Climate:
        return QStringLiteral("image://theme/icon-m-ambience");

    case Entity::Group:
        return QStringLiteral("image://theme/icon-m-levels");

    case Entity::Light:
        return QStringLiteral("image://theme/icon-m-day");

    case Entity::Person:
        return QStringLiteral("image://theme/icon-m-users");

    case Entity::Sensor:
    case Entity::BinarySensor:
        return QStringLiteral("image://theme/icon-m-global-proxy");

    case Entity::Switch:
        return QStringLiteral("image://theme/icon-m-charging");

    default:
        return QStringLiteral("image://theme/icon-m-asterisk");
    }
}

bool EntitiesProvider::loading() const
{
    return m_loading;
}

void EntitiesProvider::refresh()
{
    setLoading(true);
    m_api->getStates();
}

void EntitiesProvider::updateEntity(const QString &entityId)
{
    m_api->getState(entityId);
}

void EntitiesProvider::updateModel(int type)
{
    if (!m_models.keys().contains(Entity::EntityType(type)))
        return;

    for (const Entity *entity : m_models.value(Entity::EntityType(type))->entities()) {
        m_api->getState(entity->entityId());
    }
}

void EntitiesProvider::updateState(const QJsonObject &obj)
{
    auto *entity = entityById(obj.value("entity_id").toString());

    if (!entity)
        return;

    entity->setState(obj.value(QStringLiteral("state")).toVariant());
    entity->setAttributes(obj.value(QStringLiteral("attributes")).toObject().toVariantMap());
}

void EntitiesProvider::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

void EntitiesProvider::onDataAvailable(const QString &endpoint, const QJsonDocument &doc)
{
#ifdef QT_DEBUG
    qDebug() << endpoint;
#endif

    if (endpoint == QStringLiteral(HASS_API_ENDPOINT_STATES)) {
        parseStates(doc.array());
    } else if (endpoint.startsWith(QStringLiteral(HASS_API_ENDPOINT_STATES) + "/")) {
        updateEntity(doc.object());
    } else if (endpoint.startsWith(QStringLiteral(HASS_API_ENDPOINT_SERVICES))) {
        updateEntities(doc.array());
    }
}

void EntitiesProvider::addEntityToModel(const Entity::EntityType &type, Entity *entity)
{
    if (!m_models.contains(type))
        registerModel(type);

    m_models.value(type)->addEntity(entity);
}

Entity::EntityType EntitiesProvider::getEntityType(const QString &entityId) const
{
    const QString type = entityId.split(".").first();

    if (type == QStringLiteral("alarm_control_panel")) {
        return Entity::Alarm;
    } else if (type == QStringLiteral("automation")) {
        return Entity::Automation;
    } else if (type == QStringLiteral("binary_sensor")) {
        return Entity::BinarySensor;
    } else if (type == QStringLiteral("camera")) {
        return Entity::Camera;
    } else if (type == QStringLiteral("climate")) {
        return Entity::Climate;
    } else if (type == QStringLiteral("device_tracker")) {
        return Entity::DeviceTracker;
    } else if (type == QStringLiteral("group")) {
        return Entity::Group;
    } else if (type == QStringLiteral("light")) {
        return Entity::Light;
    } else if (type == QStringLiteral("media_player")) {
        return Entity::MediaPlayer;
    } else if (type == QStringLiteral("person")) {
        return Entity::Person;
    } else if (type == QStringLiteral("sensor")) {
        return Entity::Sensor;
    } else if (type == QStringLiteral("sun")) {
        return Entity::Sun;
    } else if (type == QStringLiteral("switch")) {
        return Entity::Switch;
    } else if (type == QStringLiteral("weather")) {
        return Entity::Weather;
    } else if (type == QStringLiteral("zone")) {
        return Entity::Zone;
    } else {
        return Entity::Unkown;
    }
}

void EntitiesProvider::parseStates(const QJsonArray &states)
{
    // reset models
    for (EntitiesModel *model : m_models) {
        model->reset();
    }

    // add data to models
    for (const QJsonValue &item : states) {
        const QJsonObject obj = item.toObject();

        // check entity type
        const Entity::EntityType type = getEntityType(obj.value(QStringLiteral("entity_id")).toString());

        Entity *entity{nullptr};

        switch (type) {
        case Entity::Alarm:
        case Entity::Automation:
        case Entity::BinarySensor:
        case Entity::Camera:
        case Entity::DeviceTracker:
        case Entity::MediaPlayer:
        case Entity::Person:
        case Entity::Sensor:
        case Entity::Sun:
        case Entity::Switch:
        case Entity::Weather:
        case Entity::Zone:
            entity = new Entity;
            entity->setType(type);
            break;

        case Entity::Climate:
            entity = new Climate;
            break;

        case Entity::Group:
            entity = new Group;
            break;

        case Entity::Light:
            entity = new Light;
            break;

        default:
            entity = new Entity;
            entity->setType(Entity::Unkown);
            break;
        }

        // set data to entity
        entity->setEntityId(obj.value(QStringLiteral("entity_id")).toString());
        entity->setState(obj.value(QStringLiteral("state")).toVariant());

        // parse attributes
        const QJsonObject attributes = obj.value(QStringLiteral("attributes")).toObject();
        entity->setName(attributes.value(QStringLiteral("friendly_name")).toString());
        entity->setAttributes(attributes.toVariantMap());
        entity->setSupportedFeatures(quint16(attributes.value(QStringLiteral("supported_features")).toInt(0)));

        // parse context
        entity->setContext(obj.value(QStringLiteral("context")).toObject().toVariantMap());


        // --------------------------------------------------------------------
        // check homeassistant update info is availabel (binary_sensor.updater)
        if (entity->entityId() == QStringLiteral("binary_sensor.updater")) {
            emit homeassistantVersionAvailable(entity->attributes().value(QStringLiteral("newest_version")).toString());
        }
        // --------------------------------------------------------------------

        // add to model or delete
        switch (entity->type()) {

        case Entity::Alarm:
        case Entity::Automation:
        case Entity::Camera:
        case Entity::Climate:
        case Entity::Group:
        case Entity::Light:
        case Entity::Person:
        case Entity::Switch:
            addEntityToModel(entity->type(), entity);
            break;

        case Entity::Sensor:
        case Entity::BinarySensor:
            addEntityToModel(Entity::Sensor, entity);
            break;

        default:
            entity->deleteLater();
            break;
        }
    }

    // process group children
    if (m_models.keys().contains(Entity::Group)) {
        for (Entity *entity : m_models.value(Entity::Group)->entities()) {
            auto *group = qobject_cast<Group *>(entity);

            for (const QString &childId : entity->attributes().value(QStringLiteral("entity_id")).toStringList()) {
                for (EntitiesModel *model : m_models.values()) {
                    Entity *child = model->entityById(childId);

                    if (!child)
                        continue;

                    group->childrenModel()->addEntity(child);
                    break;
                }
            }
        }
    }

    setLoading(false);
}

void EntitiesProvider::registerModel(const Entity::EntityType &entityType)
{
    EntityTypeItem item;
    item.type = entityType;
    item.icon = getEntityIcon(entityType);

    switch (entityType) {
    case Entity::Alarm:
        item.title = tr("Alarms");
        item.description = tr("List of all alarms");
        break;

    case Entity::Automation:
        item.title = tr("Automations");
        item.description = tr("List of all automations");
        break;

    case Entity::Camera:
        item.title = tr("Cameras");
        item.description = tr("List of all cameras");
        break;

    case Entity::Climate:
        item.title = tr("Climates");
        item.description = tr("List of all climates");
        break;

    case Entity::Group:
        item.title = tr("Groups");
        item.description = tr("List of all groups");
        break;

    case Entity::Light:
        item.title = tr("Lights");
        item.description = tr("List of all lights");
        break;

    case Entity::Person:
        item.title = tr("Persons");
        item.description = tr("List of all persons");
        break;

    case Entity::Sensor:
    case Entity::BinarySensor:
        item.title = tr("Sensors");
        item.description = tr("List of all sensors");
        item.type = Entity::Sensor;
        break;

    case Entity::Switch:
        item.title = tr("Switches");
        item.description = tr("List of all switches");
        break;

    default:
        return;
    }

    m_typesModel->addItem(item);


    auto *model = new EntitiesModel(this);

    m_models.insert(entityType, model);
}

void EntitiesProvider::updateEntity(const QJsonObject &obj)
{
    const QString id = obj.value(QStringLiteral("entity_id")).toString();

    const Entity::EntityType type = getEntityType(id);

    EntitiesModel *model = m_models.value(type, nullptr);

    if (!model)
        return;

    Entity *entity = model->entityById(id);

    if (!entity)
        return;

    // set state
    entity->setState(obj.value(QStringLiteral("state")).toVariant());

    // parse attributes
    const QJsonObject attributes = obj.value(QStringLiteral("attributes")).toObject();
    entity->setAttributes(attributes.toVariantMap());
    entity->setSupportedFeatures(quint16(attributes.value(QStringLiteral("supported_features")).toInt(0)));

    // parse context
    entity->setContext(obj.value(QStringLiteral("context")).toObject().toVariantMap());

    // --------------------------------------------------------------------
    // check homeassistant update info is availabel (binary_sensor.updater)
    if (entity->entityId() == QStringLiteral("binary_sensor.updater")) {
        emit homeassistantVersionAvailable(entity->attributes().value(QStringLiteral("newest_version")).toString());
    }
    // -------------------------------------------------------------------
}

void EntitiesProvider::updateEntities(const QJsonArray &entities)
{
    for (const QJsonValue &item : entities) {
        updateEntity(item.toObject());
    }
}
