#include "device.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QSettings>

#include <mdm-sysinfo.h>

#include "sensors/devicesensorbattery.h"
#include "sensors/devicesensorbatterycharging.h"

Device::Device(QObject *parent) :
    QObject(parent),
    m_sensorModel(new DeviceSensorModel(this)),
    m_trackerGPS(nullptr),
    m_zonesModel(new EntityModel(this)),
    m_encryption(false),
    m_name(Sailfish::Mdm::SysInfo::productName()),
    m_registered(false),
    m_sensorAutoUpdate(false)

{
    DeviceSensorBattery *battery = new DeviceSensorBattery;
    registerSensor(battery);

    DeviceSensorBatteryCharging *batteryCharging = new DeviceSensorBatteryCharging;
    registerSensor(batteryCharging);

    readSettings();
}

Device::~Device()
{
    writeSettings();
}

QString Device::id() const
{
    return Sailfish::Mdm::SysInfo::deviceUid();
}

QString Device::manufacturer() const
{
    return Sailfish::Mdm::SysInfo::manufacturer();
}

QString Device::model() const
{
    return Sailfish::Mdm::SysInfo::deviceModel();
}

QString Device::softwareName() const
{
    return QStringLiteral("Sailfish OS");
}

QString Device::softwareVersion() const
{
    return Sailfish::Mdm::SysInfo::softwareVersion();
}

QString Device::wlanMacAddress() const
{
    return Sailfish::Mdm::SysInfo::wlanMacAddress();
}

DeviceSensorModel *Device::sensorModel()
{
    return m_sensorModel;
}

QList<DeviceSensor *> Device::sensors() const
{
    return m_sensorModel->sensors();
}

DeviceTracker *Device::trackerGPS()
{
    return m_trackerGPS;
}

EntityModel *Device::zonesModel()
{
    return m_zonesModel;
}

bool Device::encryption() const
{
    return m_encryption;
}

QString Device::name() const
{
    return m_name;
}

bool Device::registered() const
{
    return m_registered;
}

bool Device::sensorAutoUpdate() const
{
    return m_sensorAutoUpdate;
}

bool Device::trackingGPS() const
{
    return m_trackingGPS;
}

bool Device::trackingWifi() const
{
    return m_trackingWifi;
}

void Device::update()
{
#ifdef QT_DEBUG
    qDebug() << "UPDATE DEVICE LOCATION";
#endif
}

void Device::updateZones()
{
    emit updateZonesRequested();
}

void Device::setEncryption(bool enable)
{
    if (m_encryption == enable)
        return;

    m_encryption = enable;
    emit encryptionChanged(m_encryption);
}

void Device::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void Device::setRegistered(bool registered)
{
    if (m_registered == registered)
        return;

    m_registered = registered;
    emit registeredChanged(m_registered);
}

void Device::setSensorAutoUpdate(bool enable)
{
    if (m_sensorAutoUpdate == enable)
        return;

    m_sensorAutoUpdate = enable;
    emit sensorAutoUpdateChanged(m_sensorAutoUpdate);

    for (DeviceSensor *sensors : sensors()) {
        sensors->setEnabled(enable);
    }

    writeSettings();
}

void Device::setTrackingGPS(bool enable)
{
    if (m_trackingGPS == enable)
        return;

    m_trackingGPS = enable;
    emit trackingGPSChanged(m_trackingGPS);

    // enable / disable tracker

    if (enable) {
        m_trackerGPS = new DeviceTrackerGPS(this);
        connect(m_trackerGPS, &DeviceTracker::locationUpdated, this, &Device::locationUpdated);
    } else {
        m_trackerGPS->deleteLater();
        m_trackerGPS = nullptr;
    }
}

void Device::setTrackingWifi(bool enable)
{
    if (m_trackingWifi == enable)
        return;

    m_trackingWifi = enable;
    emit trackingWifiChanged(m_trackingWifi);
}

void Device::registerSensor(DeviceSensor *sensor)
{
    connect(sensor, &DeviceSensor::sensorUpdated, this, &Device::sensorUpdated);

    m_sensorModel->addSensor(sensor);
}

void Device::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("SENSORS"));
    setSensorAutoUpdate(settings.value(QStringLiteral("autoupdate"), false).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("TRACKING"));
    setTrackingGPS(settings.value(QStringLiteral("gps"), false).toBool());
    setTrackingWifi(settings.value(QStringLiteral("wifi"), false).toBool());
    settings.endGroup();
}

void Device::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("SENSORS"));
    settings.setValue(QStringLiteral("autoupdate"), m_sensorAutoUpdate);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("TRACKING"));
    settings.setValue(QStringLiteral("gps"), m_trackingGPS);
    settings.setValue(QStringLiteral("wifi"), m_trackingWifi);
    settings.endGroup();
}
