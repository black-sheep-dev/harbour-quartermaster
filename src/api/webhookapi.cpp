#include "webhookapi.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>

WebhookApi::WebhookApi(Wallet *wallet, QObject *parent) :
    ApiInterface(wallet, parent),
    m_connectionFailures(ConnectionFailureNone)
{
    connect(this, &ApiInterface::baseUrlChanged, this, &WebhookApi::updateWebhookUrl);
    connect(this, &ApiInterface::requestFinished, this, &WebhookApi::onReplyFinished);
    connect(this->wallet(), &Wallet::initialized, this, &WebhookApi::updateWebhookUrl);
}

bool WebhookApi::isRegistered()
{
    return !wallet()->webhookId().isEmpty();
}

void WebhookApi::reset()
{
    m_connectionFailures = ConnectionFailureNone;

    m_encryption = false;

    wallet()->setCloudhookUrl(QString());
    wallet()->setRemoteUiUrl(QString());
    wallet()->setSecret(QString());
    wallet()->setWebhookId(QString());

    wallet()->storeSecrets();
}

void WebhookApi::setRegistrationData(const QJsonObject &obj)
{
#ifdef QT_DEBUG
    qDebug() << obj;
#endif

    wallet()->setCloudhookUrl(obj.value(QStringLiteral("cloudhook_url")).toString());
    wallet()->setRemoteUiUrl(obj.value(QStringLiteral("remote_ui_url")).toString());
    wallet()->setSecret(obj.value(QStringLiteral("secret")).toString());
    wallet()->setWebhookId(obj.value(QStringLiteral("webhook_id")).toString());

    wallet()->storeSecrets();

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

bool WebhookApi::encryption() const
{
    return m_encryption;
}

void WebhookApi::setEncryption(bool enable)
{
    if (m_encryption == enable)
        return;

    m_encryption = enable;
    emit encryptionChanged(m_encryption);
}

void WebhookApi::onReplyFinished(const QString &identifier, QNetworkReply *reply)
{
    if (!reply || identifier.isEmpty())
        return;

    m_activeRequests.removeAll(identifier);

#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REPLY";
#endif

    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->errorString();
#endif
        reply->deleteLater();
        return;
    }

    // read data
    const QByteArray &data = reply->readAll();
    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

#ifdef QT_DEBUG
    qDebug() << identifier;
    qDebug() << data;
    qDebug() << status;
#endif

    // delete reply
    reply->deleteLater();

    if (data.isEmpty()) {
        return;
    }

    // parse data
    QJsonParseError error{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << error.errorString();
#endif
        return;
    }

    // logging
    if (logging())
        logData(QStringLiteral("webhook_reply"), doc.toJson(QJsonDocument::Indented));

    // handle data
    emit dataAvailable(identifier, doc);
}

void WebhookApi::updateWebhookUrl()
{
    if ( !wallet()->cloudhookUrl().isEmpty() && !(m_connectionFailures & ConnectionFailureCloudhook) ) {
        m_webhookUrl = wallet()->cloudhookUrl();
    } else if ( !wallet()->remoteUiUrl().isEmpty() && !(m_connectionFailures & ConnectionFailureRemoteUi) ) {
        m_webhookUrl = wallet()->remoteUiUrl() + QStringLiteral("/api/webhook/") + wallet()->webhookId();
    } else {
        m_webhookUrl = baseUrl() + QStringLiteral("/api/webhook/") + wallet()->webhookId();
    }
}

void WebhookApi::sendRequest(const QString &type, const QJsonObject &payload)
{
    if (m_activeRequests.contains(type))
        return;

    m_activeRequests.append(type);

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
