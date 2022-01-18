#include "devicesensor.h"

#include "src/constants.h"

DeviceSensor::DeviceSensor(QObject *parent) :
    QObject(parent),
    m_deviceClass(QStringLiteral("None")),
    m_sensorType(ApiKey::KEY_SENSOR),
    m_state(QVariant(0))
{

}

QJsonObject DeviceSensor::toJson()
{
    QJsonObject sensor = getBaseSensorJson();
    sensor.insert(ApiKey::KEY_DEVICE_CLASS, m_deviceClass);
    sensor.insert(ApiKey::KEY_ICON, getIcon());
    sensor.insert(ApiKey::KEY_NAME, m_name);
    sensor.insert(ApiKey::KEY_UNIT_OF_MEASUREMENT, m_unit);

    return sensor;
}

QJsonObject DeviceSensor::getBaseSensorJson()
{
    refreshState();

    QJsonObject sensor;
    sensor.insert(ApiKey::KEY_ICON, getIcon());
    sensor.insert(ApiKey::KEY_TYPE, m_sensorType);
    sensor.insert(ApiKey::KEY_STATE, getStateValue());
    sensor.insert(ApiKey::KEY_UNIQUE_ID, m_uniqueId);

    return sensor;
}

const QString &DeviceSensor::deviceClass() const
{
    return m_deviceClass;
}

bool DeviceSensor::enabled() const
{
    return m_enabled;
}

const QString &DeviceSensor::icon() const
{
    return m_icon;
}

const QString &DeviceSensor::name() const
{
    return m_name;
}

const QString &DeviceSensor::sensorType() const
{
    return m_sensorType;
}

const QVariant &DeviceSensor::state() const
{
    return m_state;
}

const QString &DeviceSensor::uniqueId() const
{
    return m_uniqueId;
}

const QString &DeviceSensor::unit() const
{
    return m_unit;
}

void DeviceSensor::setDeviceClass(const QString &deviceClass)
{
    if (m_deviceClass == deviceClass)
        return;

    m_deviceClass = deviceClass;
    emit deviceClassChanged();
}

void DeviceSensor::setEnabled(bool enable)
{
    if (m_enabled == enable)
        return;

    m_enabled = enable;
    emit enabledChanged();

    onEnabledChanged();
}

void DeviceSensor::setIcon(const QString &icon)
{
    if (m_icon == icon)
        return;

    m_icon = icon;
    emit iconChanged();
}

void DeviceSensor::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged();
}

void DeviceSensor::setSensorType(const QString &sensorType)
{
    if (m_sensorType == sensorType)
        return;

    m_sensorType = sensorType;
    emit sensorTypeChanged();
}

void DeviceSensor::setState(const QVariant &state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged();

    onStateChanged();
}

void DeviceSensor::setUniqueId(const QString &uniqueId)
{
    if (m_uniqueId == uniqueId)
        return;

    m_uniqueId = uniqueId;
    emit uniqueIdChanged();
}

void DeviceSensor::setUnit(const QString &unit)
{
    if (m_unit == unit)
        return;

    m_unit = unit;
    emit unitChanged();
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

void DeviceSensor::onStateChanged()
{
    emit sensorUpdated(getBaseSensorJson());
}

QString DeviceSensor::getIcon() const
{
    return QString();
}

void DeviceSensor::onEnabledChanged()
{

}

void DeviceSensor::refreshState()
{

}
