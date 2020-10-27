#include "zone.h"

#include <QJsonObject>

#include "src/string_constants.h"

Zone::Zone(QObject *parent) :
    QObject(parent),
    m_networks(new WifiNetworkModel(this)),
    m_name(QString())
    
{
    connect(m_networks, &WifiNetworkModel::changed, this, &Zone::networksChanged);
}

WifiNetworkModel *Zone::networksModel()
{
    return m_networks;
}

void Zone::setJson(const QJsonObject &object)
{
    if (object.isEmpty())
        return;

    const QJsonObject attributes = object.value(API_KEY_ATTRIBUTES).toObject();
    const QJsonObject context = object.value(API_KEY_CONTEXT).toObject();

    setGuid(context.value(API_KEY_ID).toString());
    setName(attributes.value(API_KEY_FRIENDLY_NAME).toString());
    setLatitude(attributes.value(API_KEY_LATITUDE).toDouble());
    setLongitude(attributes.value(API_KEY_LONGITUDE).toDouble());
    setRadius(attributes.value(API_KEY_RADIUS).toDouble());
}

QString Zone::guid() const
{
    return m_guid;
}

double Zone::latitude() const
{
    return m_latitude;
}

double Zone::longitude() const
{
    return m_longitude;
}

QString Zone::name() const
{
    return m_name;
}

double Zone::radius() const
{
    return m_radius;
}

void Zone::setGuid(const QString &guid)
{
    if (m_guid == guid)
        return;

    m_guid = guid;
    emit guidChanged(m_guid);
}

void Zone::setLatitude(double latitude)
{
    if (qFuzzyCompare(m_latitude, latitude))
        return;

    m_latitude = latitude;
    emit latitudeChanged(m_latitude);
}

void Zone::setLongitude(double longitude)
{
    if (qFuzzyCompare(m_longitude, longitude))
        return;

    m_longitude = longitude;
    emit longitudeChanged(m_longitude);
}

void Zone::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void Zone::setRadius(double radius)
{
    if (qFuzzyCompare(m_radius, radius))
        return;

    m_radius = radius;
    emit radiusChanged(m_radius);
}

