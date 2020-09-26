#include "clientinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>

ClientInterface::ClientInterface(QObject *parent) :
    QObject(parent),
    m_device(new Device(this)),
    m_homeassistantInfo(new HomeassistantInfo(this)),
    m_manager(new QNetworkAccessManager(this)),
    m_webhook(new WebhookInterface(m_device, this)),
    m_busy(false),
    m_registered(false)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &ClientInterface::onApiReply);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this ,&ClientInterface::onSslErrors);

    readSettings();
    m_registered = m_webhook->isValid();
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

}

void ClientInterface::reset()
{
    setHostname(QString());
    setPort(8123);
    setSsl(false);
    setToken(QString());

    m_webhook->resetSettings();
}

void ClientInterface::saveSettings()
{
    writeSettings();
}

void ClientInterface::getConfig()
{
    m_homeassistantInfo->setLoading(true);
    apiGet(QStringLiteral(HASS_API_ENDPOINT_CONFIG));
}

void ClientInterface::registerDevice()
{
    if (m_registered)
        return;

    QJsonObject device;
    device.insert(QStringLiteral("device_id"), m_device->id());
    device.insert(QStringLiteral("app_id"), "org.nubecula.harbour.quartermaster");
    device.insert(QStringLiteral("app_name"), QCoreApplication::applicationName());
    device.insert(QStringLiteral("app_version"), QCoreApplication::applicationVersion());
    device.insert(QStringLiteral("device_name"), m_device->name());
    device.insert(QStringLiteral("manufacturer"), m_device->manufacturer());
    device.insert(QStringLiteral("model"), m_device->model());
    device.insert(QStringLiteral("os_name"), m_device->softwareName());
    device.insert(QStringLiteral("os_version"), m_device->softwareVersion());
    device.insert(QStringLiteral("supports_encryption"), m_webhook->encryption());

    apiPost(QStringLiteral(HASS_API_ENDPOINT_DEVICE_REGISTRATION), device);
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

bool ClientInterface::registered() const
{
    return m_registered;
}

bool ClientInterface::ssl() const
{
    return m_ssl;
}

QString ClientInterface::token() const
{
    return m_token;
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

void ClientInterface::setRegistered(bool registered)
{
    if (m_registered == registered)
        return;

    m_registered = registered;
    emit registeredChanged(m_registered);
}

void ClientInterface::setToken(const QString &token)
{
    if (m_token == token)
        return;

    m_token = token;
    emit tokenChanged(m_token);
}

void ClientInterface::onApiReply(QNetworkReply *reply)
{
    if (!reply)
        return;

#ifdef QT_DEBUG
    qDebug() << "API REPLY";
#endif

    // read data
    const QString url = reply->url().toString();
    const QByteArray &data = reply->readAll();
    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

#ifdef QT_DEBUG
    qDebug() << url;
    qDebug() << data;
    qDebug() << status;
#endif

    // handel errors
    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->errorString();
#endif
        if (url.endsWith(QStringLiteral("/api/config"))) {
            m_homeassistantInfo->setError(reply->errorString());
            m_homeassistantInfo->setAvailable(false);
            m_homeassistantInfo->setLoading(false);
        }

        reply->deleteLater();
        return;
    }

    // delete reply
    reply->deleteLater();

    // parse data
    if (data.isEmpty()) {
        setBusy(false);
        return;
    }

    QJsonParseError error;

    const QJsonObject obj = QJsonDocument::fromJson(data, &error).object();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
#endif
        setBusy(false);
        return;
    }

    if ( status != 200 && status != 201 ) {
        setBusy(false);
        return;
    }

    // transfer data to endpoints
    if ( url.endsWith(QStringLiteral(HASS_API_ENDPOINT_CONFIG)) ) {
        m_homeassistantInfo->setData(obj);
        m_homeassistantInfo->setAvailable(true);
        m_homeassistantInfo->setLoading(false);

    } else if ( url.endsWith(QStringLiteral(HASS_API_ENDPOINT_DEVICE_REGISTRATION)) ) {
        m_webhook->setCloudhookUrl(obj.value(QStringLiteral("cloudhook_url")).toString());
        m_webhook->setRemoteUiUrl(obj.value(QStringLiteral("remote_ui_url")).toString());
        m_webhook->setSecret(obj.value(QStringLiteral("secret")).toString());
        m_webhook->setWebhookId(obj.value(QStringLiteral("webhook_id")).toString());

        if (!m_webhook->webhookId().isEmpty()) {
            m_webhook->saveSettings();
            setRegistered(true);
            m_webhook->registerSensors();
        }
    }

    setBusy(false);
}

void ClientInterface::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->deleteLater();
    setBusy(false);

#ifdef QT_DEBUG
    for (const QSslError &error : errors) {
        qDebug() << error.errorString();
    }
#endif
}

void ClientInterface::onWebhookReply()
{

}

QNetworkRequest ClientInterface::getApiRequest(const QString &endpoint, bool token)
{
    QNetworkRequest request(QUrl(m_baseUrl + endpoint));

    if (m_ssl) {
        QSslConfiguration sslConfig(QSslConfiguration::defaultConfiguration());
        request.setSslConfiguration(sslConfig);
    }

    request.setRawHeader("Content-Type", "application/json");

    if (token)
        request.setRawHeader("Authorization", "Bearer " + m_token.toLatin1());

    return request;
}

void ClientInterface::updateBaseUrl()
{
    m_baseUrl = QString("%1://%2:%3").arg(m_ssl ? "https" : "http",
                                          m_hostname,
                                          QString::number(m_port));

    m_webhook->setBaseUrl(m_baseUrl);
}

void ClientInterface::apiGet(const QString &endpoint)
{
    m_manager->get(getApiRequest(endpoint));
}

void ClientInterface::apiPost(const QString &endpoint, const QJsonObject &payload)
{
    m_manager->post(getApiRequest(endpoint),
                    QJsonDocument(payload).toJson(QJsonDocument::Compact));
}

void ClientInterface::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    m_hostname = settings.value(QStringLiteral("hostname"), QString()).toString();
    m_port = quint16(settings.value(QStringLiteral("port"), 8123).toInt());
    m_ssl = settings.value(QStringLiteral("ssl"), false).toBool();
    m_token = settings.value(QStringLiteral("token"), QString()).toString();
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
    settings.setValue(QStringLiteral("token"), m_token);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVICE"));
    settings.setValue(QStringLiteral("name"), m_device->name());
    settings.endGroup();
}
