#ifndef DEVICESENSORBATTERY_H
#define DEVICESENSORBATTERY_H

#include "../devicesensor.h"

#include "mdm-batteryinfo.h"

class DeviceSensorBattery : public DeviceSensor
{
    Q_OBJECT
public:
    explicit DeviceSensorBattery(QObject *parent = nullptr);

private:
    Sailfish::Mdm::BatteryInfo m_battery;

    // DeviceSensor interface
public:
    QString getIcon() const override;
    void onEnabledChanged() override;
};

#endif // DEVICESENSORBATTERY_H
