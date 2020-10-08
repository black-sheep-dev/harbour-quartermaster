#ifndef ENTITIESPROVIDER_H
#define ENTITIESPROVIDER_H

#include <QObject>

#include <QMap>
#include <QJsonArray>

#include "src/api/homeassistantapi.h"
#include "src/models/entitiesmodel.h"

class EntitiesProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    enum ModelType {
        UnknownModel,
        AlarmsModel,
        AutomationsModel,
        CamerasModel,
        ClimatesModel,
        LightsModel,
        PersonsModel,
        SensorsModel,
        SwitchesModel
    };
    Q_ENUM(ModelType)

    explicit EntitiesProvider(QObject *parent = nullptr);

    // api
    void setApi(HomeassistantApi *api);

    // models
    Q_INVOKABLE EntitiesModel *model(const ModelType &type);

    // functions

    // properties
    bool loading() const;

signals:
    // properties
    void loadingChanged(bool loading);

public slots:
    Q_INVOKABLE void refresh();

    // properties
    void setLoading(bool loading);

private slots:
    void onDataAvailable(const QString &endpoint, const QJsonDocument &doc);

private:
    void parseStates(const QJsonArray &states);
    void registerModel(const ModelType &type, EntitiesModel *model);

    HomeassistantApi *m_api{nullptr};
    QMap<EntitiesProvider::ModelType, EntitiesModel *> m_models;

    // properties
    bool m_loading{false};
};

#endif // ENTITIESPROVIDER_H
