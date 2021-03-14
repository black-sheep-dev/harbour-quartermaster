#include "accesspointsmodel.h"

AccessPointsModel::AccessPointsModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

QList<AccessPoint> AccessPointsModel::accessPoints() const
{
    return m_accessPoints;
}

void AccessPointsModel::addAccessPoint(const AccessPoint &ap)
{
    beginInsertRows(QModelIndex(), m_accessPoints.count(), m_accessPoints.count());
    m_accessPoints.append(ap);
    endInsertRows();
}

void AccessPointsModel::addAccessPoint(const QString &identifier, const QString &name)
{
    AccessPoint ap;
    ap.identifier = identifier;
    ap.name = name;

    addAccessPoint(ap);
}

void AccessPointsModel::removeAccessPoint(const QString &identifier)
{
    int idx{-1};

    for (const auto &ap : m_accessPoints) {
        idx++;

        if (ap.identifier == identifier)
            break;
    }

    if (idx < 0)
        return;

    beginRemoveRows(QModelIndex(), idx, idx);
    m_accessPoints.takeAt(idx);
    endRemoveRows();
}

void AccessPointsModel::setAccessPoints(const QList<AccessPoint> &aps)
{
    beginResetModel();
    m_accessPoints.clear();
    m_accessPoints = aps;
    endResetModel();
}

void AccessPointsModel::reset()
{
    beginResetModel();
    m_accessPoints.clear();
    endResetModel();
}

bool AccessPointsModel::busy() const
{
    return m_busy;
}

void AccessPointsModel::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged(m_busy);
}

int AccessPointsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_accessPoints.count();
}

QVariant AccessPointsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto &ap = m_accessPoints[index.row()];

    switch (role) {
    case IdentifierRole:
        return ap.identifier;

    case NameRole:
        return ap.name;

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AccessPointsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[IdentifierRole]       = "identifier";
    roles[NameRole]             = "name";

    return roles;
}
