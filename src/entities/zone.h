#ifndef ZONE_H
#define ZONE_H

#include <QObject>

#include <QGeoCoordinate>
//#include "src/models/wifinetworkmodel.h"

class Zone : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString guid READ guid WRITE setGuid NOTIFY guidChanged)
    Q_PROPERTY(bool isHome READ isHome WRITE setIsHome NOTIFY isHomeChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(quint16 networkCount READ networkCount WRITE setNetworkCount NOTIFY networkCountChanged)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit Zone(QObject *parent = nullptr);

    // properties
    QString guid() const;
    bool isHome() const;
    double latitude() const;
    double longitude() const;
    QString name() const;
    quint16 networkCount() const;
    double radius() const;

signals:
    // properties
    void guidChanged(const QString &guid);
    void isHomeChanged(bool isHome);
    void latitudeChanged(double latitude);
    void longitudeChanged(double longitude);
    void nameChanged(const QString &name);
    void networkCountChanged(quint16 count);
    void radiusChanged(double radius);

public slots:
    // properties
    void setGuid(const QString &guid);
    void setIsHome(bool isHome);
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    void setName(const QString &name);
    void setNetworkCount(quint16 count);
    void setRadius(double radius);

private:
    // properties
    QString m_guid;
    bool m_isHome{false};
    double m_latitude{0.0};
    double m_longitude{0.0};
    QString m_name;
    quint16 m_networkCount{0};
    double m_radius{0.0};

};

#endif // ZONE_H
