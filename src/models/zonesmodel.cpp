#include "zonesmodel.h"

#include <QJsonArray>
#include <QJsonObject>

ZonesModel::ZonesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

ZonesModel::~ZonesModel()
{
    qDeleteAll(m_zones.begin(), m_zones.end());
}

Zone *ZonesModel::zoneAt(const int index)
{
    if (index < 0 || index >= m_zones.count())
        return nullptr;

    return m_zones.at(index);
}

QList<Zone *> ZonesModel::zones() const
{
    return m_zones;
}

void ZonesModel::addZone(Zone *zone)
{
    if (!zone)
        return;

    beginInsertRows(QModelIndex(), m_zones.count(), m_zones.count());
    m_zones.append(zone);
    endInsertRows();
}

void ZonesModel::setZones(const QList<Zone *> &zones)
{
    beginResetModel();
    qDeleteAll(m_zones.begin(), m_zones.end());
    m_zones.clear();
    m_zones = zones;
    endResetModel();

    setLoading(false);
}

void ZonesModel::setZones(const QJsonArray &array)
{
    QList<Zone *> zones;

    for (const QJsonValue &value : array) {
        Zone *zone = new Zone;
        zone->setJson(value.toObject());

        zones.append(zone);
    }

    setZones(zones);
}

bool ZonesModel::loading() const
{
    return m_loading;
}

void ZonesModel::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

int ZonesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_zones.count();
}

QVariant ZonesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Zone *zone = m_zones.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return zone->name();

    case NameRole:
        return zone->name();

    case GuidRole:
        return zone->guid();

    case LatitudeRole:
        return zone->latitude();

    case LongitudeRole:
        return zone->longitude();

    case RadiusRole:
        return zone->radius();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ZonesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[GuidRole]             = "guid";
    roles[LatitudeRole]         = "latitude";
    roles[LongitudeRole]        = "longitude";
    roles[NameRole]             = "name";
    roles[RadiusRole]           = "radius";

    return roles;
}

