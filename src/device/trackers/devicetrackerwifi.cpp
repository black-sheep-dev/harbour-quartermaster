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

DeviceTrackerWifi::DeviceTrackerWifi(ZonesModel *zones, QObject *parent) :
    DeviceTracker(parent),
    m_ncm(new QNetworkConfigurationManager(this)),
    m_localNetworks(new WifiNetworkModel(this)),
    m_zones(zones)
{
    connect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &DeviceTrackerWifi::onConfigurationChanged);
    connect(m_localNetworks, &WifiNetworkModel::requestUpdate, this, &DeviceTrackerWifi::updateWifiNetworks);
    connect(m_zones, &ZonesModel::refreshed, this, &DeviceTrackerWifi::loadNetworkSettings);

    updateWifiNetworks();
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

    QNetworkConfigurationManager ncm;

    QList<WifiNetwork *> networks;

    for (const QNetworkConfiguration &cfg : ncm.allConfigurations()) {
        if (cfg.bearerType() == QNetworkConfiguration::BearerWLAN) {

#ifdef QT_DEBUG
            qDebug() << cfg.name();
            qDebug() << cfg.identifier();
#endif
            auto *network = new WifiNetwork;
            network->setName(cfg.name());
            network->setIdentifier(cfg.identifier());

            networks.append(network);
        }
    }

    m_localNetworks->setNetworks(networks);

    m_localNetworks->setLoading(false);
}

void DeviceTrackerWifi::onConfigurationChanged(const QNetworkConfiguration &config)
{
#ifdef QT_DEBUG
    qDebug() << config.name();
    qDebug() << config.identifier();
#endif

    if (config.state() != QNetworkConfiguration::Active)
        return;

    emit networkChanged(config.identifier());
}

void DeviceTrackerWifi::updateLocation()
{

}
