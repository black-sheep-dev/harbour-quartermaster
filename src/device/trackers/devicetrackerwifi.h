#ifndef DEVICETRACKERWIFI_H
#define DEVICETRACKERWIFI_H

#include "../devicetracker.h"

#include <QNetworkConfigurationManager>

#include "src/models/wifinetworkmodel.h"
#include "src/models/zonesmodel.h"

class DeviceTrackerWifi : public DeviceTracker
{
    Q_OBJECT
public:
    explicit DeviceTrackerWifi(ZonesModel *zones, QObject *parent = nullptr);

    WifiNetworkModel *localNetworkModel();

signals:
    void networkChanged(const QString &ssid);

public slots:
    Q_INVOKABLE void loadNetworkSettings();
    Q_INVOKABLE void saveNetworkSettings();
    Q_INVOKABLE void updateWifiNetworks();

private slots:
    void onConfigurationChanged(const QNetworkConfiguration &config);

private:
    QNetworkConfigurationManager *m_ncm;
    WifiNetworkModel *m_localNetworks;
    ZonesModel *m_zones;

    // DeviceTracker interface
public slots:
    void updateLocation() override;
};

#endif // DEVICETRACKERWIFI_H
