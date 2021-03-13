#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonObject>

#include "devicesensormodel.h"

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool sensorAutoUpdate READ sensorAutoUpdate WRITE setSensorAutoUpdate NOTIFY sensorAutoUpdateChanged)

public:
    explicit Device(QObject *parent = nullptr);

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
    bool sensorAutoUpdate() const;

signals:
    void locationUpdated(const QJsonObject &position);
    void sensorUpdated(const QJsonObject &sensor);
    void updateZonesRequested();

    // properties
    void nameChanged(const QString &name);
    void sensorAutoUpdateChanged(bool enabled);


public slots:
    Q_INVOKABLE void update();
    Q_INVOKABLE void updateZones();

    // properties
    void setName(const QString &name);
    void setSensorAutoUpdate(bool enable);

private:
    void registerSensor(DeviceSensor *sensor);

    DeviceSensorModel *m_sensorModel{new DeviceSensorModel(this)};

    // properties
    QString m_name;
    bool m_sensorAutoUpdate{false};
};

#endif // DEVICE_H
