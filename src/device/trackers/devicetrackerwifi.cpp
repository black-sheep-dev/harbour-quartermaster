#include "devicetrackerwifi.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

DeviceTrackerWifi::DeviceTrackerWifi(ZonesModel *zones, QObject *parent) :
    DeviceTracker(parent),
    m_ncm(new QNetworkConfigurationManager(this)),
    m_localNetworks(new WifiNetworkModel(this)),
    m_zones(zones)
{
    connect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &DeviceTrackerWifi::onConfigurationChanged);
    connect(m_localNetworks, &WifiNetworkModel::requestUpdate, this, &DeviceTrackerWifi::updateWifiNetworks);
}

WifiNetworkModel *DeviceTrackerWifi::localNetworkModel()
{
    m_localNetworks->setLoading(true);
    updateWifiNetworks();

    return m_localNetworks;
}

void DeviceTrackerWifi::updateWifiNetworks()
{
    QNetworkConfigurationManager ncm;

    QList<WifiNetwork *> networks;

    for (const QNetworkConfiguration &cfg : ncm.allConfigurations()) {
        if (cfg.bearerType() == QNetworkConfiguration::BearerWLAN) {

#ifdef QT_DEBUG
            qDebug() << cfg.name();
            qDebug() << cfg.identifier();
#endif
            WifiNetwork *network = new WifiNetwork;
            network->setName(cfg.name());
            network->setIdentifier(cfg.identifier());

            networks.append(network);
        }
    }

    m_localNetworks->setNetworks(networks);
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
