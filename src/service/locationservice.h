#ifndef LOCATIONTRACKER_H
#define LOCATIONTRACKER_H

#include "service.h"

#include <QGeoCoordinate>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QMutex>
#include <QNetworkConfigurationManager>

#include <mdm-batteryinfo.h>

#include "src/network/accesspointsmodel.h"
#include "src/models/zonesmodel.h"

class LocationService : public Service
{
    Q_OBJECT

    Q_PROPERTY(bool atHome READ atHome WRITE setAtHome NOTIFY atHomeChanged)
    Q_PROPERTY(bool disableGpsAtHome READ disableGpsAtHome WRITE setDisableGpsAtHome NOTIFY disableGpsAtHomeChanged)
    Q_PROPERTY(bool enableGps READ enableGps WRITE setEnableGps NOTIFY enableGpsChanged)
    Q_PROPERTY(bool enableWifi READ enableWifi WRITE setEnableWifi NOTIFY enableWifiChanged)
    Q_PROPERTY(bool trackConnectedApsOnly READ trackConnectedApsOnly WRITE setTrackConnectedApsOnly NOTIFY trackConnectedApsOnlyChanged)
    Q_PROPERTY(quint32 updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    explicit LocationService(QObject *parent = nullptr);
    ~LocationService() override;

    void refresh();

    // network management
    Q_INVOKABLE AccessPointsModel *accessPointsModel(const QString &zoneGuid);
    Q_INVOKABLE bool addAccessPointToZone(const QString &zoneGuid, const QString &identifier, const QString &name);
    Q_INVOKABLE AccessPointsModel *availableAccessPointsModel();
    Q_INVOKABLE bool removeAccessPointFromZone(const QString &zoneGuid, const QString &identifier);
    Q_INVOKABLE bool resetAccessPoints(const QString &zoneGuid);
    Q_INVOKABLE void scanForAccessPoints();

    // invokables
    Q_INVOKABLE Zone *homezone();   
    Q_INVOKABLE ZonesModel *zonesModel();

    // properties
    bool atHome() const;
    bool disableGpsAtHome() const;
    bool enableGps() const;
    bool enableWifi() const;
    bool trackConnectedApsOnly() const;
    quint32 updateInterval() const;

signals:
    void scanForAccessPointsFinished();
    void settingsChanged();
    void webhookRequest(quint8 requestType, const QJsonObject &payload);

    // properties
    void atHomeChanged(bool home);
    void disableGpsAtHomeChanged(bool disabled);
    void enableGpsChanged(bool enabled);
    void enableWifiChanged(bool enabled);
    void trackConnectedApsOnlyChanged(bool only);
    void updateIntervalChanged(quint32 interval); 

public slots:
    void setHomezone(Zone *zone);
    void setZones(const QJsonArray &arr);

    // properties
    void setAtHome(bool atHome);
    void setDisableGpsAtHome(bool disable);
    void setEnableGps(bool enable);
    void setEnableWifi(bool enable);
    void setTrackConnectedApsOnly(bool only);
    void setUpdateInterval(quint32 interval);

private slots:
    void onNetworkConfigurationChanged(const QNetworkConfiguration &config);
    void onScanForAccessPointsFinished();
    void onPositionChanged(const QGeoPositionInfo &info);

private:
    AccessPoint getAccessPointFromConfig(const QNetworkConfiguration &config) const;
    void updateAccessPoints();
    void updateTrackers();
    void updateZones();

    QHash<QString, AccessPoint> m_accessPoints;
    AccessPointsModel *m_availableAccessPoints{new AccessPointsModel(this)};
    Sailfish::Mdm::BatteryInfo m_battery;
    QGeoPositionInfoSource *m_gps{nullptr};
    Zone *m_homezone{nullptr};
    QString m_lastNetworkIdentifier;
    QGeoPositionInfo m_lastPosition;
    QMutex *m_mutex{new QMutex};
    QNetworkConfigurationManager *m_ncm{nullptr};
    ZonesModel *m_zonesModel{new ZonesModel(this)};

    // properties
    bool m_atHome{false};
    bool m_disableGpsAtHome{true};
    bool m_enableGps{false};
    bool m_enableWifi{true};
    bool m_trackConnectedApsOnly{true};
    quint32 m_updateInterval{30000};

    // Service interface
public:
    void initialize() override;
    void readSettings() override;
    void writeSettings() override;
};

#endif // LOCATIONTRACKER_H
