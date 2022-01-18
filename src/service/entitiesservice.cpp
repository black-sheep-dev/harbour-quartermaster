#include "entitiesservice.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include "src/constants.h"
#include "src/global.h"

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
    if (m_entitiesModel->entitiesCount() == 0)
        return;

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
        //% "Alarms"
        item.title = qtTrId("id-alarms");
        //% "List of all alarms"
        item.description = qtTrId("id-list-of-all-alarms");
        break;

    case Entity::Automation:
        //% "Automations"
        item.title = qtTrId("id-automations");
        //% "List of all automations"
        item.description = qtTrId("id-list-of-all-automations");
        break;

    case Entity::Camera:
        //% "Cameras"
        item.title = qtTrId("id-cameras");
        //% "List of all cameras"
        item.description = qtTrId("id-list-of-all-cameras");
        break;

    case Entity::Climate:
        //% "Climates"
        item.title = qtTrId("id-climates");
        //% "List of all climates"
        item.description = qtTrId("id-list-of-all-climates");
        break;

    case Entity::Group:
        //% "Groups"
        item.title = qtTrId("id-groups");
        //% "List of all groups"
        item.description = qtTrId("id-list-of-all-groups");
        break;

    case Entity::Light:
        //% "Lights"
        item.title = qtTrId("id-lights");
        //% "List of all lights"
        item.description = qtTrId("id-list-of-all-lights");
        break;

    case Entity::Person:
        //% "Persons"
        item.title = qtTrId("id-persons");
        //% "List of all persons"
        item.description = qtTrId("id-list-of-all-persons");
        break;

    case Entity::Sensor:
    case Entity::BinarySensor:
        //% "Sensors"
        item.title = qtTrId("id-sensors");
        //% "List of all sensors"
        item.description = qtTrId("id-list-of-all-sensors");
        item.type = Entity::Sensor;
        break;

    case Entity::Switch:
        //% "Switches"
        item.title = qtTrId("id-switches");
        //% "List of all switches"
        item.description = qtTrId("id-list-of-all-switches");
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

        quint16 features = attributes.value(ApiKey::KEY_SUPPORTED_FEATURES).toInt();

        const auto colorModes = attributes.value(ApiKey::KEY_SUPPORTED_COLOR_MODES).toArray();

        if (!colorModes.isEmpty()) {
            if (colorModes.contains(ApiKey::KEY_BRIGHTNESS)) {
               features |= Light::LightBrightness;
            }

            if (colorModes.contains(ApiKey::KEY_XY)) {
               features |= Light::LightColor;
               features |= Light::LightBrightness;
            }

            if (colorModes.contains(ApiKey::KEY_COLOR_TEMP)) {
               features |= Light::LightColorTemp;
               features |= Light::LightBrightness;
            }

            if (colorModes.contains(ApiKey::KEY_WHITE_VALUE)) {
               features |= Light::LightWhiteValue;
               features |= Light::LightBrightness;
            }
        }

        entity->setSupportedFeatures(features);

        // parse context
        entity->setContext(obj.value(ApiKey::KEY_CONTEXT).toObject().toVariantMap());

        // --------------------------------------------------------------------
        // check homeassistant update info is availabel (binary_sensor.updater)
        if (entity->entityId() == QStringLiteral("binary_sensor.updater")) {
            emit homeAssistantUpdateAvailable(entity->attributes().value(ApiKey::KEY_NEWEST_VERSION).toString());
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

void EntitiesService::readSettings()
{
    QSettings settings(QFile(DEFAULT_CONFIG_FILE).exists() ? DEFAULT_CONFIG_FILE : DEPRECATED_CONFIG_FILE, QSettings::NativeFormat);

    settings.beginGroup(QStringLiteral("ENTITIES"));
    setLiveUpdates(settings.value(QStringLiteral("live_updates"), false).toBool());
    settings.endGroup();
}

void EntitiesService::writeSettings()
{
    QSettings settings(DEFAULT_CONFIG_FILE, QSettings::NativeFormat);

    settings.beginGroup(QStringLiteral("ENTITIES"));
    settings.setValue(QStringLiteral("live_updates"), m_liveUpdates);
    settings.endGroup();
}

void EntitiesService::initialize()
{
    setState(ServiceState::StateInitializing);
    readSettings();
    refresh();
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
