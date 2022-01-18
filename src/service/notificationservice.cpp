#include "notificationservice.h"

#include <QFile>
#include <QSettings>

#include <nemonotifications-qt5/notification.h>

#include "src/constants.h"
#include "src/global.h"

NotificationService::NotificationService(QObject *parent) :
    Service(parent)
{
    readSettings();
}

NotificationService::~NotificationService()
{
    writeSettings();
}

void NotificationService::setApi(ApiInterface *api)
{
    Service::setApi(api);

    setHomeAssistantVersion(api->serverConfig()->version());
    connect(api->serverConfig(), &ServerConfig::versionChanged, this, &NotificationService::setHomeAssistantVersion);
}

QString NotificationService::homeAssistantVersion() const
{
    return m_homeAssistantVersion;
}

quint8 NotificationService::options() const
{
    return m_options;
}

void NotificationService::onHomeAssistantUpdateAvailable(const QString &version)
{
    if (version.isEmpty())
        return;

    if (m_availableVersion == version)
        return;

    m_availableVersion = version;

    sendUpdateNotification();
}

void NotificationService::setHomeAssistantVersion(const QString &version)
{
    if (m_homeAssistantVersion == version)
        return;

    m_homeAssistantVersion = version;
    emit homeAssistantVersionChanged(m_homeAssistantVersion);

    sendUpdateNotification();
}

void NotificationService::setOptions(quint8 options)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Notification Options: ") << options;
#endif

    if (m_options == options)
        return;

    updateOptions(m_options, options);

    m_options = options;
    emit optionsChanged(m_options);
}

void NotificationService::sendUpdateNotification()
{

    if (!(m_options & NotificationOption::ServerUpdates))
        return;

    if (m_availableVersion.isEmpty() || m_homeAssistantVersion.isEmpty())
        return;

    if (m_homeAssistantVersion == m_availableVersion)
        return;

    Notification notify;
    notify.setCategory(QStringLiteral("x-nemo.software-update"));
    //% "There is an update to version %1 available."
    notify.setBody(qtTrId("id-notification-update-available-body").arg(m_availableVersion));
    //% "Home Assistant update available!"
    notify.setSummary(qtTrId("id-notification-update-available-summary"));
    notify.setIcon(QStringLiteral("image://theme/icon-lock-application-update"));
    notify.publish();
}

void NotificationService::updateOptions(quint8 before, quint8 after)
{
    if (api() == nullptr)
        return;

    // websocket events
    if ( !(before & NotificationOption::WebsocketEvents)
         && (after & NotificationOption::WebsocketEvents) ) {

        connect(api(), &ApiInterface::websocketEvent, this, &NotificationService::onWebsocketEvent);
        api()->addWebsocketEventsSubscription(ApiKey::KEY_CALL_SERVICE);

    } else if ( (before & NotificationOption::WebsocketEvents)
                && !(after & NotificationOption::WebsocketEvents) ) {

        disconnect(api(), &ApiInterface::websocketEvent, this, &NotificationService::onWebsocketEvent);
        api()->removeWebsocketEventsSubscription(ApiKey::KEY_CALL_SERVICE);
    }
}

void NotificationService::readSettings()
{
    QSettings settings(QFile(DEFAULT_CONFIG_FILE).exists() ? DEFAULT_CONFIG_FILE : DEPRECATED_CONFIG_FILE, QSettings::NativeFormat);
    settings.beginGroup(QStringLiteral("NOTIFICATIONS"));
    setOptions(settings.value(QStringLiteral("options"), NotificationOption::None).toUInt());
    settings.endGroup();
}

void NotificationService::writeSettings()
{
    QSettings settings(DEFAULT_CONFIG_FILE, QSettings::NativeFormat);

    settings.beginGroup(QStringLiteral("NOTIFICATIONS"));
    settings.setValue(QStringLiteral("options"), m_options);
    settings.endGroup();
}


void NotificationService::initialize()
{
    setState(ServiceState::StateInitializing);
    updateOptions(0, m_options);
    setState(ServiceState::StateInitialized);
}

void NotificationService::onWebsocketEvent(const QString &event, const QJsonValue &data)
{
    if (event != ApiKey::KEY_CALL_SERVICE)
        return;

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket Notification");
    qDebug() << data;
#endif

    const auto obj = data.toObject();

    if ( obj.value(QStringLiteral("domain")).toString() == QStringLiteral("notify")
         && obj.value(QStringLiteral("service")).toString() == QStringLiteral("notify") ) {

        const auto serviceData = obj.value(QStringLiteral("service_data")).toObject();

        Notification notify;
        notify.setCategory("x-nemo.messaging.sms");
        notify.setAppIcon(QStringLiteral("image://theme/harbour-quartermaster"));
        notify.setBody(serviceData.value(QStringLiteral("message")).toString());
        notify.setSummary(serviceData.value(QStringLiteral("title")).toString());
        //notify.setIcon(QStringLiteral("image://theme/icon-lock-information"));
        notify.setUrgency(Notification::Critical);
        //notify.setHintValue("sound-file", "/usr/share/sounds/jolla-ringtones/stereo/poppy-red-alert-1.ogg");
        notify.publish();
    }
}
