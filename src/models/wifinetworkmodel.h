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
        NameRole        = Qt::UserRole + 1,
        IdentifierRole,
        SelectedRole
    };
    Q_ENUMS(WifiNetworkRoles)

    explicit WifiNetworkModel(QObject *parent = nullptr);
    ~WifiNetworkModel() override;

    Q_INVOKABLE void update();


    void addNetwork(WifiNetwork *network);
    Q_INVOKABLE void addNetwork(const QString &ssid, const QString &identifier);
    void removeNetwork(WifiNetwork *network);
    Q_INVOKABLE void removeNetwork(const QString &identifier);
    void reset();
    void setNetworks(const QList<WifiNetwork *> &networks);

    // properties
    bool loading() const;

signals:
    void requestUpdate();

    // properties
    void loadingChanged(bool loading);

public slots:
    // properties
    void setLoading(bool loading);

private:
    QList<WifiNetwork *> m_networks;

    // properties
    bool m_loading;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;



};

#endif // WIFINETWORKMODEL_H
