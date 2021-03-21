#include "apiconnector.h"

#include <QDir>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStandardPaths>

#include <zlib.h>

#include "src/constants.h"

static const QString HASS_API_ENDPOINT_WEBHOOK = QStringLiteral("/api/webhook/");
static const QString HASS_API_ENDPOINT_WEBSOCKET = QStringLiteral("/api/websocket/");

ApiConnector::ApiConnector(QObject *parent) :
    QObject(parent)
{
    m_apiEndpoints[Api::RequestGetApi]                       = QStringLiteral("/api");
    m_apiEndpoints[Api::RequestGetApiConfig]                 = QStringLiteral("/api/config");
    m_apiEndpoints[Api::RequestGetApiDiscoveryInfo]          = QStringLiteral("/api/discovery_info");
    m_apiEndpoints[Api::RequestGetApiEvents]                 = QStringLiteral("/api/events");
    m_apiEndpoints[Api::RequestGetApiServices]               = QStringLiteral("/api/services");
    m_apiEndpoints[Api::RequestGetApiHistoryPeriod]          = QStringLiteral("/api/history/period/");
    m_apiEndpoints[Api::RequestGetApiLogBook]                = QStringLiteral("/api/logbook/");
    m_apiEndpoints[Api::RequestGetApiStates]                 = QStringLiteral("/api/states");
    m_apiEndpoints[Api::RequestGetApiStatesEntity]           = QStringLiteral("/api/states/");
    m_apiEndpoints[Api::RequestGetApiErrorLog]               = QStringLiteral("/api/error_log");
    m_apiEndpoints[Api::RequestGetApiCameraProxy]            = QStringLiteral("/api/camera_proxy/");
    m_apiEndpoints[Api::RequestPostApiStates]                = QStringLiteral("/api/states/");
    m_apiEndpoints[Api::RequestPostApiEvents]                = QStringLiteral("/api/events/");
    m_apiEndpoints[Api::RequestPostApiServices]              = QStringLiteral("/api/services/");
    m_apiEndpoints[Api::RequestPostApiConfigCheckConfig]     = QStringLiteral("/api/config/core/check_config");
    m_apiEndpoints[Api::RequestPostApiRegisterDevice]        = QStringLiteral("/api/mobile_app/registrations");


    m_webhookTypes[Api::RequestWebhookCallService]           = QStringLiteral("call_service");
    m_webhookTypes[Api::RequestWebhookEnableEncryption]      = QStringLiteral("enable_encryption");
    m_webhookTypes[Api::RequestWebhookFireEvent]             = QStringLiteral("fire_event");
    m_webhookTypes[Api::RequestWebhookGetConfig]             = QStringLiteral("get_config");
    m_webhookTypes[Api::RequestWebhookGetZones]              = QStringLiteral("get_zones");
    m_webhookTypes[Api::RequestWebhookRegisterSensor]        = QStringLiteral("register_sensor");
    m_webhookTypes[Api::RequestWebhookRenderTemplate]        = QStringLiteral("render_templates");
    m_webhookTypes[Api::RequestWebhookStreamCamera]          = QStringLiteral("stream_camera");
    m_webhookTypes[Api::RequestWebhookUpdateLocation]        = QStringLiteral("update_location");
    m_webhookTypes[Api::RequestWebhookUpdateRegistration]    = QStringLiteral("update_registration");
    m_webhookTypes[Api::RequestWebhookUpdateSensorStates]    = QStringLiteral("update_sensor_states");


    connect(m_manager, &QNetworkAccessManager::sslErrors, this, &ApiConnector::onSslErrors);
    connect(this, &ApiConnector::credentialsChanged, [this]() {
        refreshBaseUrl();
        refreshWebhookUrl();
    });
}

QString ApiConnector::baseUrl() const
{
    return m_baseUrl;
}

void ApiConnector::connectToHost()
{
    m_connectionFailures = ConnectionFailureNone;
    getConfig();
}

void ApiConnector::initialize()
{
    //openWebsocket();
    refreshBaseUrl();
    refreshWebhookUrl();

    emit initialized();
}

ServerConfig *ApiConnector::serverConfig()
{
    return m_serverConfig;
}

void ApiConnector::callService(const QString &domain, const QString &service, const QString &entityId)
{
    QJsonObject payload;
    payload.insert(ApiKey::KEY_ENTITY_ID, entityId);

    callService(domain, service, payload);
}

void ApiConnector::callService(const QString &domain, const QString &service, const QJsonObject &payload)
{
    sendRequest(Api::RequestPostApiServices,
                domain + "/" + service,
                payload);
}

void ApiConnector::checkApi()
{
    sendRequest(Api::RequestGetApi);
}

void ApiConnector::checkConfig()
{
    sendRequest(Api::RequestPostApiConfigCheckConfig);
}

void ApiConnector::fireEvent(const QString &event, const QJsonObject &payload)
{
    sendRequest(Api::RequestPostApiEvents, event, payload);
}

void ApiConnector::getCameraProxy(const QString &entityId)
{
    sendRequest(Api::RequestGetApiCameraProxy, entityId);
}

void ApiConnector::getConfig()
{
    sendRequest(Api::RequestGetApiConfig);
}

void ApiConnector::getDiscoveryInfo(const QString &hostname, quint16 port)
{
    m_serverConfig->setInternalUrl(hostname);
    m_serverConfig->setInternalPort(port);
    refreshBaseUrl();

    sendRequest(Api::RequestGetApiDiscoveryInfo, QString(), QJsonObject());
}

void ApiConnector::getEntityState(const QString &entityId)
{
    sendRequest(Api::RequestGetApiStatesEntity, entityId);
}

void ApiConnector::getErrorLog()
{
    sendRequest(Api::RequestGetApiErrorLog, m_apiEndpoints[Api::RequestGetApiErrorLog]);
}

void ApiConnector::getEvents()
{
    sendRequest(Api::RequestGetApiConfig, m_apiEndpoints[Api::RequestGetApiConfig]);
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

    sendRequest(Api::RequestGetApiHistoryPeriod, query);
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

    sendRequest(Api::RequestGetApiLogBook, query);
}

void ApiConnector::getServices()
{
    sendRequest(Api::RequestGetApiServices);
}

void ApiConnector::getStates()
{
    sendRequest(Api::RequestGetApiStates);
}

void ApiConnector::setEntityState(const QString &entityId, const QJsonObject &payload)
{
    sendRequest(Api::RequestPostApiStates, entityId, payload);
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

void ApiConnector::sendRequest(quint8 type, const QString &query, const QJsonObject &payload)
{
#ifdef QT_DEBUG
    qDebug() << "REQUEST TO: " << type;
    qDebug() << "QUERY: " << query;
    qDebug() << "PAYLOAD: " << payload;
    qDebug() << "AT HOME: " << m_atHome;
#endif

    if ( m_credentials.token.isEmpty() && type != Api::RequestGetApiDiscoveryInfo )
        return;

    // check if request is already running
    QMutexLocker locker(m_mutex);
    if (m_runningRequest.contains(type))
        return;

    m_runningRequest.append(type);

    // build QNetworkRequest
    QNetworkRequest request;

    request.setUrl(m_baseUrl + m_apiEndpoints.value(type) + query);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Accept-Encoding", "gzip");

#ifdef QT_DEBUG
    qDebug() << "URL: " << request.url().toString();
#endif


    if (type != Api::RequestGetApiDiscoveryInfo)
        request.setRawHeader("Authorization", "Bearer " + m_credentials.token.toLatin1());


    // send
    QNetworkReply *reply{nullptr};

    switch (type) {
    case Api::RequestGetApi:
    case Api::RequestGetApiConfig:
    case Api::RequestGetApiDiscoveryInfo:
    case Api::RequestGetApiEvents:
    case Api::RequestGetApiServices:
    case Api::RequestGetApiHistoryPeriod:
    case Api::RequestGetApiLogBook:
    case Api::RequestGetApiStates:
    case Api::RequestGetApiStatesEntity:
    case Api::RequestGetApiErrorLog:
    case Api::RequestGetApiCameraProxy:
        reply = m_manager->get(request);
        break;

    case Api::RequestPostApiStates:
    case Api::RequestPostApiEvents:
    case Api::RequestPostApiServices:
    case Api::RequestPostApiConfigCheckConfig:
    case Api::RequestPostApiRegisterDevice:
        reply = m_manager->post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
        break;

    default:
        m_runningRequest.removeAll(type);
        emit error(type, Api::ErrorBadRequest, m_apiEndpoints.value(type) + query);
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

    // check if request is already running
    QMutexLocker locker(m_mutex);
    if (m_runningRequest.contains(type))
        return;

    m_runningRequest.append(type);

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

    // refresh urls
    refreshBaseUrl();
    refreshWebhookUrl();
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
    refreshWebhookUrl();
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
    m_runningRequest.removeAll(type);
    locker.unlock();

#ifdef QT_DEBUG
    qDebug() << "URL: " << url;
    qDebug() << "REQUEST FINSHED: " << type;
    qDebug() << "STATUS CODE: " << statusCode;
#endif

    // check for errors
    switch (statusCode) {
    case 200:
    case 201:
        break;

    case 400:
        emit error(type, Api::ErrorDataInvalid, typeString);
        return;

    case 401:
        emit error(type, Api::ErrorUnauthorized, typeString);
        return;

    case 404:
        emit error(type, Api::ErrorNotFound, typeString);
        return;

    case 405:
        emit error(type, Api::ErrorMethodNotAllowed, typeString);
        return;

    default:
        updateConnectionFailures(url);
        emit requestFinished(type, false);
        emit error(type, Api::ErrorUnkown, errorString);
        return;
    }


    // begin parsing data
    QByteArray data = gunzip(raw);

    if (data.isEmpty())
        data = raw;

    QJsonParseError err{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << err.errorString();
#endif
        emit requestFinished(type, false);
        emit error(type, Api::ErrorJsonInvalid, typeString);
        return;
    }

//#ifdef QT_DEBUG
//      qDebug() << doc;
//#endif

    // logging
    if (logging())
        logData(QStringLiteral("rest_api_reply"), doc.toJson(QJsonDocument::Indented));

    // request specific handling
    switch (type) {
    case Api::RequestGetApiConfig:
        m_serverConfig->setData(doc.object());
        break;

    case Api::RequestGetApiDiscoveryInfo:
        m_serverConfig->setData(doc.object());
        break;

//    case Api::RequestPostApiRegisterDevice:
//        parseDeviceRegistration(doc.object());
//        break;

    case Api::RequestPostApiConfigCheckConfig:
        parseConfigCheck(doc.object());
        break;

    default:
        // emit data
        qDebug() << "EMIT DATA";
        emit requestDataFinished(type, doc);
        break;
    }

    emit requestFinished(type, true);
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
    const quint64 type = reply->property(ApiKey::KEY_TYPE.toLatin1()).toULongLong();
    const QString typeString = m_webhookTypes.value(type);
    const QString url = reply->url().toString();
    const QByteArray raw = reply->readAll();
    const QString errorString = reply->errorString();

    reply->deleteLater();

    QMutexLocker locker(m_mutex);
    m_runningRequest.removeAll(type);
    locker.unlock();

#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REQUEST FINSHED: " << type;
    qDebug() << "TYPE: " << typeString;
    qDebug() << "STATUS CODE: " << statusCode;
#endif

    // check for errors
    switch (statusCode) {
    case 200:
    case 201:
        break;

    case 400:
        emit error(type, Api::ErrorDataInvalid, typeString);
        return;

    case 404:
        emit error(type, Api::ErrorMobileAppNotLoaded, typeString);
        return;

    case 410:
        emit error(type, Api::ErrorIntegrationDeleted, typeString);
        emit requestRegistrationRefresh();
        return;

    default:
        updateConnectionFailures(url);
        emit requestFinished(type, false);
        emit error(type, Api::ErrorUnkown, errorString);
        return;
    }

    // begin parsing data 
    QByteArray data = gunzip(raw);

    if (data.isEmpty())
        data = raw;

    QJsonParseError err{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << err.errorString();
#endif
        emit requestFinished(type, false);
        emit error(type, Api::ErrorJsonInvalid, typeString);
        return;
    }

    // logging
    if (logging())
        logData(QStringLiteral("webhook_reply_") + typeString, doc.toJson(QJsonDocument::Indented));

    // emit data
    emit requestFinished(type, true);
    emit requestDataFinished(type, doc);
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
}

void ApiConnector::parseConfigCheck(const QJsonObject &data)
{
    m_serverConfig->setConfigValid(data.value(ApiKey::KEY_RESULT).toString() == ApiKey::KEY_VALID);
    m_serverConfig->setConfigError(data.value(ApiKey::KEY_ERRORS).toString());
}

void ApiConnector::refreshBaseUrl()
{
    if ( m_atHome
         || m_serverConfig->externalUrl().isEmpty()
         || m_connectionModes == ApiConnector::InternalConnection) {

        m_baseUrl = m_serverConfig->internalUrl()
                + QStringLiteral(":%1").arg(m_serverConfig->internalPort());
    } else {
        m_baseUrl = m_serverConfig->externalUrl()
                + QStringLiteral(":%1").arg(m_serverConfig->externalPort());
    }
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

// -----------------------------------------------------------------------------------------------------------------------------------------------
// WEBSOCKET
// -----------------------------------------------------------------------------------------------------------------------------------------------

quint8 ApiConnector::subscriptions() const
{
    return m_subscriptions;
}

bool ApiConnector::websocketAtHomeOnly() const
{
    return m_websocketAtHomeOnly;
}

void ApiConnector::setSubscriptions(quint8 subscriptions)
{
    if (m_subscriptions == subscriptions)
        return;

    m_subscriptions = subscriptions;
    emit subscriptionsChanged(m_subscriptions);
}

void ApiConnector::setWebsocketAtHomeOnly(bool only)
{
    if (m_websocketAtHomeOnly == only)
        return;

    m_websocketAtHomeOnly = only;
    emit websocketAtHomeOnlyChanged(m_websocketAtHomeOnly);
}

void ApiConnector::closeWebsocket()
{

}

void ApiConnector::openWebsocket()
{
    if ( (!m_atHome && m_websocketAtHomeOnly)
         || m_subscriptions == Api::SubscriptionNone
         || !(m_serverConfig->components() & ServerConfig::ComponentWebsocketApi) ) {

        return;
    }

    m_websocket.open(m_baseUrl + HASS_API_ENDPOINT_WEBSOCKET);
}

void ApiConnector::reconnectWebsocket()
{
    closeWebsocket();
    openWebsocket();
}
