#include "devicesensormodel.h"

DeviceSensorModel::DeviceSensorModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

DeviceSensorModel::~DeviceSensorModel()
{
    qDeleteAll(m_sensors.begin(), m_sensors.end());
}

QList<DeviceSensor *> DeviceSensorModel::sensors() const
{
    return m_sensors;
}

void DeviceSensorModel::addSensor(DeviceSensor *sensor)
{
    if (!sensor)
        return;

    beginInsertRows(QModelIndex(), m_sensors.count(), m_sensors.count());
    m_sensors.append(sensor);
    endInsertRows();
}

void DeviceSensorModel::setSensors(const QList<DeviceSensor *> &sensors)
{
    beginResetModel();
    qDeleteAll(m_sensors.begin(), m_sensors.end());
    m_sensors.clear();
    m_sensors = sensors;
    endResetModel();
}

int DeviceSensorModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_sensors.count();
}

QVariant DeviceSensorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const DeviceSensor *sensor = m_sensors.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return sensor->name();

    case EnabledRole:
        return sensor->enabled();

    case NameRole:
        return sensor->name();

    case StateRole:
        return sensor->state();

    case UnitRole:
        return sensor->unit();

    case DeviceClassRole:
        return sensor->deviceClass();

    case UniqueIdRole:
        return sensor->uniqueId();

    default:
        return QVariant();
    }
}

bool DeviceSensorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    DeviceSensor *sensor = m_sensors.at(index.row());

    if (role == EnabledRole) {
        sensor->setEnabled(value.toBool());
        return true;
    }

    return false;
}

QHash<int, QByteArray> DeviceSensorModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[DeviceClassRole]      = "device_class";
    roles[EnabledRole]          = "enabled";
    roles[NameRole]             = "name";
    roles[StateRole]            = "state";
    roles[UniqueIdRole]         = "unique_id";
    roles[UnitRole]             = "unit";

    return roles;
}
