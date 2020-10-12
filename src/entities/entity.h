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
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariant state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(quint16 supportedFeatures READ supportedFeatures WRITE setSupportedFeatures NOTIFY supportedFeaturesChanged)
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

    explicit Entity(QObject *parent = nullptr);
    explicit Entity(const Entity &other);
    ~Entity() override;

    Q_INVOKABLE bool hasFeature(const quint16 feature) const;

    // properties
    QVariantMap attributes() const;
    QVariantMap context() const;
    QString entityId() const;
    QString name() const;
    QVariant state() const;
    quint16 supportedFeatures() const;
    EntityType type() const;

signals:
    // properties
    void attributesChanged(const QVariantMap &attributes);
    void contextChanged(const QVariantMap &context);
    void entityIdChanged(const QString &id);
    void nameChanged(const QString &name);
    void stateChanged(const QVariant &state);
    void supportedFeaturesChanged(quint16 supportedFeatures);
    void typeChanged(Entity::EntityType type);  

public slots:
    // properties
    void setAttributes(const QVariantMap &attributes);
    void setContext(const QVariantMap &context);
    void setEntityId(const QString &id);
    void setName(const QString &name);
    void setState(const QVariant &state);
    void setSupportedFeatures(quint16 supportedFeatures);
    void setType(Entity::EntityType type);

private:
    // properties
    QVariantMap m_attributes;
    QVariantMap m_context;
    QString m_entityId;
    QString m_name;
    QVariant m_state;
    quint16 m_supportedFeatures{0};
    Entity::EntityType m_type{Unkown};

    // virtual functions
public:
    virtual void parseAttributes();
};

#endif // ENTITY_H
