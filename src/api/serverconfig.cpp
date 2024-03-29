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
{

}

bool ServerConfig::isCompatible()
{
    return m_versionCompatibility
            && (m_components & ComponentMobileApp)
            && (m_components & ComponentWebhook);
}

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

    const QString externalUrl = object.value(ApiKey::KEY_EXTERNAL_URL).toString();
    if (!externalUrl.isEmpty()) {
//        if (!externalUrl.contains(QRegExp(RegExp::PORT_INCLUDED)))
//            externalUrl.append(QStringLiteral(":8123"));

        setExternalUrl(externalUrl);
    }


    const QString internalUrl = object.value(ApiKey::KEY_INTERNAL_URL).toString();

    if (!internalUrl.isEmpty()) {
//        if (!internalUrl.contains(QRegExp(RegExp::PORT_INCLUDED)))
//            internalUrl.append(QStringLiteral(":8123"));

        setInternalUrl(internalUrl);
    }

    setLocationName(object.value(ApiKey::KEY_LOCATION_NAME).toString());
    setLatitude(object.value(ApiKey::KEY_LATITUDE).toDouble());
    setLongitude(object.value(ApiKey::KEY_LONGITUDE).toDouble());

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

const QVariantList &ServerConfig::componentList() const
{
    return m_componentList;
}

quint16 ServerConfig::components() const
{
    return m_components;
}

const QString &ServerConfig::configError() const
{
    return m_configError;
}

bool ServerConfig::configValid() const
{
    return m_configValid;
}

const QString &ServerConfig::externalUrl() const
{
    return m_externalUrl;
}

const QString &ServerConfig::internalUrl() const
{
    return m_internalUrl;
}

double ServerConfig::latitude() const
{
    return m_latitude;
}

const QString &ServerConfig::locationName() const
{
    return m_locationName;
}

double ServerConfig::longitude() const
{
    return m_longitude;
}

const QString ServerConfig::version() const
{
    return m_version.toString();
}

const QString &ServerConfig::unitLength() const
{
    return m_unitLength;
}

const QString &ServerConfig::unitMass() const
{
    return m_unitMass;
}

const QString &ServerConfig::unitPressure() const
{
    return m_unitPressure;
}

const QString &ServerConfig::unitTemperature() const
{
    return m_unitTemperature;
}

const QString &ServerConfig::unitVolume() const
{
    return m_unitVolume;
}

bool ServerConfig::versionCompatibility() const
{
    return m_versionCompatibility;
}

void ServerConfig::setComponentList(const QVariantList &components)
{
    if (m_componentList == components)
        return;

    m_componentList = components;
    emit componentListChanged();
}

void ServerConfig::setComponents(quint16 components)
{
    if (m_components == components)
        return;

    m_components = components;
    emit componentsChanged();
}

void ServerConfig::setConfigError(const QString &error)
{
    if (m_configError == error)
        return;

    m_configError = error;
    emit configErrorChanged();
}

void ServerConfig::setConfigValid(bool valid)
{
    if (m_configValid == valid)
        return;

    m_configValid = valid;
    emit configValidChanged();
}

void ServerConfig::setExternalUrl(const QString &url)
{
    const QString check = cleanUrl(url);

    if (m_externalUrl == check)
        return;

    m_externalUrl = check;
    emit externalUrlChanged();
}

void ServerConfig::setInternalUrl(const QString &url)
{
    const QString check = cleanUrl(url);

    if (m_internalUrl == check)
        return;

    m_internalUrl = check;
    emit internalUrlChanged();
}

void ServerConfig::setLatitude(double latitude)
{
    if (qFuzzyCompare(m_latitude, latitude))
        return;

    m_latitude = latitude;
    emit latitudeChanged();
}

void ServerConfig::setLocationName(const QString &locationName)
{
    if (m_locationName == locationName)
        return;

    m_locationName = locationName;
    emit locationNameChanged();
}

void ServerConfig::setLongitude(double longitude)
{
    if (qFuzzyCompare(m_longitude, longitude))
        return;

    m_longitude = longitude;
    emit longitudeChanged();
}

void ServerConfig::setUnitLength(const QString &unit)
{
    if (m_unitLength == unit)
        return;

    m_unitLength = unit;
    emit unitLengthChanged();
}

void ServerConfig::setUnitMass(const QString &unit)
{
    if (m_unitMass == unit)
        return;

    m_unitMass = unit;
    emit unitMassChanged();
}

void ServerConfig::setUnitPressure(const QString &unit)
{
    if (m_unitPressure == unit)
        return;

    m_unitPressure = unit;
    emit unitPressureChanged();
}

void ServerConfig::setUnitTemperature(const QString &unit)
{
    if (m_unitTemperature == unit)
        return;

    m_unitTemperature = unit;
    emit unitTemperatureChanged();
}

void ServerConfig::setUnitVolume(const QString &unit)
{
    if (m_unitVolume == unit)
        return;

    m_unitVolume = unit;
    emit unitVolumeChanged();
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
    emit versionCompatibilityChanged();
}

QString ServerConfig::cleanUrl(const QString &url) const
{
    QString out = url;

    while (out.endsWith('/')) {
        out.remove(out.length() - 1, 1);
    }

    return out;
}
