#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonObject>

#include <mdm-batteryinfo.h>

#include "devicesensormodel.h"
#include "trackers/devicetrackergps.h"
#include "src/entities/entitymodel.h"

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool encryption READ encryption WRITE setEncryption NOTIFY encryptionChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool registered READ registered WRITE setRegistered NOTIFY registeredChanged)
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
    Q_INVOKABLE EntityModel *zonesModel();

    // properties
    bool encryption() const;
    QString name() const;
    bool registered() const;
    bool sensorAutoUpdate() const;
    bool trackingGPS() const;
    bool trackingWifi() const;

signals:
    void locationUpdated(const QJsonObject &position);
    void sensorUpdated(const QJsonObject &sensor);
    void updateZonesRequested();

    // properties
    void encryptionChanged(bool enabled);
    void nameChanged(const QString &name);
    void sensorAutoUpdateChanged(bool enabled);
    void trackingGPSChanged(bool enabled);
    void trackingWifiChanged(bool enabled);

    void registeredChanged(bool registered);

public slots:
    Q_INVOKABLE void update();
    Q_INVOKABLE void updateZones();

    // properties
    void setEncryption(bool enable);
    void setName(const QString &name);
    void setRegistered(bool registered);
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
    EntityModel *m_zonesModel;

    // properties
    bool m_encryption;
    QString m_name;
    bool m_registered;
    bool m_sensorAutoUpdate;
    bool m_trackingGPS;
    bool m_trackingWifi;
};

#endif // DEVICE_H
