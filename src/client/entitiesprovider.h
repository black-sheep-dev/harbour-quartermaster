#ifndef ENTITIESPROVIDER_H
#define ENTITIESPROVIDER_H

#include <QObject>

#include <QMap>
#include <QJsonArray>

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
        ClimatesModel,
        LightsModel,
        PersonsModel,
        SensorsModel,
        SwitchesModel
    };
    Q_ENUM(ModelType)

    explicit EntitiesProvider(QObject *parent = nullptr);

    // models
    Q_INVOKABLE EntitiesModel *model(const ModelType &type);

    // functions
    void parseStates(const QJsonArray &states);

    // properties
    bool loading() const;

signals:
    // properties
    void loadingChanged(bool loading);

public slots:
    // properties
    void setLoading(bool loading);

private:
    void registerModel(const ModelType &type, EntitiesModel *model);

    QMap<EntitiesProvider::ModelType, EntitiesModel *> m_models;

    // properties
    bool m_loading{false};
};

#endif // ENTITIESPROVIDER_H
