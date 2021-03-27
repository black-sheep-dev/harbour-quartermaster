#include "entitiesservice.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>

#include "src/constants.h"

EntitiesService::EntitiesService(QObject *parent) :
    Service(parent)
{

}

EntitiesService::~EntitiesService()
{
    writeSettings();
}

EntitiesModel *EntitiesService::entitiesModel()
{
    return m_entitiesModel;
}

EntityTypesModel *EntitiesService::entityTypesModel()
{
    return m_entityTypesModel;
}

void EntitiesService::callService(const QString &domain, const QString &service, const QString &entityId)
{
    QJsonObject payload;
    payload.insert(ApiKey::KEY_ENTITY_ID, entityId);

    callService(domain, service, payload);
}

void EntitiesService::callService(const QString &domain, const QString &service, const QJsonObject &payload)
{
    emit apiRequest(Api::RequestPostApiServices,
                    domain + "/" + service,
                    payload);
}

QString EntitiesService::getEntityIcon(quint8 entityType) const
{
    switch (entityType) {
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

void EntitiesService::getEntityState(const QString &entityId)
{
    emit apiRequest(Api::RequestGetApiStatesEntity, entityId);
}

void EntitiesService::refresh()
{
    emit apiRequest(Api::RequestGetApiStates);
}

bool EntitiesService::liveUpdates() const
{
    return m_liveUpdates;
}

void EntitiesService::updateEntity(const QJsonObject &obj)
{
    auto entity = m_entitiesModel->entityById(obj.value(ApiKey::KEY_ENTITY_ID).toString());

    if (entity == nullptr)
        return;

    entity->setState(obj.value(ApiKey::KEY_STATE).toVariant());
    entity->setAttributes(obj.value(ApiKey::KEY_ATTRIBUTES).toObject().toVariantMap());
}

void EntitiesService::setLiveUpdates(bool liveUpdates)
{
    if (m_liveUpdates == liveUpdates)
        return;

    m_liveUpdates = liveUpdates;
    emit liveUpdatesChanged(m_liveUpdates);

    // update api subscription
    if (m_liveUpdates)
        api()->addWebsocketEventsSubscription(ApiKey::KEY_STATE_CHANGED);
    else
        api()->removeWebsocketEventsSubscription(ApiKey::KEY_STATE_CHANGED);
}

Entity::EntityType EntitiesService::getEntityType(const QString &entityId) const
{
    const QString type = entityId.section('.', 0, 0);

    if (type == QLatin1String("alarm_control_panel")) {
        return Entity::Alarm;
    } else if (type == QLatin1String("automation")) {
        return Entity::Automation;
    } else if (type == QLatin1String("binary_sensor")) {
        return Entity::BinarySensor;
    } else if (type == QLatin1String("camera")) {
        return Entity::Camera;
    } else if (type == QLatin1String("climate")) {
        return Entity::Climate;
    } else if (type == QLatin1String("device_tracker")) {
        return Entity::DeviceTracker;
    } else if (type == QLatin1String("group")) {
        return Entity::Group;
    } else if (type == QLatin1String("light")) {
        return Entity::Light;
    } else if (type == QLatin1String("media_player")) {
        return Entity::MediaPlayer;
    } else if (type == QLatin1String("person")) {
        return Entity::Person;
    } else if (type == QLatin1String("sensor")) {
        return Entity::Sensor;
    } else if (type == QLatin1String("sun")) {
        return Entity::Sun;
    } else if (type == QLatin1String("switch")) {
        return Entity::Switch;
    } else if (type == QLatin1String("weather")) {
        return Entity::Weather;
    } else if (type == QLatin1String("zone")) {
        return Entity::Zone;
    } else {
        return Entity::Unkown;
    }
}

EntityTypeItem EntitiesService::getEntityTypeItem(const Entity::EntityType &entityType, quint16 count) const
{
    EntityTypeItem item;
    item.type = entityType;
    item.icon = getEntityIcon(entityType);
    item.count = count;

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
        break;
    }

    return item;
}

void EntitiesService::parseStates(const QJsonArray &arr)
{
#ifdef QT_DEBUG
    qDebug() << "PARSE STATES";
#endif

    // add data to model
    QList<Entity *> entities;
    QHash<Entity::EntityType, quint16> types;

    for (const auto &item : arr) {
        const QJsonObject obj = item.toObject();

        // check entity type
        const Entity::EntityType type = getEntityType(obj.value(ApiKey::KEY_ENTITY_ID).toString());

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
        entity->setEntityId(obj.value(ApiKey::KEY_ENTITY_ID).toString());
        entity->setState(obj.value(ApiKey::KEY_STATE).toVariant());

        // parse attributes
        const QJsonObject attributes = obj.value(ApiKey::KEY_ATTRIBUTES).toObject();
        entity->setName(attributes.value(ApiKey::KEY_FRIENDLY_NAME).toString());
        entity->setAttributes(attributes.toVariantMap());
        entity->setSupportedFeatures(quint16(attributes.value(ApiKey::KEY_SUPPORTED_FEATURES).toInt(0)));

        // parse context
        entity->setContext(obj.value(ApiKey::KEY_CONTEXT).toObject().toVariantMap());

        // --------------------------------------------------------------------
        // check homeassistant update info is availabel (binary_sensor.updater)
        if (entity->entityId() == QStringLiteral("binary_sensor.updater")) {
            emit homeassistantVersionAvailable(entity->attributes().value(ApiKey::KEY_NEWEST_VERSION).toString());
        }
        // --------------------------------------------------------------------

        // add or delete
        switch (type) {

        case Entity::Alarm:
        case Entity::Automation:
        case Entity::Camera:
        case Entity::Climate:
        case Entity::Group:
        case Entity::Light:
        case Entity::Person:
        case Entity::Switch:
        case Entity::Sensor:
            entities.append(entity);
            types.insert(type, types.value(type, 0) + 1);
            break;

        case Entity::BinarySensor:
            entities.append(entity); 
            types.insert(Entity::Sensor, types.value(Entity::Sensor, 0) + 1);
            break;

        default:
            entity->deleteLater();
            break;
        }
    }

    // add to model
    m_entitiesModel->setEntities(entities);

    // register types
    QList<EntityTypeItem> items;

    QHashIterator<Entity::EntityType, quint16> iter(types);
    while (iter.hasNext()) {
        iter.next();

        items.append(getEntityTypeItem(iter.key(), iter.value()));
    }
    m_entityTypesModel->setItems(items);

    // process group children
    if (types.keys().contains(Entity::Group)) {
        for (auto entity : entities) {
            if (entity->type() != Entity::Group)
                continue;

            auto group = qobject_cast<Group *>(entity);
            auto childIds = group->attributes().value(ApiKey::KEY_ENTITY_ID).toStringList();

            for (const auto child : entities) {
                if (!childIds.contains(child->entityId()))
                    continue;

                group->childrenModel()->addEntity(child);

                childIds.removeAll(child->entityId());
                if (childIds.isEmpty())
                    break;
            }
        }
    }
}

void EntitiesService::connectToApi()
{
    connect(this, &EntitiesService::apiRequest, api(), &ApiInterface::sendRequest);
    connect(api(), &ApiInterface::requestFinished, this, &EntitiesService::onRequestFinished);
    connect(api(), &ApiInterface::requestError, this, &EntitiesService::onRequestError);
    connect(api(), &ApiInterface::websocketEvent, this, &EntitiesService::onWebsocketEvent);
}

void EntitiesService::initialize()
{
    setState(ServiceState::StateInitalizing);
    readSettings();
    refresh();
}

void EntitiesService::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("ENTITIES"));
    setLiveUpdates(settings.value(QStringLiteral("live_updates"), false).toBool());
    settings.endGroup();
}

void EntitiesService::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("ENTITIES"));
    settings.setValue(QStringLiteral("live_updates"), m_liveUpdates);
    settings.endGroup();
}

void EntitiesService::onRequestFinished(quint8 requestType, const QJsonDocument &data)
{
    switch (requestType) {
    case Api::RequestGetApiStates:
        parseStates(data.array());

        if (state() != ServiceState::StateInitialized)
            setState(ServiceState::StateInitialized);
        break;

    case Api::RequestPostApiServices:
    case Api::RequestGetApiStatesEntity:
        updateEntity(data.object());
        break;

    default:
        break;
    }
}

void EntitiesService::onWebsocketEvent(const QString &event, const QJsonValue &data)
{
    if (event != ApiKey::KEY_STATE_CHANGED)
        return;

    updateEntity(data.toObject().value(ApiKey::KEY_NEW_STATE).toObject());
}
