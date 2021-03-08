#include "serverconfig.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>

#include "src/constants.h"

ServerConfig::ServerConfig(QObject *parent) :
    QObject(parent)
//    m_zone(new Zone(this))
{

}

bool ServerConfig::isCompatible()
{
    return m_versionCompatibility
            && (m_components & ComponentMobileApp)
            && (m_components & ComponentWebhook);
}

//Zone *ServerConfig::homezone() const
//{
//    return m_zone;
//}


bool ServerConfig::isUpdateAvailable(const QString &version) const
{
    const QVersionNumber v = QVersionNumber::fromString(version);

    return v > m_version;
}

void ServerConfig::setData(const QJsonObject &object)
{
    if (object.isEmpty()) {
        return;
    }

    const QVariantList components = object.value(ApiKey::KEY_COMPONENTS).toArray().toVariantList();
    setComponentList(components);

    setExternalUrl(object.value(ApiKey::KEY_EXTERNAL_URL).toString());
    setInternalUrl(object.value(ApiKey::KEY_INTERNAL_URL).toString());
//    setLocationName(object.value(ApiKey::KEY_LOCATION_NAME).toString());
    //m_zone->setLatitude(object.value(ApiKey::KEY_LATITUDE).toDouble());
    //m_zone->setLongitude(object.value(ApiKey::KEY_LONGITUDE).toDouble());

    // check version
    const QString version = object.value(ApiKey::KEY_VERSION).toString();
    setVersion(version);

    // component flags
    quint16 flags = ComponentNone;

    if (components.contains(QLatin1String("mobile_app"))) {
        flags |= ComponentMobileApp;
    }

    if (components.contains(QLatin1String("webhook"))) {
        flags |= ComponentWebhook;
    }

    if (components.contains(QLatin1String("websocket_api"))) {
        flags |= ComponentWebsocketApi;
    }

    if (components.contains(QLatin1String("config"))) {
        flags |= ComponentConfig;
    }

    setComponents(flags);

    // units
    const QJsonObject units = object.value(ApiKey::KEY_UNIT_SYSTEM).toObject();

    m_unitLength = units.value(ApiKey::KEY_LENGTH).toString();
    m_unitMass = units.value(ApiKey::KEY_MASS).toString();
    m_unitPressure = units.value(ApiKey::KEY_PRESSURE).toString();
    m_unitTemperature = units.value(ApiKey::KEY_TEMPERATURE).toString();
    m_unitVolume = units.value(ApiKey::KEY_VOLUME).toString();
}

bool ServerConfig::available() const
{
    return m_available;
}

QVariantList ServerConfig::componentList() const
{
    return m_componentList;
}

quint16 ServerConfig::components() const
{
    return m_components;
}

QString ServerConfig::configError() const
{
    return m_configError;
}

bool ServerConfig::configValid() const
{
    return m_configValid;
}

QString ServerConfig::error() const
{
    return m_error;
}

quint16 ServerConfig::externalPort() const
{
    return m_externalPort;
}

QString ServerConfig::externalUrl() const
{
    return m_externalUrl;
}

quint16 ServerConfig::internalPort() const
{
    return m_internalPort;
}

QString ServerConfig::internalUrl() const
{
    return m_internalUrl;
}

bool ServerConfig::loading() const
{
    return m_loading;
}

//QString ServerConfig::locationName() const
//{
//    return QString();//m_zone->name();
//}

QString ServerConfig::version() const
{
    return m_version.toString();
}

QString ServerConfig::unitLength() const
{
    return m_unitLength;
}

QString ServerConfig::unitMass() const
{
    return m_unitMass;
}

QString ServerConfig::unitPressure() const
{
    return m_unitPressure;
}

QString ServerConfig::unitTemperature() const
{
    return m_unitTemperature;
}

QString ServerConfig::unitVolume() const
{
    return m_unitVolume;
}

bool ServerConfig::versionCompatibility() const
{
    return m_versionCompatibility;
}

void ServerConfig::setAvailable(bool available)
{
    if (m_available == available)
        return;

    m_available = available;
    emit availableChanged(m_available);

    // reset error
    m_error = QString();
}

void ServerConfig::setComponentList(const QVariantList &components)
{
    if (m_componentList == components)
        return;

    m_componentList = components;
    emit componentListChanged(m_componentList);
}

void ServerConfig::setComponents(quint16 components)
{
    if (m_components == components)
        return;

    m_components = components;
    emit componentsChanged(m_components);
}

void ServerConfig::setConfigError(const QString &error)
{
    if (m_configError == error)
        return;

    m_configError = error;
    emit configErrorChanged(m_configError);
}

void ServerConfig::setConfigValid(bool valid)
{
    if (m_configValid == valid)
        return;

    m_configValid = valid;
    emit configValidChanged(m_configValid);
}

void ServerConfig::setError(const QString &error)
{
    if (m_error == error)
        return;

    m_error = error;
    emit errorChanged(m_error);
}

void ServerConfig::setExternalPort(quint16 port)
{
    if (m_externalPort == port)
        return;

    m_externalPort = port;
    emit externalPortChanged(m_externalPort);
}

void ServerConfig::setExternalUrl(const QString &url)
{
    if (m_externalUrl == url)
        return;

    m_externalUrl = url;
    emit externalUrlChanged(m_externalUrl);
}

void ServerConfig::setInternalPort(quint16 internalPort)
{
    if (m_internalPort == internalPort)
        return;

    m_internalPort = internalPort;
    emit internalPortChanged(m_internalPort);
}

void ServerConfig::setInternalUrl(const QString &url)
{
    if (m_internalUrl == url)
        return;

    m_internalUrl = url;
    emit internalUrlChanged(m_internalUrl);
}

void ServerConfig::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

//void ServerConfig::setLocationName(const QString &name)
//{
//    if (m_zone->name() == name)
//        return;

//    m_zone->setName(name);
//    emit locationNameChanged(m_locationName);
//}

void ServerConfig::setUnitLength(const QString &unit)
{
    if (m_unitLength == unit)
        return;

    m_unitLength = unit;
    emit unitLengthChanged(m_unitLength);
}

void ServerConfig::setUnitMass(const QString &unit)
{
    if (m_unitMass == unit)
        return;

    m_unitMass = unit;
    emit unitMassChanged(m_unitMass);
}

void ServerConfig::setUnitPressure(const QString &unit)
{
    if (m_unitPressure == unit)
        return;

    m_unitPressure = unit;
    emit unitPressureChanged(m_unitPressure);
}

void ServerConfig::setUnitTemperature(const QString &unit)
{
    if (m_unitTemperature == unit)
        return;

    m_unitTemperature = unit;
    emit unitTemperatureChanged(m_unitTemperature);
}

void ServerConfig::setUnitVolume(const QString &unit)
{
    if (m_unitVolume == unit)
        return;

    m_unitVolume = unit;
    emit unitVolumeChanged(m_unitVolume);
}

void ServerConfig::setVersion(const QString &version)
{
    const QVersionNumber v = QVersionNumber::fromString(version);

    if (m_version == v)
        return;

    m_version = v;
    emit versionChanged(m_version.toString());

    // check compatibility
    if (v.majorVersion() >= 2020) {
        setVersionCompatibility(true);
    } else {
        setVersionCompatibility(v.minorVersion() >= 90);
    }
}

void ServerConfig::setVersionCompatibility(bool compatibility)
{
    if (m_versionCompatibility == compatibility)
        return;

    m_versionCompatibility = compatibility;
    emit versionCompatibilityChanged(m_versionCompatibility);
}
