#ifndef ZONESMODEL_H
#define ZONESMODEL_H

#include <QAbstractListModel>

#include "src/entities/zone.h"

class ZonesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    enum ZoneRoles {
        GuidRole        = Qt::UserRole + 1,
        LatitudeRole,
        LongitudeRole,
        NameRole,
        NetworkCountRole,
        RadiusRole
    };
    Q_ENUM(ZoneRoles)

    explicit ZonesModel(QObject *parent = nullptr);

    Q_INVOKABLE Zone *zoneAt(int index);
    QList<Zone *> zones() const;

    void addZone(Zone *zone);
    void setZones(const QList<Zone *> &zones);
    void setZones(const QJsonArray &array);

    // properties
    bool loading() const;

signals:
    void loadingChanged(bool loading); 
    void refreshed();

public slots:
    void setLoading(bool loading);

private slots:
    void onNetworksChanged();

private:
    QList<Zone *> m_zones;

    // properties
    bool m_loading{false};


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // ZONESMODEL_H
