#include "devicesensorbatterycharging.h"

DeviceSensorBatteryCharging::DeviceSensorBatteryCharging(QObject *parent) :
    DeviceSensor(parent)
{
    setName(tr("Battery Charging"));
    setDeviceClass(QStringLiteral("battery_charging"));
    setSensorType(QStringLiteral("binary_sensor"));
    setUniqueId(QStringLiteral("battery_charging"));

    setState(int(m_battery.chargerStatus()));

    onEnabledChanged();
}

void DeviceSensorBatteryCharging::onChargerStatusChanged(BatteryStatus::ChargerStatus status)
{
    setState(int(status));
}

QString DeviceSensorBatteryCharging::getIcon() const
{
    if (state().toInt() == 1) {
        return QStringLiteral("mdi:battery-charging");
    }

    const int percentage = m_battery.chargePercentage();

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

void DeviceSensorBatteryCharging::onEnabledChanged()
{
    if (enabled()) {
        connect(&m_battery,
                &Sailfish::Mdm::BatteryInfo::chargerStatusChanged,
                this,
                &DeviceSensorBatteryCharging::onChargerStatusChanged);
    } else {
        disconnect(&m_battery,
                   &Sailfish::Mdm::BatteryInfo::chargerStatusChanged,
                   this,
                   &DeviceSensorBatteryCharging::onChargerStatusChanged);
    }
}
