#ifndef ZONE_H
#define ZONE_H

#include <QObject>

#include <QGeoCoordinate>
#include "src/models/wifinetworkmodel.h"

class Zone : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString guid READ guid WRITE setGuid NOTIFY guidChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit Zone(QObject *parent = nullptr);

    // networks
    Q_INVOKABLE WifiNetworkModel *networksModel();

    void setData(const QJsonObject &object);

    // properties
    QString guid() const;
    double latitude() const;
    double longitude() const;
    QString name() const;
    double radius() const;

signals:
    void networksChanged();

    // properties
    void guidChanged(QString guid);
    void latitudeChanged(double latitude);
    void longitudeChanged(double longitude);
    void nameChanged(const QString &name);
    void radiusChanged(double radius);

public slots:
    // properties
    void setGuid(const QString &guid);
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    void setName(const QString &name);
    void setRadius(double radius);

private:
    WifiNetworkModel *m_networks{nullptr};

    // properties
    QString m_guid;
    double m_latitude{0.0};
    double m_longitude{0.0};
    QString m_name;
    double m_radius{0.0};
};

#endif // ZONE_H
