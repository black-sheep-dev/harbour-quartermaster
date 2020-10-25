#include "devicetracker.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>

DeviceTracker::DeviceTracker(QObject *parent) :
    QObject(parent)
{

}

void DeviceTracker::onPositionChanged(const QGeoPositionInfo &info)
{
#ifdef QT_DEBUG
    qDebug() << info.isValid();
#endif

    if (!info.isValid())
        return;

    if (m_lastPosition == info)
        return;

    QJsonArray position;
    position.append(info.coordinate().latitude());
    position.append(info.coordinate().longitude());


    QJsonObject location;

    location.insert(QStringLiteral("gps"), position);
    location.insert(QStringLiteral("altitude"), int(info.coordinate().altitude()));
    location.insert(QStringLiteral("battery"), m_battery.chargePercentage() > 0 ? m_battery.chargePercentage() : 0);

    if (info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
        location.insert(QStringLiteral("gps_accuracy"), int(info.attribute(QGeoPositionInfo::HorizontalAccuracy)));

    if (info.hasAttribute(QGeoPositionInfo::VerticalAccuracy))
        location.insert(QStringLiteral("vertical_accuracy"), int(info.attribute(QGeoPositionInfo::VerticalAccuracy)));

    if (info.hasAttribute(QGeoPositionInfo::GroundSpeed))
        location.insert(QStringLiteral("speed"), int(info.attribute(QGeoPositionInfo::GroundSpeed)));

    if (info.hasAttribute(QGeoPositionInfo::Direction))
        location.insert(QStringLiteral("course"), int(info.attribute(QGeoPositionInfo::Direction)));


    m_lastPosition = info;

    emit locationUpdated(location);
}

void DeviceTracker::reloadConfig()
{

}

void DeviceTracker::updateLocation()
{

}
