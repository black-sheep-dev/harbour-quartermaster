#include "apiconnector.h"

#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStandardPaths>

#include <zlib.h>

#include "src/constants.h"

static const QString HASS_API_ENDPOINT_WEBHOOK = QStringLiteral("/api/webhook/");

ApiConnector::ApiConnector(QObject *parent) :
    QObject(parent)
{
    m_apiEndpoints[RequestGetApi]                       = QStringLiteral("/api");
    m_apiEndpoints[RequestGetApiConfig]                 = QStringLiteral("/api/config");
    m_apiEndpoints[RequestGetApiDiscoveryInfo]          = QStringLiteral("/api/discovery_info");
    m_apiEndpoints[RequestGetApiEvents]                 = QStringLiteral("/api/events");
    m_apiEndpoints[RequestGetApiServices]               = QStringLiteral("/api/services");
    m_apiEndpoints[RequestGetApiHistoryPeriod]          = QStringLiteral("/api/history/period/");
    m_apiEndpoints[RequestGetApiLogBook]                = QStringLiteral("/api/logbook/");
    m_apiEndpoints[RequestGetApiStates]                 = QStringLiteral("/api/states");
    m_apiEndpoints[RequestGetApiStatesEntity]           = QStringLiteral("/api/states/");
    m_apiEndpoints[RequestGetApiErrorLog]               = QStringLiteral("/api/error_log");
    m_apiEndpoints[RequestGetApiCameraProxy]            = QStringLiteral("/api/camera_proxy/");
    m_apiEndpoints[RequestPostApiStates]                = QStringLiteral("/api/states/");
    m_apiEndpoints[RequestPostApiEvents]                = QStringLiteral("/api/events/");
    m_apiEndpoints[RequestPostApiServices]              = QStringLiteral("/api/services/");
    m_apiEndpoints[RequestPostApiConfigCheckConfig]     = QStringLiteral("/api/config/core/check_config");
    m_apiEndpoints[RequestPostApiRegisterDevice]        = QStringLiteral("/api/mobile_app/registrations");


    m_webhookTypes[RequestWebhookCallService]           = QStringLiteral("call_service");
    m_webhookTypes[RequestWebhookEnableEncryption]      = QStringLiteral("enable_encryption");
    m_webhookTypes[RequestWebhookFireEvent]             = QStringLiteral("fire_event");
    m_webhookTypes[RequestWebhookGetConfig]             = QStringLiteral("get_config");
    m_webhookTypes[RequestWebhookGetZones]              = QStringLiteral("get_zones");
    m_webhookTypes[RequestWebhookRegisterSensor]        = QStringLiteral("register_sensor");
    m_webhookTypes[RequestWebhookRenderTemplate]        = QStringLiteral("render_templates");
    m_webhookTypes[RequestWebhookStreamCamera]          = QStringLiteral("stream_camera");
    m_webhookTypes[RequestWebhookUpdateLocation]        = QStringLiteral("update_location");
    m_webhookTypes[RequestWebhookUpdateRegistration]    = QStringLiteral("update_registration");
    m_webhookTypes[RequestWebhookUpdateSensorStates]    = QStringLiteral("call_service");


    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &ApiConnector::onSslErrors);
    connect(this, &ApiConnector::credentialsChanged, [this]() { refreshWebhookUrl(); });
}

void ApiConnector::connectToHost()
{
    m_connectionFailures = ConnectionFailureNone;
    getConfig();
}

ServerConfig *ApiConnector::serverConfig()
{
    return m_serverConfig;
}

void ApiConnector::callService(const QString &domain, const QString &service, const QJsonObject &payload)
{
    sendRequest(RequestGetApiServices,
                domain + "/" + service,
                payload);
}

void ApiConnector::checkApi()
{
    sendRequest(RequestGetApi);
}

void ApiConnector::checkConfig()
{
    sendRequest(RequestPostApiConfigCheckConfig);
}

void ApiConnector::fireEvent(const QString &event, const QJsonObject &payload)
{
    sendRequest(RequestPostApiEvents, event, payload);
}

void ApiConnector::getCameraProxy(const QString &entityId)
{
    sendRequest(RequestGetApiCameraProxy, entityId);
}

void ApiConnector::getConfig()
{
    sendRequest(RequestGetApiConfig);
}

void ApiConnector::getDiscoveryInfo(const QString &hostname, quint16 port)
{
    m_serverConfig->setInternalUrl(hostname);
    m_serverConfig->setInternalPort(port);

    sendRequest(RequestGetApiDiscoveryInfo, QString(), QJsonObject(), false);
}

void ApiConnector::getEntityState(const QString &entityId)
{
    sendRequest(RequestGetApiStatesEntity, entityId);
}

void ApiConnector::getErrorLog()
{
    sendRequest(RequestGetApiErrorLog, m_apiEndpoints[RequestGetApiErrorLog]);
}

void ApiConnector::getEvents()
{
    sendRequest(RequestGetApiConfig, m_apiEndpoints[RequestGetApiConfig]);
}

void ApiConnector::getHistory(const QDateTime &timestamp, const QStringList &entityIds, const QDateTime &endTime, bool minimal, bool significantChangesOnly)
{
    QString query;

    if (!timestamp.isValid())
        query.append(QDateTime::currentDateTimeUtc().toString(Qt::ISODate));

    if (endTime.isValid())
        query.append(QStringLiteral("?end_time=") + endTime.toString(Qt::ISODate));

    if (!entityIds.isEmpty())
        query.append(QStringLiteral("?filter_entity_id=") + entityIds.join(','));

    if (minimal)
        query.append(QStringLiteral("?minimal_response"));

    if (significantChangesOnly)
        query.append(QStringLiteral("?significant_changes_only"));

    sendRequest(RequestGetApiHistoryPeriod, query);
}

void ApiConnector::getLogbook(const QDateTime &timestamp, const QString &entityId, const QDateTime &endTime)
{
    QString query;

    if (!timestamp.isValid())
        query.append(QDateTime::currentDateTimeUtc().toString(Qt::ISODate));

    if (endTime.isValid())
        query.append(QStringLiteral("?end_time=") + endTime.toString(Qt::ISODate));

    if (!entityId.isEmpty())
        query.append(QStringLiteral("?entity=") + entityId);

    sendRequest(RequestGetApiLogBook, query);
}

void ApiConnector::getServices()
{
    sendRequest(RequestGetApiServices);
}

void ApiConnector::getStates()
{
    sendRequest(RequestGetApiStates);
}

void ApiConnector::registerDevice(const QJsonObject &object)
{
    sendRequest(RequestPostApiRegisterDevice, QString(), object);
}

void ApiConnector::setEntityState(const QString &entityId, const QJsonObject &payload)
{
    sendRequest(RequestPostApiStates, entityId, payload);
}

bool ApiConnector::atHome() const
{
    return m_atHome;
}

quint8 ApiConnector::connectionModes() const
{
    return m_connectionModes;
}

Credentials ApiConnector::credentials() const
{
    return m_credentials;
}

bool ApiConnector::encryption() const
{
    return m_encryption;
}

bool ApiConnector::logging() const
{
    return m_logging;
}

void ApiConnector::sendRequest(quint8 type, const QString &query, const QJsonObject &payload, bool token)
{
#ifdef QT_DEBUG
    qDebug() << "REQUEST TO: " << type;
    qDebug() << "QUERY: " << query;
    qDebug() << "PAYLOAD: " << payload;
#endif

    if ( (m_credentials.token.isEmpty() || !token) && type != RequestGetApiDiscoveryInfo )
        return;

    // check if request is already enqueued
    QMutexLocker locker(m_mutex);
    if (m_activeRequests.contains(type))
        return;

    m_activeRequests.append(type);
    locker.unlock();

    // build QNetworkRequest
    QNetworkRequest request;

    if ( m_atHome
         || m_serverConfig->externalUrl().isEmpty()
         || m_connectionModes == ApiConnector::InternalConnection) {

        request.setUrl(m_serverConfig->internalUrl()
                       + QStringLiteral(":%1").arg(m_serverConfig->internalPort())
                       + m_apiEndpoints.value(type) + query);
    } else {
        request.setUrl(m_serverConfig->externalUrl()
                       + QStringLiteral(":%1").arg(m_serverConfig->externalPort())
                       + m_apiEndpoints.value(type) + query);
    }

    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept-Encoding", "gzip");

#ifdef QT_DEBUG
    qDebug() << "URL: " << request.url().toString();
#endif


    if (token)
        request.setRawHeader("Authorization", "Bearer " + m_credentials.token.toLatin1());


    // send
    QNetworkReply *reply{nullptr};

    switch (type) {
    case RequestGetApi:
    case RequestGetApiConfig:
    case RequestGetApiDiscoveryInfo:
    case RequestGetApiEvents:
    case RequestGetApiServices:
    case RequestGetApiHistoryPeriod:
    case RequestGetApiLogBook:
    case RequestGetApiStates:
    case RequestGetApiStatesEntity:
    case RequestGetApiErrorLog:
    case RequestGetApiCameraProxy:
        reply = m_manager->get(request);
        break;

    case RequestPostApiStates:
    case RequestPostApiEvents:
    case RequestPostApiServices:
    case RequestPostApiConfigCheckConfig:
    case RequestPostApiRegisterDevice:
        reply = m_manager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
        break;

    default:
        locker.relock();
        m_activeRequests.removeAll(type);
        locker.unlock();
        emit apiError(type, ErrorBadRequest, m_apiEndpoints.value(type) + query);
        return;
    }

    reply->setProperty(ApiKey::KEY_TYPE.toLatin1(), type);
    connect(reply, &QNetworkReply::finished, this, &ApiConnector::onFinished);
}

void ApiConnector::sendWebhookRequest(quint8 type, const QJsonObject &payload)
{
#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REQUEST TO: " << type;
    qDebug() << "PAYLOAD: " << payload;
#endif

    if (m_credentials.webhookId.isEmpty())
        return;

    // check if request is already enqueued
    QMutexLocker locker(m_mutex);
    if (m_activeRequests.contains(type))
        return;

    m_activeRequests.append(type);
    locker.unlock();

    // prepare message
    QJsonObject message;
    message.insert(ApiKey::KEY_TYPE, m_webhookTypes.value(type));

    // prepare data if available
    if (!payload.isEmpty()) {
        if (m_encryption) {
            message.insert(ApiKey::KEY_ENCRYPTED, true);
            message.insert(ApiKey::KEY_DATA, payload);
        } else {
            message.insert(ApiKey::KEY_DATA, payload);
        }
    }

    // build QNetworkRequest
    QNetworkRequest request;
    request.setUrl(m_webhookUrl);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept-Encoding", "gzip");

    auto reply = m_manager->post(request, QJsonDocument(message).toJson(QJsonDocument::Compact));
    reply->setProperty(ApiKey::KEY_TYPE.toLatin1(), type);
    connect(reply, &QNetworkReply::finished, this, &ApiConnector::onWebhookFinished);
}

void ApiConnector::setAtHome(bool atHome)
{
    if (m_atHome == atHome)
        return;

    m_atHome = atHome;
    emit atHomeChanged(m_atHome);
}

void ApiConnector::setConnectionModes(quint8 modes)
{
    if (m_connectionModes == modes)
        return;

    m_connectionModes = modes;
    emit connectionModesChanged(m_connectionModes);

    refreshWebhookUrl();
}

void ApiConnector::setCredentials(const Credentials &credentials)
{
#ifdef QT_DEBUG
        qDebug() << credentials.token;
#endif

    if (m_credentials == credentials)
        return;

    m_credentials = credentials;
    emit credentialsChanged(m_credentials);

    setEncryption(!m_credentials.secret.isEmpty());
}

void ApiConnector::setEncryption(bool encryption)
{
    if (m_encryption == encryption)
        return;

    m_encryption = encryption;
    emit encryptionChanged(m_encryption);
}

void ApiConnector::setLogging(bool logging)
{
    if (m_logging == logging)
        return;

    m_logging = logging;
    emit loggingChanged(m_logging);

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + APP_TARGET);

    if (!dir.exists())
        dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + APP_TARGET);
}

void ApiConnector::onFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    // check reply
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const quint8 type = reply->property(ApiKey::KEY_TYPE.toLatin1()).toUInt();
    const QString typeString = m_webhookTypes.value(type);
    const QString url = reply->url().toString();
    const QString errorString = reply->errorString();
    const QByteArray raw = reply->readAll();

    QMutexLocker locker(m_mutex);
    m_activeRequests.removeAll(type);
    locker.relock();


#ifdef QT_DEBUG
    qDebug() << "REQUEST FINSHED: " << type;
    qDebug() << "STATUS CODE: " << statusCode;
#endif

    // check for errors
    switch (statusCode) {
    case 200:
    case 201:
        break;

    case 400:
        emit apiError(type, ErrorDataInvalid, typeString);
        return;

    case 401:
        emit apiError(type, ErrorUnauthorized, typeString);
        return;

    case 404:
        emit apiError(type, ErrorNotFound, typeString);
        return;

    case 405:
        emit apiError(type, ErrorMethodNotAllowed, typeString);
        return;

    default:
        updateConnectionFailures(url);
        emit apiError(type, ErrorUnkown, errorString);
        return;
    }


    // begin parsing data
    QByteArray data = gunzip(raw);

    if (data.isEmpty())
        data = raw;

    QJsonParseError error{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << error.errorString();
#endif
        emit apiError(type, ErrorJsonInvalid, typeString);
        return;
    }

    // logging
    if (logging())
        logData(QStringLiteral("rest_api_reply"), doc.toJson(QJsonDocument::Indented));

    // request specific handling
    switch (type) {
    case RequestGetApiConfig:
        m_serverConfig->setData(doc.object());
        break;

    case RequestGetApiDiscoveryInfo:
        m_serverConfig->setData(doc.object());
        emit hostDiscovered(true);
        break;

    case RequestPostApiRegisterDevice:
        parseDeviceRegistration(doc.object());
        break;

    case RequestPostApiConfigCheckConfig:
        parseConfigCheck(doc.object());

    default:
        // emit data
        emit apiRequestFinished(type, doc);
        break;
    }

    emit requestFinished(type);
}

void ApiConnector::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(reply);
    Q_UNUSED(errors);
}

void ApiConnector::onWebhookFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    // check reply
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const quint8 type = reply->property(ApiKey::KEY_TYPE.toLatin1()).toUInt();
    const QString typeString = m_webhookTypes.value(type);
    const QString url = reply->url().toString();
    const QByteArray raw = reply->readAll();
    const QString errorString = reply->errorString();

    reply->deleteLater();

    QMutexLocker locker(m_mutex);
    m_activeRequests.removeAll(type);
    locker.relock();


#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REQUEST FINSHED: " << type;
    qDebug() << "STATUS CODE: " << statusCode;
#endif

    // check for errors
    switch (statusCode) {
    case 200:
    case 201:
        break;

    case 400:
        emit apiError(type, ErrorDataInvalid, typeString);
        return;

    case 404:
        emit apiError(type, ErrorMobileAppNotLoaded, typeString);
        return;

    case 410:
        emit apiError(type, ErrorIntegrationDeleted, typeString);
        emit requestRegistrationRefresh();
        return;

    default:
        updateConnectionFailures(url);
        emit apiError(type, ErrorUnkown, errorString);
        return;
    }

    // begin parsing data 
    QByteArray data = gunzip(raw);

    if (data.isEmpty())
        data = raw;

    QJsonParseError error{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << error.errorString();
#endif
        emit apiError(type, ErrorJsonInvalid, typeString);
        return;
    }

    // logging
    if (logging())
        logData(QStringLiteral("webhook_reply_") + typeString, doc.toJson(QJsonDocument::Indented));

    // emit data
    emit apiRequestFinished(type, doc);
    emit requestFinished(type);
}

QByteArray ApiConnector::gunzip(const QByteArray &data)
{
    if (data.size() <= 4) {
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

void ApiConnector::logData(const QString &identifier, const QByteArray &data)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
               + "/" + APP_TARGET
               + "/log_"
               + identifier
               + "_" + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMddhhmmsszzz"))
               + ".txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << QString(data);

    file.close();
}

void ApiConnector::parseDeviceRegistration(const QJsonObject &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#endif

    m_credentials.cloudhookUrl = data.value(ApiKey::KEY_CLOUDHOOK_URL).toString();
    m_credentials.remoteUiUrl = data.value(ApiKey::KEY_REMOTE_UI_URL).toString();
    m_credentials.secret = data.value(ApiKey::KEY_SECRET).toString();
    m_credentials.webhookId = data.value(ApiKey::KEY_WEBHOOK_ID).toString();

    emit credentialsChanged(m_credentials);
    emit deviceRegistered(!m_credentials.webhookId.isEmpty());
}

void ApiConnector::parseConfigCheck(const QJsonObject &data)
{
    m_serverConfig->setConfigValid(data.value(ApiKey::KEY_RESULT).toString() == ApiKey::KEY_VALID);
    m_serverConfig->setConfigError(data.value(ApiKey::KEY_ERRORS).toString());
}

void ApiConnector::refreshWebhookUrl()
{
    if (m_connectionModes == ApiConnector::InternalConnection) {
        m_webhookUrl = m_serverConfig->internalUrl()
                + QStringLiteral(":%1").arg(m_serverConfig->internalPort())
                + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
        return;
    }

    if ( !m_credentials.cloudhookUrl.isEmpty() && !(m_connectionFailures & ConnectionFailureCloudhook) ) {
        m_webhookUrl = m_credentials.cloudhookUrl;
    } else if ( !m_credentials.remoteUiUrl.isEmpty() && !(m_connectionFailures & ConnectionFailureRemoteUi) ) {
        m_webhookUrl = m_credentials.remoteUiUrl + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
    } else if ( !m_serverConfig->externalUrl().isEmpty() && !(m_connectionFailures & ConnectionFailureExternal) ) {

        if (m_atHome) {
            m_webhookUrl = m_serverConfig->internalUrl()
                    + QStringLiteral(":%1").arg(m_serverConfig->internalPort())
                    + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
        } else {
            m_webhookUrl = m_serverConfig->externalUrl()
                    + QStringLiteral(":%1").arg(m_serverConfig->externalPort())
                    + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
        }

    } else {
        m_webhookUrl = m_serverConfig->internalUrl()
                + QStringLiteral(":%1").arg(m_serverConfig->internalPort())
                + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
    }
}

void ApiConnector::updateConnectionFailures(const QString &url)
{
    if (url.startsWith(m_credentials.cloudhookUrl)) {
        m_connectionFailures |= ConnectionFailureCloudhook;
    } else if (url.startsWith(m_credentials.remoteUiUrl)) {
        m_connectionFailures |= ConnectionFailureRemoteUi;
    } else if (url.startsWith(m_serverConfig->externalUrl())) {
        m_connectionFailures |= ConnectionFailureExternal;
    } else if (url.startsWith(m_serverConfig->internalUrl())) {
        m_connectionFailures | ConnectionFailureInternal;
    }

    emit connectionFailure(m_connectionFailures);
}
