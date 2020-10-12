#ifndef ENTITIESPROVIDER_H
#define ENTITIESPROVIDER_H

#include <QObject>

#include <QMap>
#include <QJsonArray>

#include "src/api/homeassistantapi.h"
#include "src/models/entitiesmodel.h"
#include "src/models/entitytypesmodel.h"

#include "src/entities/climate.h"
#include "src/entities/light.h"

class EntitiesProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    explicit EntitiesProvider(QObject *parent = nullptr);

    // api
    void setApi(HomeassistantApi *api);
    Q_INVOKABLE void callService(const QString &domain, const QString &service, const QString &entityId, const QJsonObject &data = QJsonObject());


    // models
    Q_INVOKABLE EntitiesModel *model(const int &type);
    Q_INVOKABLE EntityTypesModel *typesModel();

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
    void addEntityToModel(const Entity::EntityType &type, Entity *entity);
    Entity::EntityType getEntityType(const QString &entityId) const;
    void parseStates(const QJsonArray &states);
    void registerModel(const Entity::EntityType &entityType);
    void updateEntities(const QJsonArray &entities);

    HomeassistantApi *m_api{nullptr};
    QMap<Entity::EntityType, EntitiesModel *> m_models;
    EntityTypesModel *m_typesModel{nullptr};

    // properties
    bool m_loading{false};
};

#endif // ENTITIESPROVIDER_H
