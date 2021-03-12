#ifndef LOCATIONTRACKER_H
#define LOCATIONTRACKER_H

#include <QObject>

#include <QGeoCoordinate>

#include "src/models/wifinetworkmodel.h"
#include "src/models/zonesmodel.h"

class LocationTracker : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enableGPS READ enableGPS WRITE setEnableGPS NOTIFY enableGPSChanged)
    Q_PROPERTY(bool enableWifi READ enableWifi WRITE setEnableWifi NOTIFY enableWifiChanged)
    Q_PROPERTY(double homezoneLatitude READ homezoneLatitude WRITE setHomezoneLatitude NOTIFY homezoneLatitudeChanged)
    Q_PROPERTY(double homezoneLongitude READ homezoneLongitude WRITE setHomezoneLongitude NOTIFY homezoneLongitudeChanged)

public:
    explicit LocationTracker(QObject *parent = nullptr);
    ~LocationTracker() override;

    Q_INVOKABLE WifiNetworkModel *networksModel(const QString &zoneGuid);
    Q_INVOKABLE void storeNetworkModel(const QString &zoneGuid, WifiNetworkModel *model);
    Q_INVOKABLE ZonesModel *zonesModel();

    // properties
    bool enableGPS() const;
    bool enableWifi() const;
    double homezoneLatitude() const;
    double homezoneLongitude() const;

signals:
    // properties
    void enableGPSChanged(bool enabled);
    void enableWifiChanged(bool enabled);
    void homezoneLatitudeChanged(double latitude);
    void homezoneLongitudeChanged(double longitude);

public slots:
    void setZones(const QJsonArray &arr);

    // properties
    void setEnableGPS(bool enable);
    void setEnableWifi(bool enable);
    void setHomezoneLatitude(double latitude);
    void setHomezoneLongitude(double longitude);

private:
    void updateHomezone();

    QHash<QString, WifiNetwork *> m_networks;
    ZonesModel *m_zonesModel{new ZonesModel(this)};

    // properties
    bool m_enableGPS{false};
    bool m_enableWifi{false};
    double m_homezoneLatitude{0};
    double m_homezoneLongitude{0};
};

#endif // LOCATIONTRACKER_H
