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
    m_name(Sailfish::Mdm::SysInfo::productName())
{
    auto battery = new DeviceSensorBattery;
    registerSensor(battery);

    auto batteryCharging = new DeviceSensorBatteryCharging;
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

void Device::update()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("UPDATE DEVICE LOCATION");
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

    for (auto &sensors : sensors()) {
        sensors->setEnabled(enable);
    }

    writeSettings();
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
}

void Device::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("SENSORS"));
    settings.setValue(QStringLiteral("autoupdate"), m_sensorAutoUpdate);
    settings.endGroup();
}
