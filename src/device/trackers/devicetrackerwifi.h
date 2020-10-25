#ifndef DEVICETRACKERWIFI_H
#define DEVICETRACKERWIFI_H

#include "../devicetracker.h"

#include <QNetworkConfigurationManager>

#include "src/models/wifinetworkmodel.h"
#include "src/models/zonesmodel.h"

#include <keepalive/backgroundactivity.h>

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
    void onBackroundServiceRunning();
    void onConfigurationChanged(const QNetworkConfiguration &config);

private:
    bool checkNetworkIdentifier(const QString &identifier);
    QString getActiveNetworkIdentifier() const;

    BackgroundActivity *m_activity{nullptr};
    QGeoPositionInfo m_currentPositionInfo;
    QNetworkConfigurationManager *m_ncm{nullptr};
    QString m_lastIdentifier;
    WifiNetworkModel *m_localNetworks{nullptr};
    ZonesModel *m_zones{nullptr};

    // DeviceTracker interface
public:
    void reloadConfig() override;

public slots:
    void updateLocation() override;
};

#endif // DEVICETRACKERWIFI_H
