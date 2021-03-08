#include "app.h"

#include <QSettings>

App::App(QObject *parent) :
    QObject(parent)
{
    readSetting();

    connect(m_api, &ApiConnector::credentialsChanged, m_wallet, &Wallet::setCredentials);
    connect(m_wallet, &Wallet::credentialsChanged, m_api, &ApiConnector::setCredentials);
    connect(m_wallet, &Wallet::validityChanged, this, [this](bool valid) {
        this->setNeedSetup(!valid);
    });

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
}

void App::registerDevice()
{

}

void App::saveSettings()
{
    writeSettings();
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
        externalPort = quint16(settings.value(QStringLiteral("external_port"), 8123).toUInt());
        externalUrl = settings.value(QStringLiteral("external_url")).toString();
        internalPort = quint16(settings.value(QStringLiteral("internal_port"), 8123).toUInt());
        internalUrl = settings.value(QStringLiteral("internal_url")).toString();
    }

    if (settings.childGroups().contains(QStringLiteral("WEBHOOK_API"))) {
        settings.beginGroup(QStringLiteral("WEBHOOK_API"));
        settings.remove("");
        settings.endGroup();
    }

    settings.beginGroup(QStringLiteral("CONNECTION"));

    m_api->serverConfig()->setExternalPort(externalPort);
    m_api->serverConfig()->setExternalUrl(externalUrl);
    m_api->serverConfig()->setExternalPort(internalPort);
    m_api->serverConfig()->setInternalUrl(internalUrl);

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
}
