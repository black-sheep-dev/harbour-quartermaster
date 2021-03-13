#include "locationservice.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>
#include <QJsonObject>

#include "src/constants.h"

#include "src/api/api.h"

LocationService::LocationService(QObject *parent) :
    QObject(parent)
{

}

void LocationService::initialize()
{
    connect(this, &LocationService::settingsChanged, this, &LocationService::updateTrackers);
    updateTrackers();
}

Zone *LocationService::homezone()
{
    return m_homezone;
}

WifiNetworkModel *LocationService::localNetworksModel()
{
    return m_localNetworksModel;
}

WifiNetworkModel *LocationService::networksModel(const QString &zoneGuid)
{
    auto model = new WifiNetworkModel();
    model->setNetworks(m_networks.values(zoneGuid));

    return model;
}

void LocationService::storeNetworkModel(const QString &zoneGuid, WifiNetworkModel *model)
{

}

ZonesModel *LocationService::zonesModel()
{
    return m_zonesModel;
}

bool LocationService::atHome() const
{
    return m_atHome;
}

bool LocationService::disableGpsAtHome() const
{
    return m_disableGpsAtHome;
}

bool LocationService::enableGps() const
{
    return m_enableGps;
}

bool LocationService::enableWifi() const
{
    return m_enableWifi;
}

double LocationService::homezoneLatitude() const
{
    return m_homezoneLatitude;
}

double LocationService::homezoneLongitude() const
{
    return m_homezoneLongitude;
}

quint32 LocationService::updateInterval() const
{
    return m_updateInterval;
}

void LocationService::setZones(const QJsonArray &arr)
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

void LocationService::setAtHome(bool atHome)
{
    if (m_atHome == atHome)
        return;

    m_atHome = atHome;
    emit atHomeChanged(m_atHome);
    emit settingsChanged();
}

void LocationService::setDisableGpsAtHome(bool disable)
{
    if (m_disableGpsAtHome == disable)
        return;

    m_disableGpsAtHome = disable;
    emit disableGpsAtHomeChanged(m_disableGpsAtHome);
    emit settingsChanged();
}

void LocationService::setEnableGps(bool enable)
{
    if (m_enableGps == enable)
        return;

    m_enableGps = enable;
    emit enableGpsChanged(m_enableGps);
    emit settingsChanged();
}

void LocationService::setEnableWifi(bool enable)
{
    if (m_enableWifi == enable)
        return;

    m_enableWifi = enable;
    emit enableWifiChanged(m_enableWifi);
    emit settingsChanged();
}

void LocationService::setHomezoneLatitude(double latitude)
{
    if (qFuzzyCompare(m_homezoneLatitude, latitude))
        return;

    m_homezoneLatitude = latitude;
    emit homezoneLatitudeChanged(m_homezoneLatitude);

    updateHomezone();
}

void LocationService::setHomezoneLongitude(double longitude)
{
    if (qFuzzyCompare(m_homezoneLongitude, longitude))
        return;

    m_homezoneLongitude = longitude;
    emit homezoneLongitudeChanged(m_homezoneLongitude);

    updateHomezone();
}

void LocationService::setUpdateInterval(quint32 updateInterval)
{
    if (m_updateInterval == updateInterval)
        return;

    m_updateInterval = updateInterval;
    emit updateIntervalChanged(m_updateInterval);

    // apply update interval
    if (m_gps != nullptr)
        m_gps->setUpdateInterval(m_updateInterval);
}

void LocationService::onNetworkConfigurationChanged(const QNetworkConfiguration &config)
{
    if ( config.state() != QNetworkConfiguration::Active
         || config.bearerType() != QNetworkConfiguration::BearerWLAN ) {

        return;
    }

#ifdef QT_DEBUG
    qDebug() << config.name();
    qDebug() << config.identifier();
#endif

    if (m_lastNetworkIdentifier == config.identifier())
        return;

    // check if at home
    if (m_homezone != nullptr) {
        for (const auto network : m_networks.values(m_homezone->guid())) {
            if (network->identifier() != config.identifier())
                continue;

            m_lastNetworkIdentifier = config.identifier();
            setAtHome(true);
            return;
        }
    }
    // otherwise not at home
    setAtHome(false);

    for (const auto zone : m_zonesModel->zones()) {
        for (const auto network : m_networks.values(zone->guid())) {
            if (network->identifier() != config.identifier())
                continue;

            m_lastNetworkIdentifier = config.identifier();

            QGeoPositionInfo info;
            info.setCoordinate(QGeoCoordinate(zone->latitude(), zone->longitude()));
            info.setTimestamp(QDateTime::currentDateTimeUtc());
            info.setAttribute(QGeoPositionInfo::HorizontalAccuracy, qreal(zone->radius()));

            onPositionChanged(info);
            return;
        }
    }
}

void LocationService::onPositionChanged(const QGeoPositionInfo &info)
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

    emit webhookRequest(Api::RequestWebhookUpdateLocation, location);
}

void LocationService::updateTrackers()
{
    // enable gps tracker
    if ( m_enableGps
         && !(m_disableGpsAtHome && m_atHome) ) {

        m_gps = QGeoPositionInfoSource::createDefaultSource(this);
        m_gps->setUpdateInterval(m_updateInterval);
        m_gps->startUpdates();

        connect(m_gps, &QGeoPositionInfoSource::positionUpdated, this, &LocationService::onPositionChanged);

    } else {
        m_gps->deleteLater();
        m_gps = nullptr;
    }

    // enable wifi tracker
    if (m_enableWifi) {
        m_ncm = new QNetworkConfigurationManager(this);

        connect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &LocationService::onNetworkConfigurationChanged);

    } else {
        m_ncm->deleteLater();
        m_ncm = nullptr;
    }
}

void LocationService::updateHomezone()
{
    if ( qFuzzyCompare(m_homezoneLatitude, 0)
         || qFuzzyCompare(m_homezoneLongitude, 0))
        return;

    const QGeoCoordinate home(m_homezoneLatitude, m_homezoneLongitude);

    m_homezone = nullptr;

    for (auto zone : m_zonesModel->zones()) {
        const QGeoCoordinate loc(zone->latitude(), zone->longitude());
        zone->setIsHome(loc.distanceTo(home) < zone->radius());

        m_homezone = zone;
    }
}
