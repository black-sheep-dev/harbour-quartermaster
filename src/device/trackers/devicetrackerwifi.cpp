#include "devicetrackerwifi.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>

DeviceTrackerWifi::DeviceTrackerWifi(ZonesModel *zones, QObject *parent) :
    DeviceTracker(parent),
    m_activity(new BackgroundActivity(this)),
    m_ncm(new QNetworkConfigurationManager(this)),
    m_localNetworks(new WifiNetworkModel(this)),
    m_zones(zones)
{
    connect(m_activity, &BackgroundActivity::running, this, &DeviceTrackerWifi::onBackroundServiceRunning);
    connect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &DeviceTrackerWifi::onConfigurationChanged);
    connect(m_localNetworks, &WifiNetworkModel::requestUpdate, this, &DeviceTrackerWifi::updateWifiNetworks);
    connect(m_zones, &ZonesModel::refreshed, this, &DeviceTrackerWifi::loadNetworkSettings);

    updateWifiNetworks();
    loadNetworkSettings();

    m_activity->run();
}

WifiNetworkModel *DeviceTrackerWifi::localNetworkModel()
{
    return m_localNetworks;
}

void DeviceTrackerWifi::loadNetworkSettings()
{
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
    for (Zone *zone : m_zones->zones()) {
        if (!data.contains(zone->guid()))
            continue;
        
        const QJsonArray networks = data.value(zone->guid()).toArray();
        
        QList<WifiNetwork *> list;
        
        for (const QJsonValue &item : networks) {
            const QJsonObject obj = item.toObject();
            
            auto *network = new WifiNetwork;
            network->setName(obj.value(QStringLiteral("name")).toString());
            network->setIdentifier(obj.value(QStringLiteral("identifier")).toString());
            
            list.append(network);
        }
        
        zone->networksModel()->setNetworks(list);
    }

    // update location
    checkNetworkIdentifier(getActiveNetworkIdentifier());
}

void DeviceTrackerWifi::saveNetworkSettings()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + QStringLiteral("/networks.json"));
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    
    // create data
    QJsonObject data;
    
    for (Zone *zone : m_zones->zones()) {
        QJsonArray networks;
        
        for (const WifiNetwork *network : zone->networksModel()->networks()) {
            QJsonObject net;
            net.insert(QStringLiteral("name"), network->name());
            net.insert(QStringLiteral("identifier"), network->identifier());
            
            networks.append(net);
        }
        
        if (networks.isEmpty())
            continue;
        
        data.insert(zone->guid(), networks);
    }
    
    // write data
    QTextStream out(&file);
    out << QString::fromLatin1(QJsonDocument(data).toJson());
    
    file.close();
}

void DeviceTrackerWifi::updateWifiNetworks()
{
    m_localNetworks->setLoading(true);

    QList<WifiNetwork *> networks;

    for (const QNetworkConfiguration &cfg : m_ncm->allConfigurations()) {
        if (cfg.bearerType() == QNetworkConfiguration::BearerWLAN) {

#ifdef QT_DEBUG
            qDebug() << cfg.name();
            qDebug() << cfg.state();
#endif
            auto *network = new WifiNetwork;
            network->setName(cfg.name());
            network->setIdentifier(cfg.identifier());
            network->setDefined(cfg.state() & QNetworkConfiguration::Defined);
            network->setDiscovered((cfg.state() & QNetworkConfiguration::Discovered) == QNetworkConfiguration::Discovered);

            networks.append(network);
        }
    }

    m_localNetworks->setNetworks(networks);

    m_localNetworks->setLoading(false);
}

void DeviceTrackerWifi::onBackroundServiceRunning()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("WIFI BACKGROUND SERVICE");
#endif

    for (const QNetworkConfiguration &cfg : m_ncm->allConfigurations(QNetworkConfiguration::Undefined)) {
        if (cfg.bearerType() == QNetworkConfiguration::BearerWLAN) {

#ifdef QT_DEBUG
            qDebug() << cfg.name();
            qDebug() << cfg.identifier();
#endif
            if (checkNetworkIdentifier(cfg.identifier()))
                break;
        }
    }

    m_activity->wait(BackgroundActivity::ThirtySeconds);
}

void DeviceTrackerWifi::onConfigurationChanged(const QNetworkConfiguration &config)
{
    if ( config.state() != QNetworkConfiguration::Active
         || config.bearerType() != QNetworkConfiguration::BearerWLAN ) {

        return;
    }

#ifdef QT_DEBUG
    qDebug() << config.name();
    qDebug() << config.identifier();
#endif

    checkNetworkIdentifier(config.identifier());
}

bool DeviceTrackerWifi::checkNetworkIdentifier(const QString &identifier)
{
    if (m_lastIdentifier == identifier)
        return false;

    for (auto *zone : m_zones->zones()) {
        for (const auto *network : zone->networksModel()->networks()) {
            if (network->identifier() != identifier)
                continue;

            m_lastIdentifier = identifier;

            m_currentPositionInfo.setCoordinate(QGeoCoordinate(zone->latitude(), zone->longitude()));
            m_currentPositionInfo.setTimestamp(QDateTime::currentDateTimeUtc());
            m_currentPositionInfo.setAttribute(QGeoPositionInfo::HorizontalAccuracy, qreal(zone->radius()));


#ifdef QT_DEBUG
            qDebug() << zone->name();
            qDebug() << m_currentPositionInfo.coordinate();
#endif

            QTimer::singleShot(1000, this, &DeviceTrackerWifi::updateLocation);

            return true;
        }
    }

    return false;
}

QString DeviceTrackerWifi::getActiveNetworkIdentifier() const
{
    for (const QNetworkConfiguration &cfg : m_ncm->allConfigurations()) {
        if ( cfg.bearerType() != QNetworkConfiguration::BearerWLAN
             || cfg.state() != QNetworkConfiguration::Active) {

            continue;
        }

        return cfg.identifier();
    }

    return QString();
}

void DeviceTrackerWifi::reloadConfig()
{
    loadNetworkSettings();
}


void DeviceTrackerWifi::updateLocation()
{
    onPositionChanged(m_currentPositionInfo);
}
