#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>

#include <QJsonObject>
#include <QVariant>
#include <QVariantMap>

struct EntityContext {
    Q_INVOKABLE QString id;
    Q_INVOKABLE QString parentId;
    Q_INVOKABLE QString userId;

    bool operator==(const EntityContext &context) const {
        return context.id == id && context.parentId == parentId && context.userId == userId;
    }

    bool operator!=(const EntityContext &context) const  {
        return !(context.id == id && context.parentId == parentId && context.userId == userId);
    }
};
Q_DECLARE_METATYPE(EntityContext)

class Entity : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantMap attributes READ attributes WRITE setAttributes NOTIFY attributesChanged)
    Q_PROPERTY(EntityContext context READ context WRITE setContext NOTIFY contextChanged)
    Q_PROPERTY(QString enityId READ enityId WRITE setEnityId NOTIFY enityIdChanged)
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
        Light,
        MediaPlayer,
        Person,
        Sensor,
        Sun,
        Switch,
        Weather,
        Zone
    };
    Q_ENUMS(EntityType)

    explicit Entity(QObject *parent = nullptr);
    Entity(const QJsonObject &json, QObject *parent = nullptr);

    void setJson(const QJsonObject &json);

    // properties
    QVariantMap attributes() const;
    EntityContext context() const;
    QString enityId() const;
    QString name() const;
    QVariant state() const;
    EntityType type() const;

signals:
    // properties
    void attributesChanged(const QVariantMap &attributes);
    void contextChanged(const EntityContext &context);
    void enityIdChanged(const QString &id);
    void nameChanged(const QString &name);
    void stateChanged(const QVariant &state);
    void typeChanged(EntityType type);

public slots:
    // properties
    void setAttributes(const QVariantMap &attributes);
    void setContext(const EntityContext &context);
    void setEnityId(const QString &id);
    void setName(const QString &name);
    void setState(const QVariant &state);
    void setType(EntityType type);

private:
    // properties
    QVariantMap m_attributes;
    EntityContext m_context;
    QString m_enityId;
    QString m_name;
    QVariant m_state;
    EntityType m_type;
};

#endif // ENTITY_H
