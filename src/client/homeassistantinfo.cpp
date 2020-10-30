#include "homeassistantinfo.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>

#include "src/string_constants.h"

HomeassistantInfo::HomeassistantInfo(QObject *parent) :
    QObject(parent),
    m_zone(new Zone(this)),
    m_error(QString()),
    m_externalUrl(QString()),
    m_internalUrl(QString()),
    m_locationName(QString()),
    m_version(QString())
{

}

Zone *HomeassistantInfo::homezone() const
{
    return m_zone;
}

bool HomeassistantInfo::isInstanceValid()
{
    return m_versionCompatibility && (m_components & ComponentMobileApp) && (m_components & ComponentWebhook);
}

bool HomeassistantInfo::isUpdateAvailable(const QString &version) const
{
    if (m_version.isEmpty())
        return false;

    const QStringList parts = version.split(".");

    if (parts.count() != 3)
        return false;

    if (parts.at(0).toInt() > m_majorVersion)
        return true;

    if (parts.at(1).toInt() > m_minorVersion)
        return true;

    if (parts.at(2).toInt() > m_buildVersion)
        return true;

    return false;
}

void HomeassistantInfo::setData(const QJsonObject &object)
{
    if (object.isEmpty()) {
        return;
    }

    const QVariantList components = object.value(API_KEY_COMPONENTS).toArray().toVariantList();
    setComponentList(components);

    setExternalUrl(object.value(API_KEY_EXTERNAL_URL).toString());
    setInternalUrl(object.value(API_KEY_INTERNAL_URL).toString());
    setLocationName(object.value(API_KEY_LOCATION_NAME).toString());
    m_zone->setLatitude(object.value(API_KEY_LATITUDE).toDouble());
    m_zone->setLongitude(object.value(API_KEY_LONGITUDE).toDouble());

    // check version
    const QString version = object.value(API_KEY_VERSION).toString();
    setVersion(version);

    const QStringList parts = version.split(".");

    if (parts.size() > 2)
        setVersionCompatibility(parts.at(1).toInt() >= 90);


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
}

bool HomeassistantInfo::available() const
{
    return m_available;
}

QVariantList HomeassistantInfo::componentList() const
{
    return m_componentList;
}

quint16 HomeassistantInfo::components() const
{
    return m_components;
}

QString HomeassistantInfo::configError() const
{
    return m_configError;
}

bool HomeassistantInfo::configValid() const
{
    return m_configValid;
}

QString HomeassistantInfo::error() const
{
    return m_error;
}

QString HomeassistantInfo::externalUrl() const
{
    return m_externalUrl;
}

QString HomeassistantInfo::internalUrl() const
{
    return m_internalUrl;
}

bool HomeassistantInfo::loading() const
{
    return m_loading;
}

QString HomeassistantInfo::locationName() const
{
    return m_zone->name();
}

QString HomeassistantInfo::version() const
{
    return m_version;
}

QString HomeassistantInfo::unitLength() const
{
    return m_unitLength;
}

QString HomeassistantInfo::unitMass() const
{
    return m_unitMass;
}

QString HomeassistantInfo::unitPressure() const
{
    return m_unitPressure;
}

QString HomeassistantInfo::unitTemperature() const
{
    return m_unitTemperature;
}

QString HomeassistantInfo::unitVolume() const
{
    return m_unitVolume;
}

bool HomeassistantInfo::versionCompatibility() const
{
    return m_versionCompatibility;
}

void HomeassistantInfo::setAvailable(bool available)
{
    if (m_available == available)
        return;

    m_available = available;
    emit availableChanged(m_available);

    // reset error
    m_error = QString();
}

void HomeassistantInfo::setComponentList(const QVariantList &components)
{
    if (m_componentList == components)
        return;

    m_componentList = components;
    emit componentListChanged(m_componentList);
}

void HomeassistantInfo::setComponents(quint16 components)
{
    if (m_components == components)
        return;

    m_components = components;
    emit componentsChanged(m_components);
}

void HomeassistantInfo::setConfigError(const QString &error)
{
    if (m_configError == error)
        return;

    m_configError = error;
    emit configErrorChanged(m_configError);
}

void HomeassistantInfo::setConfigValid(bool valid)
{
    if (m_configValid == valid)
        return;

    m_configValid = valid;
    emit configValidChanged(m_configValid);
}

void HomeassistantInfo::setError(const QString &error)
{
    if (m_error == error)
        return;

    m_error = error;
    emit errorChanged(m_error);
}

void HomeassistantInfo::setExternalUrl(const QString &url)
{
    if (m_externalUrl == url)
        return;

    m_externalUrl = url;
    emit externalUrlChanged(m_externalUrl);
}

void HomeassistantInfo::setInternalUrl(const QString &url)
{
    if (m_internalUrl == url)
        return;

    m_internalUrl = url;
    emit internalUrlChanged(m_internalUrl);
}

void HomeassistantInfo::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

void HomeassistantInfo::setLocationName(const QString &name)
{
    if (m_zone->name() == name)
        return;

    m_zone->setName(name);
    emit locationNameChanged(m_locationName);
}

void HomeassistantInfo::setUnitLength(const QString &unit)
{
    if (m_unitLength == unit)
        return;

    m_unitLength = unit;
    emit unitLengthChanged(m_unitLength);
}

void HomeassistantInfo::setUnitMass(const QString &unit)
{
    if (m_unitMass == unit)
        return;

    m_unitMass = unit;
    emit unitMassChanged(m_unitMass);
}

void HomeassistantInfo::setUnitPressure(const QString &unit)
{
    if (m_unitPressure == unit)
        return;

    m_unitPressure = unit;
    emit unitPressureChanged(m_unitPressure);
}

void HomeassistantInfo::setUnitTemperature(const QString &unit)
{
    if (m_unitTemperature == unit)
        return;

    m_unitTemperature = unit;
    emit unitTemperatureChanged(m_unitTemperature);
}

void HomeassistantInfo::setUnitVolume(const QString &unit)
{
    if (m_unitVolume == unit)
        return;

    m_unitVolume = unit;
    emit unitVolumeChanged(m_unitVolume);
}

void HomeassistantInfo::setVersion(const QString &version)
{
    if (m_version == version)
        return;

    m_version = version;
    emit versionChanged(m_version);

    // split version
    const QStringList parts = version.split(".");
    if (parts.count() != 3)
        return;

    m_majorVersion = quint16(parts.at(0).toInt());
    m_minorVersion = quint16(parts.at(1).toInt());
    m_buildVersion = quint16(parts.at(2).toInt());
}

void HomeassistantInfo::setVersionCompatibility(bool compatibility)
{
    if (m_versionCompatibility == compatibility)
        return;

    m_versionCompatibility = compatibility;
    emit versionCompatibilityChanged(m_versionCompatibility);
}
