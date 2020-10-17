#ifndef WIFINETWORKMODEL_H
#define WIFINETWORKMODEL_H

#include <QAbstractListModel>

#include "src/entities/wifinetwork.h"

class WifiNetworkModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    enum WifiNetworkRoles {
        DefinedRole         = Qt::UserRole + 1,
        DiscoveredRole,
        NameRole,
        IdentifierRole,
        SelectedRole
    };
    Q_ENUM(WifiNetworkRoles)

    explicit WifiNetworkModel(QObject *parent = nullptr);

    Q_INVOKABLE void addSelectedToModel(WifiNetworkModel *model);
    Q_INVOKABLE void resetSelection();
    Q_INVOKABLE void setSelected(WifiNetworkModel *model);
    Q_INVOKABLE void update();


    void addNetwork(WifiNetwork *network);
    Q_INVOKABLE void addNetwork(const QString &name, const QString &identifier);
    void removeNetwork(WifiNetwork *network);
    Q_INVOKABLE void removeNetwork(const QString &identifier);
    Q_INVOKABLE void reset();
    void setNetworks(const QList<WifiNetwork *> &networks);
    QList<WifiNetwork *> networks() const;

    // properties
    bool loading() const;

signals:
    void changed();
    void requestUpdate();

    // properties
    void loadingChanged(bool loading);

public slots:
    // properties
    void setLoading(bool loading);

private:
    QList<WifiNetwork *> m_networks;

    // properties
    bool m_loading{false};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // WIFINETWORKMODEL_H
