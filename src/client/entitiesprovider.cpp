#include "entitiesprovider.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonDocument>
#include <QJsonObject>

EntitiesProvider::EntitiesProvider(QObject *parent) :
    QObject(parent)
{


    registerModel(AlarmsModel, new EntitiesModel(this));
    registerModel(AutomationsModel, new EntitiesModel(this));
    registerModel(CamerasModel, new EntitiesModel(this));
    registerModel(ClimatesModel, new EntitiesModel(this));
    registerModel(LightsModel, new EntitiesModel(this));
    registerModel(PersonsModel, new EntitiesModel(this));
    registerModel(SensorsModel, new EntitiesModel(this));
    registerModel(SwitchesModel, new EntitiesModel(this));
}

void EntitiesProvider::setApi(HomeassistantApi *api)
{
    if (!api)
        return;

    m_api = api;
    connect(m_api, &HomeassistantApi::dataAvailable, this, &EntitiesProvider::onDataAvailable);
}

EntitiesModel *EntitiesProvider::model(const EntitiesProvider::ModelType &type)
{
    return m_models.value(type, nullptr);
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

    return;
}

void EntitiesProvider::parseStates(const QJsonArray &states)
{
    // reset models
    for (EntitiesModel *model : m_models) {
        model->reset();
    }

    // add data to models
    for (const QJsonValue item : states) {
        Entity *entity = new Entity(item.toObject());

        switch (entity->type()) {
        case Entity::Alarm:
            m_models.value(AlarmsModel)->addEntity(entity);
            break;

        case Entity::Automation:
            m_models.value(AutomationsModel)->addEntity(entity);
            break;

        case Entity::Camera:
            m_models.value(CamerasModel)->addEntity(entity);
            break;


        case Entity::Climate:
            m_models.value(ClimatesModel)->addEntity(entity);
            break;

        case Entity::Light:
            m_models.value(LightsModel)->addEntity(entity);
            break;

        case Entity::Person:
            m_models.value(PersonsModel)->addEntity(entity);
            break;

        case Entity::Sensor:
        case Entity::BinarySensor:
            m_models.value(SensorsModel)->addEntity(entity);
            break;

        case Entity::Switch:
            m_models.value(SwitchesModel)->addEntity(entity);
            break;

        default:
            entity->deleteLater();
            break;
        }
    }

    setLoading(false);
}

void EntitiesProvider::registerModel(const ModelType &type, EntitiesModel *model)
{
    connect(this, &EntitiesProvider::loadingChanged, model, &EntitiesModel::setLoading);

    m_models.insert(type, model);
}
