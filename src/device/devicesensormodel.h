#ifndef DEVICESENSORMODEL_H
#define DEVICESENSORMODEL_H

#include <QAbstractListModel>

#include "devicesensor.h"

class DeviceSensorModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DeviceSensorRole {
        DeviceClassRole     = Qt::UserRole + 1,
        EnabledRole,
        NameRole,
        StateRole,
        UniqueIdRole,
        UnitRole
    };
    Q_ENUM(DeviceSensorRole)

    explicit DeviceSensorModel(QObject *parent = nullptr);
    ~DeviceSensorModel() override;

    QList<DeviceSensor *> sensors() const;

public slots:
    void addSensor(DeviceSensor *sensor);
    void setSensors(const QList<DeviceSensor *> &sensors);

private:
    QList<DeviceSensor *> m_sensors;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // DEVICESENSORMODEL_H
