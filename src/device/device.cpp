#include "device.h"

#include <mdm-sysinfo.h>

Device::Device(QObject *parent) :
    QObject(parent),
    m_name(Sailfish::Mdm::SysInfo::productName())
{
    connect(&m_battery, &Sailfish::Mdm::BatteryInfo::chargePercentageChanged, this, [=](int percentage) {
        QJsonObject sensor;
        sensor.insert(QStringLiteral("icon"), batteryIcon(percentage));
        sensor.insert(QStringLiteral("state"), percentage);
        sensor.insert(QStringLiteral("type"), QStringLiteral("sensor"));
        sensor.insert(QStringLiteral("unique_id"), QStringLiteral("battery_state"));

        emit sensorUpdated(sensor);
    });
    connect(&m_battery, &Sailfish::Mdm::BatteryInfo::chargerStatusChanged, this, [=](Sailfish::Mdm::BatteryInfo::ChargerStatus status) {
        QJsonObject sensor;
        sensor.insert(QStringLiteral("icon"),
                      status == Sailfish::Mdm::BatteryInfo::Connected ? QStringLiteral("mdi:battery-charging") : batteryIcon(m_battery.chargePercentage()));
        sensor.insert(QStringLiteral("state"), m_battery.chargerStatus() == Sailfish::Mdm::BatteryInfo::Connected);
        sensor.insert(QStringLiteral("type"), QStringLiteral("binary_sensor"));
        sensor.insert(QStringLiteral("unique_id"), QStringLiteral("battery_charging"));

        emit sensorUpdated(sensor);
    });
}

int Device::batteryChargePercentage() const
{
    return m_battery.chargePercentage();
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

QJsonArray Device::sensors() const
{
    QJsonArray sensors;

    // battery state
    QJsonObject batteryState;
    batteryState.insert(QStringLiteral("device_class"), QStringLiteral("battery"));
    batteryState.insert(QStringLiteral("icon"), batteryIcon(m_battery.chargePercentage()));
    batteryState.insert(QStringLiteral("name"), QStringLiteral("Battery State"));
    batteryState.insert(QStringLiteral("state"), m_battery.chargePercentage());
    batteryState.insert(QStringLiteral("type"), QStringLiteral("sensor"));
    batteryState.insert(QStringLiteral("unique_id"), QStringLiteral("battery_state"));
    batteryState.insert(QStringLiteral("unit_of_measurement"), QStringLiteral("%"));
    sensors.append(batteryState);

    // battery charging
    QJsonObject batteryCharging;
    batteryCharging.insert(QStringLiteral("device_class"), QStringLiteral("battery_charging"));
    batteryCharging.insert(QStringLiteral("icon"),
                           m_battery.chargerStatus() == Sailfish::Mdm::BatteryInfo::Connected ? QStringLiteral("mdi:battery-charging") : batteryIcon(m_battery.chargePercentage()));
    batteryCharging.insert(QStringLiteral("name"), QStringLiteral("Battery Charging"));
    batteryCharging.insert(QStringLiteral("state"), m_battery.chargerStatus() == Sailfish::Mdm::BatteryInfo::Connected);
    batteryCharging.insert(QStringLiteral("type"), QStringLiteral("binary_sensor"));
    batteryCharging.insert(QStringLiteral("unique_id"), QStringLiteral("battery_charging"));
    batteryCharging.insert(QStringLiteral("unit_of_measurement"), QString());
    sensors.append(batteryCharging);

    return sensors;
}

QString Device::name() const
{
    return m_name;
}

void Device::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

QString Device::batteryIcon(int percentage) const
{
    if (percentage >= 5 && percentage < 15) {
        return QStringLiteral("mdi:battery-10");
    } else if (percentage >= 15 && percentage < 25) {
        return QStringLiteral("mdi:battery-20");
    } else if (percentage >= 25 && percentage < 35) {
        return QStringLiteral("mdi:battery-30");
    } else if (percentage >= 35 && percentage < 45) {
        return QStringLiteral("mdi:battery-40");
    } else if (percentage >= 45 && percentage < 55) {
        return QStringLiteral("mdi:battery-50");
    } else if (percentage >= 55 && percentage < 65) {
        return QStringLiteral("mdi:battery-60");
    } else if (percentage >= 65 && percentage < 75) {
        return QStringLiteral("mdi:battery-70");
    } else if (percentage >= 75 && percentage < 85) {
        return QStringLiteral("mdi:battery-80");
    } else if (percentage >= 85 && percentage < 95) {
        return QStringLiteral("mdi:battery-90");
    } else if (percentage >= 95) {
        return QStringLiteral("mdi:battery");
    } else {
        return QStringLiteral("mdi:battery-alert");
    }
}
