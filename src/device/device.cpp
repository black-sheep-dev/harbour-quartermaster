#include "device.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <mdm-sysinfo.h>

#include "sensors/devicesensorbattery.h"
#include "sensors/devicesensorbatterycharging.h"

Device::Device(QObject *parent) :
    QObject(parent),
    m_name(Sailfish::Mdm::SysInfo::productName())
{
    auto battery = new DeviceSensorBattery;
    registerSensor(battery);

    auto batteryCharging = new DeviceSensorBatteryCharging;
    registerSensor(batteryCharging);
}

Device::~Device()
{

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

QString Device::name() const
{
    return m_name;
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

void Device::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void Device::setSensorAutoUpdate(bool enable)
{
    if (m_sensorAutoUpdate == enable)
        return;

    m_sensorAutoUpdate = enable;
    emit sensorAutoUpdateChanged(m_sensorAutoUpdate);

    for (auto sensor : sensors()) {
        sensor->setEnabled(enable);
    }
}

void Device::registerSensor(DeviceSensor *sensor)
{
    connect(sensor, &DeviceSensor::sensorUpdated, this, &Device::sensorUpdated);

    m_sensorModel->addSensor(sensor);
}
