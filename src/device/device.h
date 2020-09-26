#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonObject>

#include <mdm-batteryinfo.h>

#include "devicesensormodel.h"
#include "trackers/devicetrackergps.h"

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool sensorAutoUpdate READ sensorAutoUpdate WRITE setSensorAutoUpdate NOTIFY sensorAutoUpdateChanged)
    Q_PROPERTY(bool trackingGPS READ trackingGPS WRITE setTrackingGPS NOTIFY trackingGPSChanged)
    Q_PROPERTY(bool trackingWifi READ trackingWifi WRITE setTrackingWifi NOTIFY trackingWifiChanged)

public:
    explicit Device(QObject *parent = nullptr);
    ~Device();

    Q_INVOKABLE QString id() const;
    Q_INVOKABLE QString manufacturer() const;
    Q_INVOKABLE QString model() const;
    Q_INVOKABLE QString softwareName() const;
    Q_INVOKABLE QString softwareVersion() const;
    Q_INVOKABLE QString wlanMacAddress() const;

    // internal
    Q_INVOKABLE DeviceSensorModel *sensorModel();
    QList<DeviceSensor *> sensors() const;
    Q_INVOKABLE DeviceTracker *trackerGPS();

    // properties
    QString name() const;
    bool sensorAutoUpdate() const;
    bool trackingGPS() const;
    bool trackingWifi() const;

signals:
    void locationUpdated(const QJsonObject &position);
    void sensorUpdated(const QJsonObject &sensor);

    // properties
    void nameChanged(const QString &name);
    void sensorAutoUpdateChanged(bool enabled);
    void trackingGPSChanged(bool enabled);
    void trackingWifiChanged(bool enabled);

public slots:
    Q_INVOKABLE void update();

    // properties
    void setName(const QString &name);
    void setSensorAutoUpdate(bool enable);
    void setTrackingGPS(bool enable);
    void setTrackingWifi(bool enable);

private:
    void registerSensor(DeviceSensor *sensor);

    // settings
    void readSettings();
    void writeSettings();

    DeviceSensorModel *m_sensorModel;
    DeviceTrackerGPS *m_trackerGPS;

    // properties
    QString m_name;
    bool m_sensorAutoUpdate;
    bool m_trackingGPS;
    bool m_trackingWifi;
};

#endif // DEVICE_H
