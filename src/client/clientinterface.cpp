#include "clientinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

ClientInterface::ClientInterface(QObject *parent) :
    QObject(parent),
    m_api(new HomeassistantApi(this)),
    m_webhook(new WebhookApi(this)),
    m_device(new Device(this)),
    m_homeassistantInfo(new HomeassistantInfo(this)),
    m_busy(false)
{
    connect(m_api, &HomeassistantApi::tokenChanged, this, &ClientInterface::tokenChanged);
    connect(m_api, &HomeassistantApi::dataAvailable, this, &ClientInterface::onDataAvailable);
    connect(m_device, &Device::sensorUpdated, m_webhook, &WebhookApi::updateSensor);
    connect(m_device, &Device::locationUpdated, m_webhook, &WebhookApi::updateLocation);

    readSettings();

    m_device->setRegistered(m_webhook->isRegistered());
}

ClientInterface::~ClientInterface()
{
    writeSettings();
}

void ClientInterface::connectToHost()
{
    getConfig();
}

Device *ClientInterface::device()
{
    return m_device;
}

HomeassistantInfo *ClientInterface::homeassistantInfo()
{
    return m_homeassistantInfo;
}

void ClientInterface::initialize()
{
    m_webhook->initialize();
    m_webhook->updateRegistration(m_device);
}

void ClientInterface::reset()
{
    setHostname(QString());
    setPort(8123);
    setSsl(false);
    setToken(QString());

    m_webhook->reset();

    writeSettings();
}

void ClientInterface::saveSettings()
{
    writeSettings();
}

void ClientInterface::getConfig()
{
    m_homeassistantInfo->setLoading(true);
    m_api->getConfig();
}

void ClientInterface::registerDevice()
{
    setBusy(true);
    m_api->registerDevice(m_device);
}

bool ClientInterface::busy() const
{
    return m_busy;
}

QString ClientInterface::hostname() const
{
    return m_hostname;
}

quint16 ClientInterface::port() const
{
    return m_port;
}

bool ClientInterface::ssl() const
{
    return m_ssl;
}

QString ClientInterface::token() const
{
    return m_api->token();
}

void ClientInterface::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged(m_busy);
}

void ClientInterface::setHostname(const QString &hostname)
{
    if (m_hostname == hostname)
        return;

    // clean up hostname
    if (hostname.startsWith("http://")) {
        setSsl(false);
        m_hostname = hostname.mid(7, hostname.length());
    } else if (hostname.startsWith("https://")) {
        setSsl(true);
        m_hostname = hostname.mid(8, hostname.length());
    } else {
        m_hostname = hostname;
    }
    emit hostnameChanged(m_hostname);

    updateBaseUrl();
}

void ClientInterface::setPort(quint16 port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(m_port);

    updateBaseUrl();
}

void ClientInterface::setSsl(bool ssl)
{
    if (m_ssl == ssl)
        return;

    m_ssl = ssl;
    emit sslChanged(m_ssl);

    updateBaseUrl();
}

void ClientInterface::setToken(const QString &token)
{
    m_api->setToken(token);
}

void ClientInterface::onDataAvailable(const QString &endpoint, const QJsonDocument &doc)
{
#ifdef QT_DEBUG
    qDebug() << endpoint;
#endif

    if (endpoint == QStringLiteral(HASS_API_ENDPOINT_CONFIG)) {
        m_homeassistantInfo->setData(doc.object());
        m_homeassistantInfo->setAvailable(true);
        m_homeassistantInfo->setLoading(false);
    } else if (endpoint == QStringLiteral(HASS_API_ENDPOINT_DEVICE_REGISTRATION)) {
        m_webhook->setRegistrationData(doc.object());
        m_device->setRegistered(m_webhook->isRegistered());

        for (const DeviceSensor *sensor : m_device->sensors()) {
            m_webhook->registerSensor(sensor);
        }
        setBusy(false);
        writeSettings();
    }
}

void ClientInterface::updateBaseUrl()
{
    m_baseUrl = QString("%1://%2:%3").arg(m_ssl ? "https" : "http",
                                          m_hostname,
                                          QString::number(m_port));

    m_api->setBaseUrl(m_baseUrl);
    m_api->setSsl(m_ssl);

    m_webhook->setBaseUrl(m_baseUrl);
    m_webhook->setSsl(m_ssl);
}

void ClientInterface::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    m_hostname = settings.value(QStringLiteral("hostname"), QString()).toString();
    m_port = quint16(settings.value(QStringLiteral("port"), 8123).toInt());
    m_ssl = settings.value(QStringLiteral("ssl"), false).toBool();
    m_api->setToken(settings.value(QStringLiteral("token"), QString()).toString());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("WEBHOOK_API"));
    m_webhook->setCloudhookUrl(settings.value(QStringLiteral("cloudhook_url"), QString()).toString());
    m_webhook->setEncryption(settings.value(QStringLiteral("encryption"), false).toBool());
    m_webhook->setRemoteUiUrl(settings.value(QStringLiteral("remote_ui_url"), QString()).toString());
    m_webhook->setSecret(settings.value(QStringLiteral("secret"), QString()).toString());
    m_webhook->setWebhookId(settings.value(QStringLiteral("webhook_id"), QString()).toString());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVICE"));
    const QString name = settings.value(QStringLiteral("name"), QString()).toString();

    if (!name.isEmpty())
        m_device->setName(name);
    settings.endGroup();

    updateBaseUrl();
}

void ClientInterface::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    settings.setValue(QStringLiteral("hostname"), m_hostname);
    settings.setValue(QStringLiteral("port"), m_port);
    settings.setValue(QStringLiteral("ssl"), m_ssl);
    settings.setValue(QStringLiteral("token"), m_api->token());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("WEBHOOK_API"));
    settings.setValue(QStringLiteral("cloudhook_url"), m_webhook->cloudhookUrl());
    settings.setValue(QStringLiteral("encryption"), m_webhook->encryption());
    settings.setValue(QStringLiteral("remote_ui_url"), m_webhook->remoteUiUrl());
    settings.setValue(QStringLiteral("secret"), m_webhook->secret());
    settings.setValue(QStringLiteral("webhook_id"), m_webhook->webhookId());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVICE"));
    settings.setValue(QStringLiteral("name"), m_device->name());
    settings.endGroup();
}
