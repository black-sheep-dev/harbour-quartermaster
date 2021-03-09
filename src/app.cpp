#include "app.h"

#include <QSettings>
#include <QCoreApplication>

#include "constants.h"

App::App(QObject *parent) :
    QObject(parent)
{
    connect(m_api, &ApiConnector::credentialsChanged, m_wallet, &Wallet::setCredentials);
    connect(m_api, &ApiConnector::requestRegistrationRefresh, this, &App::registerDevice);
    connect(m_wallet, &Wallet::credentialsChanged, m_api, &ApiConnector::setCredentials);
    connect(m_wallet, &Wallet::validityChanged, this, [this](bool valid) {
        this->setNeedSetup(!valid);
    });

    readSetting();

    m_wallet->initialize();
    //m_wallet->reset();
}

App::~App()
{
    writeSettings();
}

ApiConnector *App::api()
{
    return m_api;
}

Device *App::device()
{
    return m_device;
}

Wallet *App::wallet()
{
    return m_wallet;
}

void App::initialize()
{
    setNeedSetup(!m_wallet->isValid() || m_wallet->lastError() > 0);

    if (m_needSetup)
        return;

    initializeApiData();
}

void App::registerDevice()
{
    QJsonObject data;
    data.insert(ApiKey::KEY_DEVICE_ID, m_device->id());
    data.insert(ApiKey::KEY_APP_ID, QStringLiteral("org.nubecula.harbour.quartermaster"));
    data.insert(ApiKey::KEY_APP_NAME, QCoreApplication::applicationName());
    data.insert(ApiKey::KEY_APP_VERSION, QCoreApplication::applicationVersion());
    data.insert(ApiKey::KEY_DEVICE_NAME, m_device->name());
    data.insert(ApiKey::KEY_MANUFACTURER, m_device->manufacturer());
    data.insert(ApiKey::KEY_MODEL, m_device->model());
    data.insert(ApiKey::KEY_OS_NAME, m_device->softwareName());
    data.insert(ApiKey::KEY_OS_VERSION, m_device->softwareVersion());
    data.insert(ApiKey::KEY_SUPPORTS_ENCRYPTION, m_api->encryption());

    m_api->registerDevice(data);
}

void App::reset()
{
    m_wallet->reset();
}

void App::saveSettings()
{
    writeSettings();
}

void App::updateRegistration()
{
    QJsonObject data;

    data.insert(ApiKey::KEY_APP_VERSION, QCoreApplication::applicationVersion());
    data.insert(ApiKey::KEY_DEVICE_NAME, m_device->name());
    data.insert(ApiKey::KEY_OS_VERSION, m_device->softwareVersion());
    data.insert(ApiKey::KEY_MANUFACTURER, m_device->manufacturer());
    data.insert(ApiKey::KEY_MODEL, m_device->model());

    m_api->sendWebhookRequest(ApiConnector::RequestWebhookUpdateRegistration, data);
}

bool App::needSetup() const
{
    return m_needSetup;
    //return true;
}

void App::setNeedSetup(bool needSetup)
{
    if (m_needSetup == needSetup)
        return;

    m_needSetup = needSetup;
    emit needSetupChanged(m_needSetup);
}

void App::onApiError(quint8 code, const QString &msg)
{

}

void App::onRequestFinished(quint8 type, const QJsonDocument &payload)
{

}

void App::onWebhookRequestFinished(quint8 type, const QJsonDocument &payload)
{

}

void App::initializeApiData()
{
    m_api->getConfig();
    //m_api->getStates();
    updateRegistration();
}

void App::readSetting()
{
    QSettings settings;

    quint16 externalPort{0};
    QString externalUrl;
    quint16 internalPort{0};
    QString internalUrl;

    if (settings.childGroups().contains(QStringLiteral("API"))) {
        settings.beginGroup(QStringLiteral("API"));
        const bool ssl = settings.value(QStringLiteral("ssl"), false).toBool();
        const quint16 port = quint16(settings.value(QStringLiteral("port"), 8123).toInt());
        const QString hostname = settings.value(QStringLiteral("hostname"), QString()).toString();

        if (hostname.startsWith(QLatin1String("http")))
            internalUrl = hostname;
        else
            internalUrl = (ssl ? QStringLiteral("https://") : QStringLiteral("http://")) + hostname;

        internalPort = port;
        externalPort = port;

        settings.remove("");
        settings.endGroup();

    } else {
        settings.beginGroup(QStringLiteral("CONNECTION"));
        externalPort = quint16(settings.value(QStringLiteral("external_port"), 8123).toUInt());
        externalUrl = settings.value(QStringLiteral("external_url")).toString();
        internalPort = quint16(settings.value(QStringLiteral("internal_port"), 8123).toUInt());
        internalUrl = settings.value(QStringLiteral("internal_url")).toString();
        settings.endGroup();
    }

    if (settings.childGroups().contains(QStringLiteral("WEBHOOK_API"))) {
        settings.beginGroup(QStringLiteral("WEBHOOK_API"));
        settings.remove("");
        settings.endGroup();
    }

    m_api->serverConfig()->setExternalPort(externalPort);
    m_api->serverConfig()->setExternalUrl(externalUrl);
    m_api->serverConfig()->setInternalPort(internalPort);
    m_api->serverConfig()->setInternalUrl(internalUrl);

    settings.beginGroup(QStringLiteral("DEVELOPER_MODE"));
    m_api->setLogging(settings.value(QStringLiteral("api_logging"), false).toBool());
    settings.endGroup();
}

void App::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("CONNECTION"));
    settings.setValue(QStringLiteral("external_port"), m_api->serverConfig()->externalPort());
    settings.setValue(QStringLiteral("external_url"), m_api->serverConfig()->externalUrl());
    settings.setValue(QStringLiteral("internal_port"), m_api->serverConfig()->internalPort());
    settings.setValue(QStringLiteral("internal_url"), m_api->serverConfig()->internalUrl());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVELOPER_MODE"));
    settings.setValue(QStringLiteral("api_logging"), m_api->logging());
    settings.endGroup();
}
