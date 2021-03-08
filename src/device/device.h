#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonObject>

#include <mdm-batteryinfo.h>

#include "devicesensormodel.h"

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool registered READ registered WRITE setRegistered NOTIFY registeredChanged)
    Q_PROPERTY(bool sensorAutoUpdate READ sensorAutoUpdate WRITE setSensorAutoUpdate NOTIFY sensorAutoUpdateChanged)

public:
    explicit Device(QObject *parent = nullptr);
    ~Device() override;

    Q_INVOKABLE QString id() const;
    Q_INVOKABLE QString manufacturer() const;
    Q_INVOKABLE QString model() const;
    Q_INVOKABLE QString softwareName() const;
    Q_INVOKABLE QString softwareVersion() const;
    Q_INVOKABLE QString wlanMacAddress() const;

    // internal
    Q_INVOKABLE DeviceSensorModel *sensorModel();
    QList<DeviceSensor *> sensors() const;

    // properties
    QString name() const;
    bool registered() const;
    bool sensorAutoUpdate() const;

signals:
    void locationUpdated(const QJsonObject &position);
    void sensorUpdated(const QJsonObject &sensor);
    void updateZonesRequested();

    // properties
    void nameChanged(const QString &name);
    void registeredChanged(bool registered);
    void sensorAutoUpdateChanged(bool enabled);


public slots:
    Q_INVOKABLE void update();
    Q_INVOKABLE void updateZones();

    // properties
    void setName(const QString &name);
    void setRegistered(bool registered);
    void setSensorAutoUpdate(bool enable);

private:
    void registerSensor(DeviceSensor *sensor);

    // settings
    void readSettings();
    void writeSettings();

    DeviceSensorModel *m_sensorModel{new DeviceSensorModel(this)};

    // properties
    QString m_name;
    bool m_registered{false};
    bool m_sensorAutoUpdate{false};
};

#endif // DEVICE_H
