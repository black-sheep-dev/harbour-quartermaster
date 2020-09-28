#include "webhookapi.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>

WebhookApi::WebhookApi(QObject *parent) :
    ApiInterface(parent),
    m_connectionFailures(ConnectionFailureNone),
    m_cloudhookUrl(QString()),
    m_encryption(false),
    m_remoteUiUrl(QString()),
    m_secret(QString()),
    m_webhookId(QString())
{
    connect(this, &ApiInterface::baseUrlChanged, this, &WebhookApi::updateWebhookUrl);
    connect(this, &ApiInterface::requestFinished, this, &WebhookApi::onReplyFinished);
}

bool WebhookApi::isRegistered() const
{
    return !m_webhookId.isEmpty();
}

void WebhookApi::reset()
{
    m_connectionFailures = ConnectionFailureNone;
    m_cloudhookUrl = QString();
    m_encryption = false;
    m_remoteUiUrl = QString();
    m_secret = QString();
    m_webhookId = QString();
    m_webhookUrl = QString();
}

void WebhookApi::setRegistrationData(const QJsonObject &obj)
{
#ifdef QT_DEBUG
    qDebug() << obj;
#endif

    setCloudhookUrl(obj.value(QStringLiteral("cloudhook_url")).toString());
    setRemoteUiUrl(obj.value(QStringLiteral("remote_ui_url")).toString());
    setSecret(obj.value(QStringLiteral("secret")).toString());
    setWebhookId(obj.value(QStringLiteral("webhook_id")).toString());

    updateWebhookUrl();
}

void WebhookApi::getZones()
{
    sendRequest(QStringLiteral("get_zones"));
}

void WebhookApi::registerSensor(const DeviceSensor *sensor)
{
    sendRequest(QStringLiteral("register_sensor"), sensor->toJson());
}

void WebhookApi::updateLocation(const QJsonObject &location)
{
    sendRequest(QStringLiteral("update_location"), location);
}

void WebhookApi::updateRegistration(const Device *device)
{
    if (!device->registered())
        return;

    QJsonObject data;

    data.insert(QStringLiteral("app_version"), QCoreApplication::applicationVersion());
    data.insert(QStringLiteral("device_name"), device->name());
    data.insert(QStringLiteral("os_version"), device->softwareVersion());

    sendRequest(QStringLiteral("update_registration"), data);
}

void WebhookApi::updateSensor(const QJsonObject &sensor)
{
    qDebug() << "UPDATE SENSOR";
    qDebug() << sensor;

    sendRequest(QStringLiteral("update_sensor_states"), sensor);
}

QString WebhookApi::cloudhookUrl() const
{
    return m_cloudhookUrl;
}

bool WebhookApi::encryption() const
{
    return m_encryption;
}

QString WebhookApi::remoteUiUrl() const
{
    return m_remoteUiUrl;
}

QString WebhookApi::secret() const
{
    return m_secret;
}

QString WebhookApi::webhookId() const
{
    return m_webhookId;
}

void WebhookApi::setCloudhookUrl(const QString &url)
{
    if (m_cloudhookUrl == url)
        return;

    m_cloudhookUrl = url;
    emit cloudhookUrlChanged(m_cloudhookUrl);
}

void WebhookApi::setEncryption(bool encryption)
{
    if (m_encryption == encryption)
        return;

    m_encryption = encryption;
    emit encryptionChanged(m_encryption);
}

void WebhookApi::setRemoteUiUrl(const QString &url)
{
    if (m_remoteUiUrl == url)
        return;

    m_remoteUiUrl = url;
    emit remoteUiUrlChanged(m_remoteUiUrl);
}

void WebhookApi::setSecret(const QString &secret)
{
    if (m_secret == secret)
        return;

    m_secret = secret;
    emit secretChanged(m_secret);
}

void WebhookApi::setWebhookId(const QString &id)
{
    if (m_webhookId == id)
        return;

    m_webhookId = id;
    emit webhookIdChanged(m_webhookId);
}

void WebhookApi::onReplyFinished(const QString &identifier, QNetworkReply *reply)
{
    if (!reply || identifier.isEmpty())
        return;

#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REPLY";
#endif

    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->errorString();
#endif
        reply->deleteLater();
        return;
    }

    // read data
    const QByteArray &data = reply->readAll();
    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

#ifdef QT_DEBUG
    qDebug() << data;
    qDebug() << status;
#endif

    // delete reply
    reply->deleteLater();

    if (data.isEmpty()) {
        return;
    }

    // parse data
    QJsonParseError error;

    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << error.errorString();
#endif
        return;
    }

    // handle data
}

void WebhookApi::updateWebhookUrl()
{
    if ( !m_cloudhookUrl.isEmpty() && !(m_connectionFailures & ConnectionFailureCloudhook) ) {
        m_webhookUrl = m_cloudhookUrl;
    } else if ( !m_remoteUiUrl.isEmpty() && !(m_connectionFailures & ConnectionFailureRemoteUi) ) {
        m_webhookUrl = m_remoteUiUrl + QStringLiteral("/api/webhook/") + m_webhookId;
    } else {
        m_webhookUrl = baseUrl() + QStringLiteral("/api/webhook/") + m_webhookId;
    }
}

void WebhookApi::sendRequest(const QString &type, const QJsonObject &payload)
{
    QJsonObject message;
    message.insert(QStringLiteral("type"), type);

    // prepare data if available
    if (!payload.isEmpty()) {
        if (m_encryption) {
            message.insert(QStringLiteral("encrypted"), true);
            message.insert(QStringLiteral("data"), payload);
        } else {
            message.insert(QStringLiteral("data"), payload);
        }
    }

#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REQUEST";
    qDebug() << message;
#endif

    QNetworkRequest request = getRequest(m_webhookUrl);
    request.setUrl(m_webhookUrl);

    this->request(request, message, type);
}

void WebhookApi::initialize()
{
    updateWebhookUrl();
}
