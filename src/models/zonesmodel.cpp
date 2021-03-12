#include "zonesmodel.h"

ZonesModel::ZonesModel(QObject *parent) :
    QAbstractListModel(parent)
{

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
    zone->setParent(this);
    m_zones.append(zone);
    endInsertRows();
}

void ZonesModel::setZones(const QList<Zone *> &zones)
{
    beginResetModel();
    if (!m_zones.isEmpty())
        qDeleteAll(m_zones.begin(), m_zones.end());

    m_zones = zones;

    for (auto zone : m_zones) {
        zone->setParent(this);
    }
    endResetModel();
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

    const auto zone = m_zones.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return zone->name();

    case NameRole:
        return zone->name();

    case GuidRole:
        return zone->guid();

    case IsHomeRole:
        return zone->isHome();

    case LatitudeRole:
        return zone->latitude();

    case LongitudeRole:
        return zone->longitude();

    case NetworkCountRole:
        return zone->networkCount();

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
    roles[IsHomeRole]           = "isHome";
    roles[LatitudeRole]         = "latitude";
    roles[LongitudeRole]        = "longitude";
    roles[NameRole]             = "name";
    roles[NetworkCountRole]     = "networkCount";
    roles[RadiusRole]           = "radius";

    return roles;
}

