#include "locationtracker.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>
#include <QJsonObject>

#include "src/constants.h"

LocationTracker::LocationTracker(QObject *parent) :
    QObject(parent)
{

}

LocationTracker::~LocationTracker()
{

}

WifiNetworkModel *LocationTracker::networksModel(const QString &zoneGuid)
{
    auto model = new WifiNetworkModel();
    model->setNetworks(m_networks.values(zoneGuid));

    return model;
}

void LocationTracker::storeNetworkModel(const QString &zoneGuid, WifiNetworkModel *model)
{

}

ZonesModel *LocationTracker::zonesModel()
{
    return m_zonesModel;
}

bool LocationTracker::enableGPS() const
{
    return m_enableGPS;
}

bool LocationTracker::enableWifi() const
{
    return m_enableWifi;
}

double LocationTracker::homezoneLatitude() const
{
    return m_homezoneLatitude;
}

double LocationTracker::homezoneLongitude() const
{
    return m_homezoneLongitude;
}

void LocationTracker::setZones(const QJsonArray &arr)
{
    QList<Zone *> zones;

    for (const auto &item : arr) {
        const auto obj = item.toObject();
        const auto attributes = obj.value(ApiKey::KEY_ATTRIBUTES).toObject();
        const auto context = obj.value(ApiKey::KEY_CONTEXT).toObject();

        auto zone = new Zone;
        zone->setGuid(context.value(ApiKey::KEY_ID).toString());
        zone->setName(attributes.value(ApiKey::KEY_FRIENDLY_NAME).toString());
        zone->setLatitude(attributes.value(ApiKey::KEY_LATITUDE).toDouble());
        zone->setLongitude(attributes.value(ApiKey::KEY_LONGITUDE).toDouble());
        zone->setRadius(attributes.value(ApiKey::KEY_RADIUS).toDouble());

        zones.append(zone);
    }

    m_zonesModel->setZones(zones);
    updateHomezone();
}

void LocationTracker::setEnableGPS(bool enable)
{
    if (m_enableGPS == enable)
        return;

    m_enableGPS = enable;
    emit enableGPSChanged(m_enableGPS);
}

void LocationTracker::setEnableWifi(bool enable)
{
    if (m_enableWifi == enable)
        return;

    m_enableWifi = enable;
    emit enableWifiChanged(m_enableWifi);
}

void LocationTracker::setHomezoneLatitude(double latitude)
{
    if (qFuzzyCompare(m_homezoneLatitude, latitude))
        return;

    m_homezoneLatitude = latitude;
    emit homezoneLatitudeChanged(m_homezoneLatitude);

    updateHomezone();
}

void LocationTracker::setHomezoneLongitude(double longitude)
{
    if (qFuzzyCompare(m_homezoneLongitude, longitude))
        return;

    m_homezoneLongitude = longitude;
    emit homezoneLongitudeChanged(m_homezoneLongitude);

    updateHomezone();
}

void LocationTracker::updateHomezone()
{
    if ( qFuzzyCompare(m_homezoneLatitude, 0)
         || qFuzzyCompare(m_homezoneLongitude, 0))
        return;

    const QGeoCoordinate home(m_homezoneLatitude, m_homezoneLongitude);

    for (auto zone : m_zonesModel->zones()) {
        const QGeoCoordinate loc(zone->latitude(), zone->longitude());
        zone->setIsHome(loc.distanceTo(home) < zone->radius());
    }
}
