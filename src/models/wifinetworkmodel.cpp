#include "wifinetworkmodel.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

WifiNetworkModel::WifiNetworkModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

void WifiNetworkModel::addSelectedToModel(WifiNetworkModel *model)
{
    if (!model)
        return;

    for (auto network : m_networks) {
        if (network->selected())
            model->addNetwork(network->name(), network->identifier());
        else
            model->removeNetwork(network->identifier());
    }
}

void WifiNetworkModel::resetSelection()
{
    beginResetModel();
    for (auto network : m_networks) {
        network->setSelected(false);
    }
    endResetModel();
}

void WifiNetworkModel::setSelected(WifiNetworkModel *model)
{
    if (!model)
        return;

    const QList<WifiNetwork *> networks = model->networks();
    for (const auto selected : networks) {
        for (auto network : m_networks) {
            if (network->identifier() != selected->identifier())
                continue;

            network->setSelected(true);
            break;
        }
    }
    emit dataChanged(index(0), index(m_networks.count() - 1));
}

void WifiNetworkModel::update()
{
    setLoading(true);
    emit requestUpdate();
}

void WifiNetworkModel::addNetwork(WifiNetwork *network)
{
    if (!network)
        return;

    beginInsertRows(QModelIndex(), m_networks.count(), m_networks.count());
    m_networks.append(network);
    endInsertRows();

    emit changed();
}

void WifiNetworkModel::addNetwork(const QString &name, const QString &identifier)
{
    for (const auto network : m_networks) {
        if (network->identifier() == identifier)
            return;
    }

    auto network = new WifiNetwork(this);
    network->setName(name);
    network->setIdentifier(identifier);

    addNetwork(network);
}

void WifiNetworkModel::removeNetwork(WifiNetwork *network)
{
    const int idx = m_networks.indexOf(network);

    if (idx < 0)
        return;

    beginRemoveRows(QModelIndex(), idx, idx);
    m_networks.takeAt(idx)->deleteLater();
    endRemoveRows();

    emit changed();
}

void WifiNetworkModel::removeNetwork(const QString &identifier)
{
    for (auto it = m_networks.begin(); it != m_networks.end(); ++it) {
        auto network = qobject_cast<WifiNetwork *>(*it);
        if (network->identifier() != identifier)
            continue;

        removeNetwork(network);
        break;
    }
}

void WifiNetworkModel::reset()
{
    beginResetModel();
    qDeleteAll(m_networks.begin(), m_networks.end());
    m_networks.clear();
    endResetModel();

    emit changed();
}

void WifiNetworkModel::setNetworks(const QList<WifiNetwork *> &networks)
{
    beginResetModel();
    if (!m_networks.isEmpty())
        qDeleteAll(m_networks.begin(), m_networks.end());

    for (auto network : networks) {
        network->setParent(this);
    }

    m_networks = networks;
    endResetModel();

    setLoading(false);

    emit changed();
}

QList<WifiNetwork *> WifiNetworkModel::networks() const
{
    return m_networks;
}

bool WifiNetworkModel::loading() const
{
    return m_loading;
}

void WifiNetworkModel::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

int WifiNetworkModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_networks.count();
}

QVariant WifiNetworkModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto network = m_networks.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return network->name();

    case NameRole:
        return network->name();

    case IdentifierRole:
        return network->identifier();

    case SelectedRole:
        return network->selected();

    case DefinedRole:
        return network->defined();

    case DiscoveredRole:
        return network->discovered();

    default:
        return QVariant();
    }
}

bool WifiNetworkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role != SelectedRole)
        return false;

    m_networks.at(index.row())->setSelected(value.toBool());
    emit dataChanged(index, index);
    return true;
}

QHash<int, QByteArray> WifiNetworkModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[DefinedRole]      = "defined";
    roles[DiscoveredRole]   = "discovered";
    roles[NameRole]         = "name";
    roles[IdentifierRole]   = "identifier";
    roles[SelectedRole]     = "selected";

    return roles;
}
