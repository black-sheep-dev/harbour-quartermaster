#include "locationservice.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMutexLocker>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>

#include "src/constants.h"

#include "src/api/api.h"

LocationService::LocationService(QObject *parent) :
    Service(parent)
{
    //scanForAccessPoints();
    readSettings();
}

LocationService::~LocationService()
{
    writeSettings();
    delete m_mutex;
}

void LocationService::refresh()
{
    if (m_enableWifi)
        updateAccessPoints();
}

AccessPointsModel *LocationService::accessPointsModel(const QString &zoneGuid)
{
    auto model = new AccessPointsModel();

    model->setAccessPoints(m_accessPoints.values(zoneGuid));

    return model;
}

bool LocationService::addAccessPointToZone(const QString &zoneGuid, const QString &identifier, const QString &name)
{
    AccessPoint ap;
    ap.identifier = identifier;
    ap.name = name;

    for (const auto &check : m_accessPoints.values(zoneGuid)) {
        if (check == ap)
            return false;
    }

    m_accessPoints.insertMulti(zoneGuid, ap);
    updateZones();

    return true;
}

AccessPointsModel *LocationService::availableAccessPointsModel()
{
    updateAccessPoints();
    return m_availableAccessPoints;
}

bool LocationService::removeAccessPointFromZone(const QString &zoneGuid, const QString &identifier)
{
    QList<AccessPoint> aps = m_accessPoints.values(zoneGuid);
    bool found{false};

    for (const auto &check : aps) {
        if (check.identifier != identifier)
            continue;

        aps.removeAll(check);
        found = true;
        break;
    }

    if (!found)
        return false;

    m_accessPoints.remove(zoneGuid);
    for (const auto &ap : aps) {
        m_accessPoints.insertMulti(zoneGuid, ap);
    }

    updateZones();

    return true;
}

bool LocationService::resetAccessPoints(const QString &zoneGuid)
{
    m_accessPoints.remove(zoneGuid);
    updateZones();

    return true;
}

void LocationService::scanForAccessPoints()
{
    m_availableAccessPoints->setBusy(true);
    m_ncm->updateConfigurations();
}

void LocationService::getZones()
{
    emit webhookRequest(Api::RequestWebhookGetZones);
}

Zone *LocationService::homezone()
{
    return m_homezone;
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

bool LocationService::trackConnectedApsOnly() const
{
    return m_trackConnectedApsOnly;
}

quint32 LocationService::updateInterval() const
{
    return m_updateInterval;
}

void LocationService::setHomezone(Zone *zone)
{
    if (zone == nullptr)
        return;

    zone->setIsHome(true);
    m_zonesModel->addZone(zone);
    m_homezone = zone;
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

        zone->setNetworkCount(m_accessPoints.values(zone->guid()).count());

        // define homezone
        if (obj.value(ApiKey::KEY_ENTITY_ID).toString() == QLatin1String("zone.home")) {
            if (m_homezone != nullptr) {
                m_homezone->deleteLater();
                m_homezone = nullptr;
            }

            zone->setIsHome(true);
            m_homezone = zone;
        }


        zones.append(zone);
    }

    m_zonesModel->setZones(zones);

    updateZones();
    updateAccessPoints();
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

void LocationService::setTrackConnectedApsOnly(bool only)
{
    if (m_trackConnectedApsOnly == only)
        return;

    m_trackConnectedApsOnly = only;
    emit trackConnectedApsOnlyChanged(m_trackConnectedApsOnly);
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
    QMutexLocker locker(m_mutex);

    if ( config.bearerType() != QNetworkConfiguration::BearerWLAN ) {
        return;
    }

    if (config.state() != QNetworkConfiguration::Active)
        return;

#ifdef QT_DEBUG
    qDebug() << config.name();
    qDebug() << config.identifier();
    qDebug() << config.isValid();
    qDebug() << config.state();
#endif

    if (m_trackConnectedApsOnly && config.state() != QNetworkConfiguration::Active)
        return;

    if (m_lastNetworkIdentifier == config.identifier())
        return;

    for (const auto zone : m_zonesModel->zones()) {
        for (const auto &ap : m_accessPoints.values(zone->guid())) {
            if (ap.identifier != config.identifier())
                continue;

            if (zone->guid() == m_homezone->guid())
                setAtHome(true);
            else
                setAtHome(false);

            m_lastNetworkIdentifier = config.identifier();

            // save for later send when coming online again because of access point connection changed
            m_updatePosition.setCoordinate(QGeoCoordinate(zone->latitude(), zone->longitude()));
            m_updatePosition.setTimestamp(QDateTime::currentDateTimeUtc());
            m_updatePosition.setAttribute(QGeoPositionInfo::HorizontalAccuracy, qreal(zone->radius()));

            return;
        }
    }

    setAtHome(false);
}

void LocationService::onOnlineStateChanged(bool online)
{
    if (!online)
        return;

    QMutexLocker locker(m_mutex);
    auto info = m_updatePosition;
    locker.unlock();

    if (!info.isValid())
        return;

    onPositionChanged(info);

    locker.relock();
    m_updatePosition = QGeoPositionInfo();
}

void LocationService::onPositionChanged(const QGeoPositionInfo &info)
{
#ifdef QT_DEBUG
    qDebug() << info;
    qDebug() << info.isValid();
#endif

    QMutexLocker locker(m_mutex);

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

    locker.unlock();


    if (m_ncm->isOnline())
    emit webhookRequest(Api::RequestWebhookUpdateLocation, location);
}

void LocationService::onScanForAccessPointsFinished()
{
#ifdef QT_DEBUG
    qDebug() << "AP SCAN FINISHED";
#endif

    updateAccessPoints();

    QMutexLocker locker(m_mutex);
    m_availableAccessPoints->setBusy(false);
    locker.unlock();

    emit scanForAccessPointsFinished();
}

AccessPoint LocationService::getAccessPointFromConfig(const QNetworkConfiguration &config) const
{
    AccessPoint ap;
    ap.identifier = config.identifier();
    ap.name = config.name();

    return ap;
}

void LocationService::updateAccessPoints()
{
    if (!m_enableWifi)
        return;

    QList<AccessPoint> aps;

    for (const auto &config : m_ncm->allConfigurations()) {
        if ( config.bearerType() != QNetworkConfiguration::BearerWLAN )
            continue;

        if (config.state() == QNetworkConfiguration::Active)
            onNetworkConfigurationChanged(config);

        aps.append(getAccessPointFromConfig(config));
    }

    m_availableAccessPoints->setAccessPoints(aps);
}

void LocationService::updateTrackers()
{
#ifdef QT_DEBUG
    qDebug() << "UPDATE TRACKERS";
    qDebug() << "GPS: " << m_enableGps;
    qDebug() << "WIFI: " << m_enableWifi;
#endif

    // enable gps tracker
    if ( m_enableGps
         && !((m_disableGpsAtHome && m_enableWifi) && m_atHome)
         && m_gps == nullptr) {


        m_gps = QGeoPositionInfoSource::createDefaultSource(this);
        connect(m_gps, &QGeoPositionInfoSource::positionUpdated, this, &LocationService::onPositionChanged, Qt::QueuedConnection);

        m_gps->setUpdateInterval(m_updateInterval);
        m_gps->startUpdates();

    } else {
        if (m_gps != nullptr) {
            disconnect(m_gps, &QGeoPositionInfoSource::positionUpdated, this, &LocationService::onPositionChanged);
            m_gps->deleteLater();
            m_gps = nullptr;
        }
    }

    // enable wifi tracker
    if (m_enableWifi) {
        if ( m_ncm != nullptr)
            return;

        m_ncm = new QNetworkConfigurationManager(this);
        connect(m_ncm, &QNetworkConfigurationManager::onlineStateChanged, this, &LocationService::onOnlineStateChanged);
        connect(m_ncm, &QNetworkConfigurationManager::updateCompleted, this, &LocationService::onScanForAccessPointsFinished);
        connect(m_ncm, &QNetworkConfigurationManager::configurationAdded, this, &LocationService::onNetworkConfigurationChanged);
        connect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &LocationService::onNetworkConfigurationChanged);
        connect(m_ncm, &QNetworkConfigurationManager::configurationRemoved, this, &LocationService::onNetworkConfigurationChanged);

        updateAccessPoints();

    } else {
        if (m_ncm != nullptr) {
            disconnect(m_ncm, &QNetworkConfigurationManager::updateCompleted, this, &LocationService::onScanForAccessPointsFinished);
            disconnect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &LocationService::onNetworkConfigurationChanged);
            m_ncm->deleteLater();
            m_ncm = nullptr;
        }
    }
}

void LocationService::updateZones()
{
    for (auto zone : m_zonesModel->zones()) {
        zone->setNetworkCount(m_accessPoints.values(zone->guid()).count());
    }
}

void LocationService::connectToApi()
{
    connect(this, &LocationService::webhookRequest, api(), &ApiInterface::sendWebhookRequest);
    connect(api(), &ApiInterface::requestFinished, this, &LocationService::onRequestFinished);
    connect(this, &LocationService::atHomeChanged, api(), &ApiInterface::setAtHome);
}

void LocationService::initialize()
{
    setState(Service::StateInitializing);

    getZones();

    connect(this, &LocationService::settingsChanged, this, &LocationService::updateTrackers);
    updateTrackers();

    setState(Service::StateInitialized);
}

void LocationService::readSettings()
{
    QSettings settings;

    // tracking
    settings.beginGroup(QStringLiteral("TRACKING"));
    setEnableGps(settings.value(QStringLiteral("gps_enabled"), false).toBool());
    setUpdateInterval(settings.value(QStringLiteral("gps_update_interval"), 30000).toUInt());
    setDisableGpsAtHome(settings.value(QStringLiteral("gps_disable_at_home"), false).toBool());
    setEnableWifi(settings.value(QStringLiteral("wifi_enabled"), true).toBool());
    //m_locationService->setTrackConnectedApsOnly(settings.value(QStringLiteral("wifi_track_connected_aps_only"), true).toBool());
    settings.endGroup();

    // homezone
    settings.beginGroup(QStringLiteral("HOMEZONE"));
    const QString guid = settings.value(QStringLiteral("guid")).toString();

    if (!guid.isEmpty()) {
        auto zone = new Zone();
        zone->setGuid(guid);
        zone->setEntityId(QStringLiteral("zone.home"));
        zone->setName(settings.value(QStringLiteral("name")).toString());
        zone->setLatitude(settings.value(QStringLiteral("lat"), 0).toDouble());
        zone->setLongitude(settings.value(QStringLiteral("lon"), 0).toDouble());
        zone->setRadius(settings.value(QStringLiteral("radius"), 10).toDouble());

        setHomezone(zone);
    }

    // access point data
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + QStringLiteral("/networks.json"));

    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&file);

    QJsonParseError error{};

    const QJsonObject data = QJsonDocument::fromJson(in.readAll().toLatin1(), &error).object();

    file.close();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
        qDebug() << error.errorString();
#endif
        return;
    }

    // parse data
    for (const auto &zoneGuid : data.keys()) {
        const QJsonArray aps = data.value(zoneGuid).toArray();

        for (const auto &item : aps) {
            const QJsonObject obj = item.toObject();

            AccessPoint ap;
            ap.name = obj.value(ApiKey::KEY_NAME).toString();
            ap.identifier = obj.value(ApiKey::KEY_IDENTIFIER).toString();

            m_accessPoints.insertMulti(zoneGuid, ap);
        }
    }
}

void LocationService::writeSettings()
{
    QSettings settings;

    // tracking
    settings.beginGroup(QStringLiteral("TRACKING"));
    settings.setValue(QStringLiteral("gps_enabled"), m_enableGps);
    settings.setValue(QStringLiteral("gps_update_interval"), m_updateInterval);
    settings.setValue(QStringLiteral("gps_disable_at_home"), m_disableGpsAtHome);
    settings.setValue(QStringLiteral("wifi_enabled"), m_enableWifi);
    //settings.setValue(QStringLiteral("wifi_track_connected_aps_only"), m_locationService->enableWifi());
    settings.endGroup();

    // homezone
    if (homezone() != nullptr) {
        settings.beginGroup(QStringLiteral("HOMEZONE"));
        settings.setValue(QStringLiteral("guid"), m_homezone->guid());
        settings.setValue(QStringLiteral("name"), m_homezone->latitude());
        settings.setValue(QStringLiteral("lat"), m_homezone->latitude());
        settings.setValue(QStringLiteral("lon"), m_homezone->longitude());
        settings.setValue(QStringLiteral("radius"), m_homezone->radius());
        settings.endGroup();
    }

    // access point settings
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + QStringLiteral("/networks.json"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // create data
    QJsonObject data;

    for (const auto zone : m_zonesModel->zones()) {
        QJsonArray array;

        for (const auto &ap : m_accessPoints.values(zone->guid())) {
            QJsonObject net;
            net.insert(ApiKey::KEY_NAME, ap.name);
            net.insert(ApiKey::KEY_IDENTIFIER, ap.identifier);

            array.append(net);
        }

        if (array.isEmpty())
            continue;

        data.insert(zone->guid(), array);
    }

    if (data.isEmpty())
        return;

    // write data
    QTextStream out(&file);
    out << QString::fromLatin1(QJsonDocument(data).toJson());

    file.close();
}

void LocationService::onRequestFinished(quint8 requestType, const QJsonDocument &data)
{
    if (requestType != Api::RequestWebhookGetZones)
        return;

    setZones(data.array());
}
