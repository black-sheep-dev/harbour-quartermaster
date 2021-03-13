#ifndef LOCATIONTRACKER_H
#define LOCATIONTRACKER_H

#include <QObject>

#include <QGeoCoordinate>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QNetworkConfigurationManager>

#include <mdm-batteryinfo.h>

#include "src/models/wifinetworkmodel.h"
#include "src/models/zonesmodel.h"

class LocationService : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool atHome READ atHome WRITE setAtHome NOTIFY atHomeChanged)
    Q_PROPERTY(bool disableGpsAtHome READ disableGpsAtHome WRITE setDisableGpsAtHome NOTIFY disableGpsAtHomeChanged)
    Q_PROPERTY(bool enableGps READ enableGps WRITE setEnableGps NOTIFY enableGpsChanged)
    Q_PROPERTY(bool enableWifi READ enableWifi WRITE setEnableWifi NOTIFY enableWifiChanged) 
    Q_PROPERTY(double homezoneLatitude READ homezoneLatitude WRITE setHomezoneLatitude NOTIFY homezoneLatitudeChanged)
    Q_PROPERTY(double homezoneLongitude READ homezoneLongitude WRITE setHomezoneLongitude NOTIFY homezoneLongitudeChanged)
    Q_PROPERTY(quint32 updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    explicit LocationService(QObject *parent = nullptr);

    void initialize();

    // invokables
    Q_INVOKABLE Zone *homezone();
    Q_INVOKABLE WifiNetworkModel *localNetworksModel();
    Q_INVOKABLE WifiNetworkModel *networksModel(const QString &zoneGuid);
    Q_INVOKABLE void storeNetworkModel(const QString &zoneGuid, WifiNetworkModel *model);
    //Q_INVOKABLE void updateWifiNetworks();
    Q_INVOKABLE ZonesModel *zonesModel();

    // properties
    bool atHome() const;
    bool disableGpsAtHome() const;
    bool enableGps() const;
    bool enableWifi() const;
    double homezoneLatitude() const;
    double homezoneLongitude() const; 
    quint32 updateInterval() const;

signals:
    void webhookRequest(quint8 requestType, const QJsonObject &payload);
    void settingsChanged();

    // properties
    void atHomeChanged(bool home);
    void disableGpsAtHomeChanged(bool disabled);
    void enableGpsChanged(bool enabled);
    void enableWifiChanged(bool enabled);
    void homezoneLatitudeChanged(double latitude);
    void homezoneLongitudeChanged(double longitude);
    void updateIntervalChanged(quint32 interval);

public slots:
    void setZones(const QJsonArray &arr);

    // properties
    void setAtHome(bool atHome);
    void setDisableGpsAtHome(bool disable);
    void setEnableGps(bool enable);
    void setEnableWifi(bool enable);
    void setHomezoneLatitude(double latitude);
    void setHomezoneLongitude(double longitude);
    void setUpdateInterval(quint32 interval);

private slots:
    void onNetworkConfigurationChanged(const QNetworkConfiguration &config);
    void onPositionChanged(const QGeoPositionInfo &info);

private:
    void updateHomezone();
    void updateTrackers();

    Sailfish::Mdm::BatteryInfo m_battery;
    QNetworkConfigurationManager *m_ncm{nullptr};
    QGeoPositionInfoSource *m_gps{nullptr};
    Zone *m_homezone{nullptr};
    QString m_lastNetworkIdentifier;
    QGeoPositionInfo m_lastPosition;
    WifiNetworkModel *m_localNetworksModel{new WifiNetworkModel(this)};
    QHash<QString, WifiNetwork *> m_networks;
    ZonesModel *m_zonesModel{new ZonesModel(this)};

    // properties
    bool m_atHome{false};
    bool m_disableGpsAtHome{true};
    bool m_enableGps{false};
    bool m_enableWifi{true};
    double m_homezoneLatitude{0};
    double m_homezoneLongitude{0};
    quint32 m_updateInterval{30000};


};

#endif // LOCATIONTRACKER_H
