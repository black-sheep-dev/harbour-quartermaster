#include "app.h"

#include <QSettings>
#include <QCoreApplication>

#include "constants.h"

App::App(QObject *parent) :
    QObject(parent)
{
    m_deviceService->setApi(m_api);
    m_locationService->setApi(m_api);
    m_entitiesService->setApi(m_api);
    m_notificationService->setApi(m_api);

    connect(m_entitiesService, &EntitiesService::homeAssistantUpdateAvailable,
            m_notificationService, &NotificationService::onHomeAssistantUpdateAvailable);

    readSetting();

    // initialize
    m_deviceService->initialize();
    setNeedSetup(!m_deviceService->isRegistered());
    // setNeedSetup(true);
    //m_api->setAtHome(false);
    m_api->initialize();
}

App::~App()
{
    writeSettings();
}

ApiInterface *App::api()
{
    return m_api;
}

DeviceService *App::deviceService()
{
    return m_deviceService;
}

EntitiesService *App::entitiesService()
{
    return m_entitiesService;
}

LocationService *App::locationService()
{
    return m_locationService;
}

NotificationService *App::notificationService()
{
    return m_notificationService;
}

void App::initialize()
{
    if (m_needSetup)
        return;

    m_notificationService->initialize();
    m_locationService->initialize();
    m_entitiesService->initialize();
}

void App::reset()
{
    m_deviceService->resetRegistration();
}

void App::saveSettings()
{
    writeSettings();

    m_api->saveSettings();
    m_deviceService->saveSettings();
    m_entitiesService->saveSettings();
    m_locationService->saveSettings();
    m_notificationService->saveSettings();
}

bool App::needSetup() const
{
    return m_needSetup;
}

void App::setNeedSetup(bool needSetup)
{
    if (m_needSetup == needSetup)
        return;

    m_needSetup = needSetup;
    emit needSetupChanged(m_needSetup);
}

void App::onError(quint8 code, const QString &msg)
{
#ifdef QT_DEBUG
    qDebug() << code;
    qDebug() << msg;
#else
    Q_UNUSED(code)
    Q_UNUSED(msg)
#endif
}

void App::readSetting()
{
    QSettings settings;

    // developer mode
    settings.beginGroup(QStringLiteral("DEVELOPER_MODE"));
    m_api->setLogging(settings.value(QStringLiteral("api_logging"), false).toBool());
    settings.endGroup();
}

void App::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("DEVELOPER_MODE"));
    settings.setValue(QStringLiteral("api_logging"), m_api->logging());
    settings.endGroup();
}
