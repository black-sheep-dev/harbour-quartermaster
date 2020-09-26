#include "devicesensor.h"

DeviceSensor::DeviceSensor(QObject *parent) :
    QObject(parent),
    m_deviceClass(QStringLiteral("None")),
    m_enabled(false),
    m_name(QString()),
    m_sensorType(QStringLiteral("sensor")),
    m_state(QVariant(0)),
    m_uniqueId(QString()),
    m_unit(QString())
{

}

QJsonObject DeviceSensor::toJson() const
{
    QJsonObject sensor = getBaseSensorJson();
    sensor.insert(QStringLiteral("device_class"), m_deviceClass);
    sensor.insert(QStringLiteral("icon"), getIcon());
    sensor.insert(QStringLiteral("name"), m_name);
    sensor.insert(QStringLiteral("unit_of_measurement"), m_unit);

    return sensor;
}

QJsonObject DeviceSensor::getBaseSensorJson() const
{
    QJsonObject sensor;
    sensor.insert(QStringLiteral("icon"), getIcon());
    sensor.insert(QStringLiteral("type"), m_sensorType);
    sensor.insert(QStringLiteral("state"), getStateValue());
    sensor.insert(QStringLiteral("unique_id"), m_uniqueId);

    return sensor;
}

QString DeviceSensor::deviceClass() const
{
    return m_deviceClass;
}

bool DeviceSensor::enabled() const
{
    return m_enabled;
}

QString DeviceSensor::icon() const
{
    return m_icon;
}

QString DeviceSensor::name() const
{
    return m_name;
}

QString DeviceSensor::sensorType() const
{
    return m_sensorType;
}

QVariant DeviceSensor::state() const
{
    return m_state;
}

QString DeviceSensor::uniqueId() const
{
    return m_uniqueId;
}

QString DeviceSensor::unit() const
{
    return m_unit;
}

void DeviceSensor::setDeviceClass(const QString &deviceClass)
{
    if (m_deviceClass == deviceClass)
        return;

    m_deviceClass = deviceClass;
    emit deviceClassChanged(m_deviceClass);
}

void DeviceSensor::setEnabled(bool enable)
{
    if (m_enabled == enable)
        return;

    m_enabled = enable;
    emit enabledChanged(m_enabled);
}

void DeviceSensor::setIcon(const QString &icon)
{
    if (m_icon == icon)
        return;

    m_icon = icon;
    emit iconChanged(m_icon);
}

void DeviceSensor::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void DeviceSensor::setSensorType(const QString &sensorType)
{
    if (m_sensorType == sensorType)
        return;

    m_sensorType = sensorType;
    emit sensorTypeChanged(m_sensorType);
}

void DeviceSensor::setState(const QVariant &state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);

    onStateChanged();
}

void DeviceSensor::setUniqueId(const QString &uniqueId)
{
    if (m_uniqueId == uniqueId)
        return;

    m_uniqueId = uniqueId;
    emit uniqueIdChanged(m_uniqueId);
}

void DeviceSensor::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;

    m_unit = unit;
    emit unitChanged(m_unit);
}

void DeviceSensor::onStateChanged()
{
    emit sensorUpdated(getBaseSensorJson());
}

QJsonValue DeviceSensor::getStateValue() const
{
    QJsonValue state;

    switch (m_state.type()) {
    case QVariant::Bool:
        state = m_state.toBool();
        break;
    case QVariant::Double:
        state = m_state.toDouble();
        break;
    case QVariant::String:
        state = m_state.toString();
        break;
    case QVariant::Int:
        state = m_state.toInt();
        break;

    default:
        state = QStringLiteral("unknown");
        break;
    }

    return state;
}

QString DeviceSensor::getIcon() const
{
    return QString();
}

void DeviceSensor::onEnabledChanged()
{

}
