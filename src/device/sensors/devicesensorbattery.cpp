#include "devicesensorbattery.h"

#include "src/constants.h"

DeviceSensorBattery::DeviceSensorBattery(QObject *parent) :
    DeviceSensor(parent)
{
    //% "Battery State"
    setName(qtTrId("id-battery-state"));
    setDeviceClass(ApiKey::KEY_BATTERY);
    setSensorType(ApiKey::KEY_SENSOR);
    setUniqueId(ApiKey::KEY_BATTERY_STATE);
    setUnit(QStringLiteral("%"));

    setState(m_battery.chargePercentage());

    onEnabledChanged();
}

QString DeviceSensorBattery::getIcon() const
{
    const int percentage = state().toInt();

    QString icon;
    icon.reserve(18);

    if (percentage >= 5 && percentage < 15) {
        icon = QStringLiteral("mdi:battery-10");
    } else if (percentage >= 15 && percentage < 25) {
        icon = QStringLiteral("mdi:battery-20");
    } else if (percentage >= 25 && percentage < 35) {
        icon = QStringLiteral("mdi:battery-30");
    } else if (percentage >= 35 && percentage < 45) {
        icon = QStringLiteral("mdi:battery-40");
    } else if (percentage >= 45 && percentage < 55) {
        icon = QStringLiteral("mdi:battery-50");
    } else if (percentage >= 55 && percentage < 65) {
        icon = QStringLiteral("mdi:battery-60");
    } else if (percentage >= 65 && percentage < 75) {
        icon = QStringLiteral("mdi:battery-70");
    } else if (percentage >= 75 && percentage < 85) {
        icon = QStringLiteral("mdi:battery-80");
    } else if (percentage >= 85 && percentage < 95) {
        icon = QStringLiteral("mdi:battery-90");
    } else if (percentage >= 95) {
        icon = QStringLiteral("mdi:battery");
    } else {
        icon = QStringLiteral("mdi:battery-alert");
    }

    return icon;
}

void DeviceSensorBattery::onEnabledChanged()
{
    if (enabled())
        connect(&m_battery, &Sailfish::Mdm::BatteryInfo::chargePercentageChanged, this, &DeviceSensor::setState);
    else
        disconnect(&m_battery, &Sailfish::Mdm::BatteryInfo::chargePercentageChanged, this, &DeviceSensor::setState);
}
