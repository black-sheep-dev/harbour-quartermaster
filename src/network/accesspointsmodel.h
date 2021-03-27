#ifndef ACCESSPOINTSMODEL_H
#define ACCESSPOINTSMODEL_H

#include <QAbstractListModel>

#include "accesspoint.h"

class AccessPointsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)

public:
    enum AccessPointRoles {
        IdentifierRole      = Qt::UserRole + 1,
        NameRole
    };
    Q_ENUM(AccessPointRoles)

    explicit AccessPointsModel(QObject *parent = nullptr);

    QList<AccessPoint> accessPoints() const;

    void addAccessPoint(const AccessPoint &ap);
    Q_INVOKABLE void addAccessPoint(const QString &identifier, const QString &name);
    Q_INVOKABLE void removeAccessPoint(const QString &identifier);
    void setAccessPoints(const QList<AccessPoint> &aps);
    Q_INVOKABLE void reset();

    // properties
    bool busy() const;

signals:
    // properties
    void busyChanged(bool busy);

public slots:
    // properties
    void setBusy(bool busy);

private:
    QList<AccessPoint> m_accessPoints;

    // properties
    bool m_busy{false};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // ACCESSPOINTSMODEL_H
