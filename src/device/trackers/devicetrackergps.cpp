#include "devicetrackergps.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

DeviceTrackerGPS::DeviceTrackerGPS(QObject *parent) :
    DeviceTracker(parent),
    m_activity(new BackgroundActivity(this)),
    m_gps(QGeoPositionInfoSource::createDefaultSource(this))
{
    connect(m_activity, &BackgroundActivity::running, this, &DeviceTracker::updateLocation);

    m_gps->setUpdateInterval(30000);
    m_gps->startUpdates();

    m_activity->run();
}

void DeviceTrackerGPS::updateLocation()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("UPDATE GPS POSITION");
#endif

    onPositionChanged(m_gps->lastKnownPosition());

    m_activity->wait(BackgroundActivity::ThirtySeconds);
}
