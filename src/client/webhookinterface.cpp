#include "webhookinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>

WebhookInterface::WebhookInterface(Device *device, QObject *parent) :
    QObject(parent),
    m_baseUrl(QString()),
    m_connectionFailures(ConnectionFailureNone),
    m_device(device),
    m_manager(new QNetworkAccessManager(this)),
    m_ssl(false),
    m_webhookUrl(QString())
{
    connect(m_manager, &QNetworkAccessManager::sslErrors, this ,&WebhookInterface::onSslErrors);
    connect(m_device, &Device::sensorUpdated, this, &WebhookInterface::updateSensor);

    readSettings();
}

WebhookInterface::~WebhookInterface()
{
    writeSettings();
}

bool WebhookInterface::isValid() const
{
    return !m_webhookId.isEmpty();
}

void WebhookInterface::resetSettings()
{
    m_cloudhookUrl = QString();
    m_remoteUiUrl = QString();
    m_secret = QString();
    m_webhookId = QString();

    writeSettings();
}

void WebhookInterface::saveSettings()
{
    writeSettings();
}

void WebhookInterface::setBaseUrl(const QString &url)
{
    m_baseUrl = url;
    updateWebhookUrl();
}

void WebhookInterface::setSsl(bool enable)
{
    m_ssl = enable;
}

QString WebhookInterface::cloudhookUrl() const
{
    return m_cloudhookUrl;
}

bool WebhookInterface::encryption() const
{
    return m_encryption;
}

QString WebhookInterface::remoteUiUrl() const
{
    return m_remoteUiUrl;
}

QString WebhookInterface::secret() const
{
    return m_secret;
}

QString WebhookInterface::webhookId() const
{
    return m_webhookId;
}

void WebhookInterface::registerSensors()
{
    for (const QJsonValue value : m_device->sensors()) {
        sendRequest(QStringLiteral("register_sensor"), value.toObject());
    }
}

void WebhookInterface::updateSensor(const QJsonObject &sensor)
{
    sendRequest(QStringLiteral("update_sensor_states"), sensor);
}

void WebhookInterface::setCloudhookUrl(const QString &cloudhookUrl)
{
    if (m_cloudhookUrl == cloudhookUrl)
        return;

    m_cloudhookUrl = cloudhookUrl;
    emit cloudhookUrlChanged(m_cloudhookUrl);
}

void WebhookInterface::setEncryption(bool enabled)
{
    if (m_encryption == enabled)
        return;

    m_encryption = enabled;
    emit encryptionChanged(m_encryption);
}

void WebhookInterface::setRemoteUiUrl(const QString &url)
{
    if (m_remoteUiUrl == url)
        return;

    m_remoteUiUrl = url;
    emit remoteUiUrlChanged(m_remoteUiUrl);
}

void WebhookInterface::setSecret(const QString &secret)
{
    if (m_secret == secret)
        return;

    m_secret = secret;
    emit secretChanged(m_secret);
}

void WebhookInterface::setWebhookId(const QString &id)
{
    if (m_webhookId == id)
        return;

    m_webhookId = id;
    emit webhookIdChanged(m_webhookId);

    updateWebhookUrl();
}

void WebhookInterface::onReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (!reply)
        return;

#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REPLY";
#endif

    // handel errors
    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->errorString();
#endif
        reply->deleteLater();
        return;
    }

    // read data
    const QString url = reply->url().toString();
    const QByteArray &data = reply->readAll();
    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QString type = reply->property("msg_type").toString();

#ifdef QT_DEBUG
    qDebug() << url;
    qDebug() << data;
    qDebug() << status;
    qDebug() << type;
#endif

    // delete reply
    reply->deleteLater();

    if (data.isEmpty()) {
        return;
    }

    // parse data
}

void WebhookInterface::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->deleteLater();

#ifdef QT_DEBUG
    for (const QSslError &error : errors) {
        qDebug() << error.errorString();
    }
#endif
}

void WebhookInterface::sendRequest(const QString &type, const QJsonObject &payload)
{
    QNetworkRequest request(m_webhookUrl);

    if (m_ssl) {
        QSslConfiguration sslConfig(QSslConfiguration::defaultConfiguration());
        request.setSslConfiguration(sslConfig);
    }

    request.setRawHeader("Content-Type", "application/json");

    QJsonObject message;
    message.insert(QStringLiteral("type"), type);

    if (m_encryption) {
        message.insert(QStringLiteral("encrypted"), true);
        message.insert(QStringLiteral("data"), payload);
    } else {
        message.insert(QStringLiteral("data"), payload);
    }

#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REQUEST";
    qDebug() << message;
#endif

    QNetworkReply *reply = m_manager->post(request, QJsonDocument(message).toJson(QJsonDocument::Compact));
    reply->setProperty("msg_type", type);
    connect(reply, &QNetworkReply::finished, this, &WebhookInterface::onReply);
}

void WebhookInterface::updateWebhookUrl()
{
    if ( !m_cloudhookUrl.isEmpty() && !(m_connectionFailures & ConnectionFailureCloudhook) ) {
        m_webhookUrl = m_cloudhookUrl;
    } else if ( !m_remoteUiUrl.isEmpty() && !(m_connectionFailures & ConnectionFailureRemoteUi) ) {
        m_webhookUrl = m_remoteUiUrl + QStringLiteral("/api/webhook/") + m_webhookId;
    } else {
        m_webhookUrl = m_baseUrl + QStringLiteral("/api/webhook/") + m_webhookId;
    }
}

void WebhookInterface::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("WEBHOOK"));
    m_cloudhookUrl = settings.value(QStringLiteral("cloudhook_url"), QString()).toString();
    m_encryption = settings.value(QStringLiteral("encryption"), false).toBool();
    m_remoteUiUrl = settings.value(QStringLiteral("remote_ui_url"), QString()).toString();
    m_secret = settings.value(QStringLiteral("secret"), QString()).toString();
    m_webhookId = settings.value(QStringLiteral("webhook_id"), QString()).toString();
    settings.endGroup();
}

void WebhookInterface::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("WEBHOOK"));
    settings.setValue(QStringLiteral("cloudhook_url"), m_cloudhookUrl);
    settings.setValue(QStringLiteral("encryption"), m_encryption);
    settings.setValue(QStringLiteral("remote_ui_url"), m_remoteUiUrl);
    settings.setValue(QStringLiteral("secret"), m_secret);
    settings.setValue(QStringLiteral("webhook_id"), m_webhookId);
    settings.endGroup();
}
