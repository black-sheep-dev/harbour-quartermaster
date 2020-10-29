#include "homeassistantapi.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>

#include "src/string_constants.h"

HomeassistantApi::HomeassistantApi(Wallet *wallet, QObject *parent) :
    ApiInterface(wallet, parent)
{

}

void HomeassistantApi::callService(const QString &domain, const QString &service, const QString &entityId, const QJsonObject &data)
{
    QJsonObject payload = data;
    payload.insert(API_KEY_ENTITY_ID, entityId);

    request(getRequest(HASS_API_ENDPOINT_SERVICES + "/" + domain + "/" + service), payload);
}

void HomeassistantApi::getConfig()
{
    request(getRequest(HASS_API_ENDPOINT_CONFIG));
}

void HomeassistantApi::getErrorLog()
{
    request(getRequest(HASS_API_ENDPOINT_ERROR_LOG));
}

void HomeassistantApi::getLogBook(const QDateTime &timestamp)
{
    request(getRequest(HASS_API_ENDPOINT_LOGBOOK + "/" + timestamp.toString(Qt::ISODate)));
}

void HomeassistantApi::getState(const QString &entityId)
{
    request(getRequest(HASS_API_ENDPOINT_STATES + "/" + entityId));
}

void HomeassistantApi::getStates()
{
    request(getRequest(HASS_API_ENDPOINT_STATES));
}

void HomeassistantApi::registerDevice(Device *device)
{
    if (!device)
        return;

    QJsonObject data;
    data.insert(API_KEY_DEVICE_ID, device->id());
    data.insert(API_KEY_APP_ID, QStringLiteral("org.nubecula.harbour.quartermaster"));
    data.insert(API_KEY_APP_NAME, QCoreApplication::applicationName());
    data.insert(API_KEY_APP_VERSION, QCoreApplication::applicationVersion());
    data.insert(API_KEY_DEVICE_NAME, device->name());
    data.insert(API_KEY_MANUFACTURER, device->manufacturer());
    data.insert(API_KEY_MODEL, device->model());
    data.insert(API_KEY_OS_NAME, device->softwareName());
    data.insert(API_KEY_OS_VERSION, device->softwareVersion());
    data.insert(API_KEY_SUPPORTS_ENCRYPTION, device->encryption());

    request(getRequest(HASS_API_ENDPOINT_DEVICE_REGISTRATION), data);
}

QNetworkRequest HomeassistantApi::getRequest(const QString &endpoint)
{
    QNetworkRequest request = ApiInterface::getRequest(endpoint);
    request.setRawHeader("Accept-Encoding", "gzip");
    request.setRawHeader("Authorization", "Bearer " + wallet()->token().toLatin1());

    m_activeRequests.append(endpoint);

    return request;
}

void HomeassistantApi::onRequestFinished(QNetworkReply *reply)
{
    if (!reply)
        return;

#ifdef QT_DEBUG
    qDebug() << "API REPLY";
#endif

    // read data
    const QString endpoint = reply->url().toString().remove(baseUrl());
    const QByteArray raw = reply->readAll();
    QByteArray data = gunzip(raw);

    if (data.isEmpty())
        data = raw;

    const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    m_activeRequests.removeAll(endpoint);

#ifdef QT_DEBUG
    qDebug() << endpoint;
//    qDebug() << data;
    qDebug() << data.size();
    qDebug() << status;
#endif

    // handel errors
    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->errorString();
#endif

        reply->deleteLater();
        return;
    }

    // delete reply
    reply->deleteLater();

    // non json data handling
    if (endpoint == HASS_API_ENDPOINT_ERROR_LOG) {
        emit errorLogAvailable(data);
        return;
    }

    // json data handling
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
        logData(QStringLiteral("rest_api_reply"), doc.toJson(QJsonDocument::Indented));

    emit dataAvailable(endpoint, doc);
}

