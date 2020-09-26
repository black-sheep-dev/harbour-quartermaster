#include "devicesensorbattery.h"

DeviceSensorBattery::DeviceSensorBattery(QObject *parent) :
    DeviceSensor(parent)
{
    setName(tr("Battery State"));
    setDeviceClass(QStringLiteral("battery"));
    setSensorType(QStringLiteral("sensor"));
    setUniqueId(QStringLiteral("battery_state"));
    setUnit(QStringLiteral("%"));

    setState(m_battery.chargePercentage());

    onEnabledChanged();
}

QString DeviceSensorBattery::getIcon() const
{
    const int percentage = state().toInt();

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

void DeviceSensorBattery::onEnabledChanged()
{
    if (enabled())
        connect(&m_battery, &Sailfish::Mdm::BatteryInfo::chargePercentageChanged, this, &DeviceSensor::setState);
    else
        disconnect(&m_battery, &Sailfish::Mdm::BatteryInfo::chargePercentageChanged, this, &DeviceSensor::setState);
}
