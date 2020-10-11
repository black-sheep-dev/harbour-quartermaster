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

bool EntitiesProvider::loading() const
{
    return m_loading;
}

void EntitiesProvider::refresh()
{
    setLoading(true);
    m_api->getStates();
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
        return;
    }
}

void EntitiesProvider::addEntityToModel(const Entity::EntityType &type, Entity *entity)
{
    if (!m_models.contains(type))
        registerModel(type);

    m_models.value(type)->addEntity(entity);
}

void EntitiesProvider::parseStates(const QJsonArray &states)
{
    // reset models
    for (EntitiesModel *model : m_models) {
        model->reset();
    }

    // add data to models
    for (const QJsonValue &item : states) {
        Entity *entity = new Entity(item.toObject());

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

    setLoading(false);
}

void EntitiesProvider::registerModel(const Entity::EntityType &entityType)
{
    EntityTypeItem item;
    item.type = entityType;

    switch (entityType) {
    case Entity::Alarm:
        item.title = tr("Alarms");
        item.description = tr("List of all alarms");
        item.icon = "image://theme/icon-m-alarm";
        break;

    case Entity::Automation:
        item.title = tr("Automations");
        item.description = tr("List of all automations");
        item.icon = "image://theme/icon-m-toy";
        break;

    case Entity::Camera:
        item.title = tr("Cameras");
        item.description = tr("List of all cameras");
        item.icon = "image://theme/icon-m-video";
        break;

    case Entity::Climate:
        item.title = tr("Climates");
        item.description = tr("List of all climates");
        item.icon = "image://theme/icon-m-ambience";
        break;

    case Entity::Group:
        item.title = tr("Groups");
        item.description = tr("List of all groups");
        item.icon = "image://theme/icon-m-levels";
        break;

    case Entity::Light:
        item.title = tr("Lights");
        item.description = tr("List of all lights");
        item.icon = "image://theme/icon-m-day";
        break;

    case Entity::Person:
        item.title = tr("Persons");
        item.description = tr("List of all persons");
        item.icon = "image://theme/icon-m-users";
        break;

    case Entity::Sensor:
    case Entity::BinarySensor:
        item.title = tr("Sensors");
        item.description = tr("List of all sensors");
        item.icon = "image://theme/icon-m-global-proxy";
        item.type = Entity::Sensor;
        break;

    case Entity::Switch:
        item.title = tr("Switches");
        item.description = tr("List of all switches");
        item.icon = "image://theme/icon-m-charging";
        break;

    default:
        return;
    }

    m_typesModel->addItem(item);


    auto *model = new EntitiesModel(this);

    m_models.insert(entityType, model);
}
