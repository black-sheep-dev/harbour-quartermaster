#ifndef DEVICETRACKERGPS_H
#define DEVICETRACKERGPS_H

#include "../devicetracker.h"

#include <QGeoPositionInfoSource>

#include <keepalive/backgroundactivity.h>

class DeviceTrackerGPS : public DeviceTracker
{
    Q_OBJECT
public:
    explicit DeviceTrackerGPS(QObject *parent = nullptr);

private:
    BackgroundActivity *m_activity;
    QGeoPositionInfoSource *m_gps;

    // DeviceTracker interface
public slots:
    void updateLocation() override;

};

#endif // DEVICETRACKERGPS_H
