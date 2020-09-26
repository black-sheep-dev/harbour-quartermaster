#ifndef DEVICETRACKER_H
#define DEVICETRACKER_H

#include <QObject>

#include <QGeoPositionInfo>
#include <QJsonObject>

#include <mdm-batteryinfo.h>

class DeviceTracker : public QObject
{
    Q_OBJECT

public:
    explicit DeviceTracker(QObject *parent = nullptr);

signals:
    void locationUpdated(const QJsonObject &location);

public slots:
    void onPositionChanged(const QGeoPositionInfo &info);

private:
    Sailfish::Mdm::BatteryInfo m_battery;
    QGeoPositionInfo m_lastPosition;

    // virtual
public slots:
    virtual void updateLocation();
};

#endif // DEVICETRACKER_H
