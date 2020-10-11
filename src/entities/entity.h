#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>

#include <QJsonObject>
#include <QVariant>
#include <QVariantMap>

class Entity : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap attributes READ attributes WRITE setAttributes NOTIFY attributesChanged)
    Q_PROPERTY(QVariantMap context READ context WRITE setContext NOTIFY contextChanged)
    Q_PROPERTY(QString entityId READ entityId WRITE setEntityId NOTIFY entityIdChanged)
    Q_PROPERTY(int features READ features WRITE setFeatures NOTIFY featuresChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariant state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(EntityType type READ type WRITE setType NOTIFY typeChanged)

public:
    enum EntityType {
        Unkown,
        Alarm,
        Automation,
        BinarySensor,
        Camera,
        Climate,
        DeviceTracker,
        Group,
        Light,
        MediaPlayer,
        Person,
        Sensor,
        Sun,
        Switch,
        Weather,
        Zone
    };
    Q_ENUM(EntityType)

    enum LightFeature {
        LightNone               = 0x00,
        LightBrightness         = 0x01,
        LightColor              = 0x02,
        LightColorTemp          = 0x04,
        LightEffect             = 0x08,
        LightFlash              = 0x10,
        LightTransition         = 0x20,
        LightWhiteValue         = 0x80
    };
    Q_ENUM(LightFeature)
    Q_DECLARE_FLAGS(LightFeatures, LightFeature)

    explicit Entity(QObject *parent = nullptr);
    explicit Entity(const Entity &other);
    explicit Entity(const QJsonObject &json, QObject *parent = nullptr);
    ~Entity() override;

    void setJson(const QJsonObject &json);

    // properties
    QVariantMap attributes() const;
    QVariantMap context() const;
    QString entityId() const;
    int features() const;
    QString name() const;
    QVariant state() const;
    EntityType type() const;

signals:
    // properties
    void attributesChanged(const QVariantMap &attributes);
    void contextChanged(const QVariantMap &context);
    void entityIdChanged(const QString &id);
    void featuresChanged(int features);
    void nameChanged(const QString &name);
    void stateChanged(const QVariant &state);
    void typeChanged(Entity::EntityType type);

public slots:
    // properties
    void setAttributes(const QVariantMap &attributes);
    void setContext(const QVariantMap &context);
    void setEntityId(const QString &id);
    void setFeatures(int features);
    void setName(const QString &name);
    void setState(const QVariant &state);
    void setType(Entity::EntityType type);

private:
    // properties
    QVariantMap m_attributes;
    QVariantMap m_context;
    QString m_entityId;
    int m_features{0};
    QString m_name;
    QVariant m_state;
    Entity::EntityType m_type{Unkown};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Entity::LightFeatures)

#endif // ENTITY_H
