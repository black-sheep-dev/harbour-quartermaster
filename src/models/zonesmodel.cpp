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

    connect(zone, &Zone::changed, this, &ZonesModel::updateZone);

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

        connect(zone, &Zone::changed, this, &ZonesModel::updateZone);
    }
    endResetModel();
}

void ZonesModel::updateZone()
{
    auto zone = qobject_cast<Zone *>(sender());
    if (zone == nullptr)
        return;

    int i{-1};
    for (const auto z : m_zones) {
        i++;
        if (z->guid() == zone->guid())
            break;
    }

    const QModelIndex idx = index(i);

    if (!idx.isValid())
        return;

    emit dataChanged(idx, idx, QVector<int>() << NameRole << IsHomeRole << NetworkCountRole);
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

