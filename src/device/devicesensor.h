#ifndef DEVICESENSOR_H
#define DEVICESENSOR_H

#include <QObject>

#include <QJsonObject>
#include <QVariant>

class DeviceSensor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deviceClass READ deviceClass WRITE setDeviceClass NOTIFY deviceClassChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QVariant state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString sensorType READ sensorType WRITE setSensorType NOTIFY sensorTypeChanged)
    Q_PROPERTY(QString uniqueId READ uniqueId WRITE setUniqueId NOTIFY uniqueIdChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)

public:    
    explicit DeviceSensor(QObject *parent = nullptr);

    QJsonObject toJson() const;
    QJsonObject getBaseSensorJson() const;

    // properties
    bool active() const;
    QString deviceClass() const;
    bool enabled() const;
    QString icon() const;
    QString name() const;
    QString sensorType() const;
    QVariant state() const;
    QString uniqueId() const;
    QString unit() const;

signals:
    void sensorUpdated(const QJsonObject &object);

    // properties
    void deviceClassChanged(const QString &deviceClass);
    void enabledChanged(bool enabled);
    void iconChanged(const QString &icon);
    void nameChanged(const QString &name);
    void sensorTypeChanged(const QString &sensorType);
    void stateChanged(const QVariant &state);
    void uniqueIdChanged(const QString &uniqueId);
    void unitChanged(const QString &unit);

public slots:
    // properties
    void setDeviceClass(const QString &deviceClass);
    void setEnabled(bool enable);
    void setIcon(const QString &icon);
    void setName(const QString &name);
    void setSensorType(const QString &sensorType);
    void setState(const QVariant &state);
    void setUniqueId(const QString &uniqueId);
    void setUnit(const QString &unit);

private:
    void onStateChanged();
    QJsonValue getStateValue() const;

    // properties
    QString m_deviceClass;
    bool m_enabled{false};
    QString m_icon;
    QString m_name;
    QString m_sensorType;
    QVariant m_state;
    QString m_uniqueId;
    QString m_unit;


    // virtual
public:
    virtual QString getIcon() const;
    virtual void onEnabledChanged();
};

#endif // DEVICESENSOR_H
