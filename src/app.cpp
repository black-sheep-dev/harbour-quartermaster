#include "app.h"

#include <QSettings>
#include <QCoreApplication>

#include "constants.h"

App::App(QObject *parent) :
    QObject(parent)
{
    // device service
    connect(m_deviceService, &DeviceService::credentialsChanged, m_api, &ApiInterface::setCredentials);
    connect(m_deviceService, &DeviceService::apiRequest, m_api, &ApiInterface::sendRequest);
    connect(m_deviceService, &DeviceService::webhookRequest, m_api, &ApiInterface::sendWebhookRequest);
    connect(m_api, &ApiInterface::requestFinished, m_deviceService, &DeviceService::onRequestFinished);
    connect(m_api, &ApiInterface::requestError, m_deviceService, &DeviceService::onRequestError);

    // location service
    connect(m_locationService, &LocationService::webhookRequest, m_api, &ApiInterface::sendWebhookRequest);
    connect(m_locationService, &LocationService::atHomeChanged, m_api, &ApiInterface::setAtHome);

    // entities service
    connect(m_entitiesService, &EntitiesService::apiRequest, m_api, &ApiInterface::sendRequest);
    connect(m_api, &ApiInterface::requestFinished, m_entitiesService, &EntitiesService::onRequestFinished);
    connect(m_api, &ApiInterface::requestError, m_entitiesService, &EntitiesService::onRequestError);

//    connect(m_api, &ApiConnector::requestDataFinished, m_deviceService, &DeviceService::onRequestFinished);

//    connect(m_api, &ApiConnector::requestDataFinished, this, &App::onRequestDataFinished);
//    connect(m_api, &ApiConnector::requestDataFinished, m_entitiesService, &EntitiesService::onRequestDataFinished);
//    connect(m_api, &ApiConnector::entityStateChanged, m_entitiesService, &EntitiesService::updateEntity);


    readSetting();

    // initialize
    m_deviceService->initialize();
    setNeedSetup(!m_deviceService->isRegistered());
    //setNeedSetup(true);

    m_api->initialize();
}

App::~App()
{
    writeSettings();

    delete m_mutex;
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

void App::initialize()
{
    setNeedSetup(!m_deviceService->isRegistered());

    if (m_needSetup)
        return;

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

    m_deviceService->saveSettings();
    m_locationService->saveSettings();
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
