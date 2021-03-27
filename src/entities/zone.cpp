#include "zone.h"

#include <QJsonObject>

#include "src/constants.h"

Zone::Zone(QObject *parent) :
    QObject(parent)
    
{
}

QString Zone::entityId() const
{
    return m_entityId;
}

QString Zone::guid() const
{
    return m_guid;
}

bool Zone::isHome() const
{
    return m_isHome;
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

quint16 Zone::networkCount() const
{
    return m_networkCount;
}

double Zone::radius() const
{
    return m_radius;
}

void Zone::setEntityId(const QString &entityId)
{
    if (m_entityId == entityId)
        return;

    m_entityId = entityId;
    emit entityIdChanged(m_entityId);
}

void Zone::setGuid(const QString &guid)
{
    if (m_guid == guid)
        return;

    m_guid = guid;
    emit guidChanged(m_guid);
}

void Zone::setIsHome(bool isHome)
{
    if (m_isHome == isHome)
        return;

    m_isHome = isHome;
    emit isHomeChanged(m_isHome);
    emit changed();
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
    emit changed();
}

void Zone::setNetworkCount(quint16 count)
{
    if (m_networkCount == count)
        return;

    m_networkCount = count;
    emit networkCountChanged(m_networkCount);
    emit changed();
}

void Zone::setRadius(double radius)
{
    if (qFuzzyCompare(m_radius, radius))
        return;

    m_radius = radius;
    emit radiusChanged(m_radius);
}

