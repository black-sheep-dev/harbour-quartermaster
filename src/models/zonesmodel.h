#ifndef ZONESMODEL_H
#define ZONESMODEL_H

#include <QAbstractListModel>

#include "src/entities/zone.h"

class ZonesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ZoneRoles {
        GuidRole        = Qt::UserRole + 1,
        IsHomeRole,
        LatitudeRole,
        LongitudeRole,
        NameRole,
        NetworkCountRole,
        RadiusRole
    };
    Q_ENUM(ZoneRoles)

    explicit ZonesModel(QObject *parent = nullptr);

    Q_INVOKABLE Zone *zoneAt(int index);
    const QList<Zone *> &zones() const;

    void addZone(Zone *zone);
    void setZones(const QList<Zone *> &zones);

private slots:
    void updateZone();

private:
    QList<Zone *> m_zones;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // ZONESMODEL_H
