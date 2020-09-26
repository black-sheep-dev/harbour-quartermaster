#ifndef DEVICESENSORBATTERYCHARGING_H
#define DEVICESENSORBATTERYCHARGING_H

#include "../devicesensor.h"

#include "mdm-batteryinfo.h"

class DeviceSensorBatteryCharging : public DeviceSensor
{
    Q_OBJECT
public:
    explicit DeviceSensorBatteryCharging(QObject *parent = nullptr);

private slots:
    void onChargerStatusChanged(Sailfish::Mdm::BatteryInfo::ChargerStatus status);

private:
    Sailfish::Mdm::BatteryInfo m_battery;

    // DeviceSensor interface
public:
    QString getIcon() const override;
    void onEnabledChanged() override;
};

#endif // DEVICESENSORBATTERYCHARGING_H
