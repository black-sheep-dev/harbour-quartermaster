#include "homeassistantinfo.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>

HomeassistantInfo::HomeassistantInfo(QObject *parent) :
    QObject(parent),
    m_available(false),
    m_componentList(QVariantList()),
    m_components(ComponentNone),
    m_error(QString()),
    m_externalUrl(QString()),
    m_internalUrl(QString()),
    m_loading(false),
    m_locationName(QString()),
    m_version(QString()),
    m_versionCompatibility(false)
{

}

bool HomeassistantInfo::isInstanceValid()
{
    return m_versionCompatibility && (m_components & ComponentMobileApp) && (m_components & ComponentWebhook);
}

void HomeassistantInfo::setData(const QJsonObject &object)
{
    if (object.isEmpty()) {
        return;
    }

    const QVariantList components = object.value(QStringLiteral("components")).toArray().toVariantList();
    setComponentList(components);

    setExternalUrl(object.value(QStringLiteral("external_url")).toString());
    setInternalUrl(object.value(QStringLiteral("internal_url")).toString());
    setLocationName(object.value(QStringLiteral("location_name")).toString());


    // check version
    const QString version = object.value(QStringLiteral("version")).toString();
    setVersion(version);

    const QStringList parts = version.split(".");

    if (parts.size() > 2)
        setVersionCompatibility(parts.at(1).toInt() >= 90);


    // component flags
    Components flags = ComponentNone;

    if (components.contains(QStringLiteral("mobile_app"))) {
        flags |= ComponentMobileApp;
    }

    if (components.contains(QStringLiteral("webhook"))) {
        flags |= ComponentWebhook;
    }

    if (components.contains(QStringLiteral("websocket_api"))) {
        flags |= ComponentWebsocketApi;
    }

    setComponents(flags);
}

bool HomeassistantInfo::available() const
{
    return m_available;
}

QVariantList HomeassistantInfo::componentList() const
{
    return m_componentList;
}

HomeassistantInfo::Components HomeassistantInfo::components() const
{
    return m_components;
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
    return m_locationName;
}

QString HomeassistantInfo::version() const
{
    return m_version;
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

void HomeassistantInfo::setComponents(Components components)
{
    if (m_components == components)
        return;

    m_components = components;
    emit componentsChanged(m_components);
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
    if (m_locationName == name)
        return;

    m_locationName = name;
    emit locationNameChanged(m_locationName);
}

void HomeassistantInfo::setVersion(const QString &version)
{
    if (m_version == version)
        return;

    m_version = version;
    emit versionChanged(m_version);
}

void HomeassistantInfo::setVersionCompatibility(bool compatibility)
{
    if (m_versionCompatibility == compatibility)
        return;

    m_versionCompatibility = compatibility;
    emit versionCompatibilityChanged(m_versionCompatibility);
}
