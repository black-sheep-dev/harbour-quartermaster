#include "wifinetworkmodel.h"

WifiNetworkModel::WifiNetworkModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

WifiNetworkModel::~WifiNetworkModel()
{
    qDeleteAll(m_networks.begin(), m_networks.end());
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
}

void WifiNetworkModel::addNetwork(const QString &ssid, const QString &identifier)
{
    for (const WifiNetwork *network : m_networks) {
        if (network->identifier() == identifier)
            return;
    }

    auto *network = new WifiNetwork;
    network->setName(ssid);
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
}

void WifiNetworkModel::removeNetwork(const QString &identifier)
{
    for (WifiNetwork *network : m_networks) {
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
}

void WifiNetworkModel::setNetworks(const QList<WifiNetwork *> &networks)
{
    beginResetModel();
    qDeleteAll(m_networks.begin(), m_networks.end());
    m_networks.clear();
    m_networks = networks;
    endResetModel();

    setLoading(false);
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

    const WifiNetwork *network = m_networks.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return network->name();

    case NameRole:
        return network->name();

    case IdentifierRole:
        return network->identifier();

    case SelectedRole:
        return network->selected();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> WifiNetworkModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NameRole]         = "name";
    roles[IdentifierRole]   = "identifier";
    roles[SelectedRole]     = "selected";

    return roles;
}
