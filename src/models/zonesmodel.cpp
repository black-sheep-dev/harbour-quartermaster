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
    connect(zone, &Zone::networksChanged, this, &ZonesModel::onNetworksChanged);
    endInsertRows();
}

void ZonesModel::setZones(const QList<Zone *> &zones)
{
    beginResetModel();
    qDeleteAll(m_zones.begin(), m_zones.end());
    m_zones.clear();
    m_zones = zones;

    for (Zone *zone : m_zones) {
        connect(zone, &Zone::networksChanged, this, &ZonesModel::onNetworksChanged);
    }

    endResetModel();

    setLoading(false);
    emit refreshed();
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

void ZonesModel::onNetworksChanged()
{
    auto *zone = qobject_cast<Zone *>(sender());

    if (!zone)
        return;

    const QModelIndex idx = index(m_zones.indexOf(zone));

    if (!idx.isValid())
        return;

    emit dataChanged(idx, idx);
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

    case NetworkCountRole:
        return zone->networksModel()->networks().count();

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
    roles[NetworkCountRole]     = "networks_count";
    roles[RadiusRole]           = "radius";

    return roles;
}

