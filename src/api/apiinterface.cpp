#include "apiinterface.h"

#include <QAbstractSocket>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>

#include "src/constants.h"
#include "src/tools/helper.h"

static const QString HASS_API_ENDPOINT_WEBHOOK = QStringLiteral("/api/webhook/");
static const QString HASS_API_ENDPOINT_WEBSOCKET = QStringLiteral("/api/websocket");

ApiInterface::ApiInterface(QObject *parent) :
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

    m_websocket->ignoreSslErrors();
    m_websocket->setSslConfiguration(QSslConfiguration::defaultConfiguration());

    connect(m_websocket, &QWebSocket::connected, this, &ApiInterface::onWebsocketConnected);
    connect(m_websocket, &QWebSocket::disconnected, this, &ApiInterface::onWebsocketDisconnected);
    connect(m_websocket,
            static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            [](QAbstractSocket::SocketError error) {
                    qDebug() << "WEBSOCKET ERROR: " << error;
    });
    connect(m_websocket, &QWebSocket::sslErrors, []() {
        qDebug() << "WEBSOCKET ERROR";
    });

    // states changed events
    connect(this, &ApiInterface::statesChanged, [this](States states) {
       if ( states & StateHasConnectionInfos
            && states & StateHasCredentialsToken ) {

           websocketOpen();
       }
    });

    readSettings();

    refreshStates();
}

ApiInterface::~ApiInterface()
{
    writeSettings();
}

QString ApiInterface::baseUrl()
{
    return m_baseUrl;
}

void ApiInterface::getConfig()
{
    sendRequest(Api::RequestGetApiConfig);
}

void ApiInterface::getDiscoveryInfo(const QString &uri)
{
    QString url = uri;

    if (!url.contains(QRegExp(RegExp::PORT_INCLUDED)))
        url.append(QStringLiteral(":8123"));

    m_config->setInternalUrl(url);
    refreshStates();

    sendRequest(Api::RequestGetApiDiscoveryInfo, QString(), QJsonObject());
}

void ApiInterface::initialize()
{
    getConfig();
}

void ApiInterface::saveSettings()
{
    writeSettings();
}

ServerConfig *ApiInterface::serverConfig()
{
    return m_config;
}

bool ApiInterface::atHome() const
{
    return m_atHome;
}

quint8 ApiInterface::connectionMode() const
{
    return m_connectionMode;
}

Credentials ApiInterface::credentials() const
{
    return m_credentials;
}

bool ApiInterface::logging() const
{
    return m_logging;
}

ApiInterface::States ApiInterface::states() const
{
    return m_states;
}

void ApiInterface::sendRequest(quint8 type, const QString &query, const QJsonObject &payload)
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
    locker.unlock();

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
        emit requestError(type, Api::ErrorBadRequest, m_apiEndpoints.value(type) + query);
        return;
    }

    reply->setProperty(ApiKey::KEY_TYPE.toLatin1(), type);
    connect(reply, &QNetworkReply::finished, this, &ApiInterface::onRequestFinished);
}

void ApiInterface::sendWebhookRequest(quint8 type, const QJsonValue &payload)
{
#ifdef QT_DEBUG
    qDebug() << "WEBHOOK REQUEST TO: " << type;
    qDebug() << "PAYLOAD: " << payload;
#endif

    if (m_credentials.webhookId.isEmpty())
        return;

    QMutexLocker locker(m_mutex);
    if (!m_runningRequest.contains(type))
         m_runningRequest.append(type);
    locker.unlock();

    // prepare message
    QJsonObject message;
    message.insert(ApiKey::KEY_TYPE, m_webhookTypes.value(type));

    // prepare data if available
    if (!payload.isNull()) {
        if (!m_credentials.secret.isEmpty()) {
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
    connect(reply, &QNetworkReply::finished, this, &ApiInterface::onWebhookRequestFinished);
}

void ApiInterface::setConnectionMode(quint8 connectionMode)
{
    if (m_connectionMode == connectionMode)
        return;

    m_connectionMode = connectionMode;
    emit connectionModeChanged(m_connectionMode);

    refreshUrls();
}

void ApiInterface::setAtHome(bool atHome)
{
    if (m_atHome == atHome)
        return;

    m_atHome = atHome;
    emit atHomeChanged(m_atHome);

    refreshStates();
}

void ApiInterface::setCredentials(const Credentials &credentials)
{
    if (m_credentials == credentials)
        return;

    m_credentials = credentials;
    emit credentialsChanged(m_credentials);

    // refresh
    refreshStates();
}

void ApiInterface::setLogging(bool logging)
{
    if (m_logging == logging)
        return;

    m_logging = logging;
    emit loggingChanged(m_logging);
}

void ApiInterface::setStates(States states)
{
    if (m_states == states)
        return;

    m_states = states;
    emit statesChanged(m_states);
}

void ApiInterface::onRequestFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    // check reply
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const quint8 type = reply->property(ApiKey::KEY_TYPE.toLatin1()).toUInt();
    const QString typeString = m_webhookTypes.value(type);
    const QString url = reply->url().toString();
    const QNetworkReply::NetworkError error = reply->error();
    const QString errorString = reply->errorString();
    const QByteArray raw = reply->readAll();

    QMutexLocker locker(m_mutex);
    m_runningRequest.removeAll(type);
    locker.unlock();

    reply->deleteLater();

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
        emit requestError(type, Api::ErrorDataInvalid, typeString);
        return;

    case 401:
        emit requestError(type, Api::ErrorUnauthorized, typeString);
        return;

    case 404:
        emit requestError(type, Api::ErrorNotFound, typeString);
        return;

    case 405:
        emit requestError(type, Api::ErrorMethodNotAllowed, typeString);
        return;

    default:
        if (m_atHome && error == QNetworkReply::HostNotFoundError)
            setAtHome(false);

        emit requestError(type, Api::ErrorUnkown, errorString);
        return;
    }


    // begin parsing data
    QByteArray data = Helper::gunzip(raw);

    if (data.isEmpty())
        data = raw;

    QJsonParseError err{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << err.errorString();
#endif
        emit requestError(type, Api::ErrorJsonInvalid, typeString);
        return;
    }

    // logging
    if (logging())
        logData(QStringLiteral("rest_api_reply"), doc.toJson(QJsonDocument::Indented));

    // request specific handling
    switch (type) {
    case Api::RequestGetApiConfig:
        m_config->setData(doc.object());
        refreshStates();
        break;

    case Api::RequestGetApiDiscoveryInfo:
        m_config->setData(doc.object());
        break;

    default:
        emit requestFinished(type, doc);
        return;
    }

    emit requestFinished(type);
}

void ApiInterface::onWebhookRequestFinished()
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
    const QNetworkReply::NetworkError error = reply->error();
    const QString errorString = reply->errorString();

    QMutexLocker locker(m_mutex);
    m_runningRequest.removeAll(type);
    locker.unlock();

    reply->deleteLater();

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
        emit requestError(type, Api::ErrorDataInvalid, typeString);
        return;

    case 404:
        emit requestError(type, Api::ErrorMobileAppNotLoaded, typeString);
        return;

    case 410:
        emit requestError(type, Api::ErrorIntegrationDeleted, typeString);
        return;

    default:
        if (m_atHome && error == QNetworkReply::HostNotFoundError)
            setAtHome(false);

        emit requestError(type, Api::ErrorUnkown, errorString);
        return;
    }

    // begin parsing data
    QByteArray data = Helper::gunzip(raw);

    if (data.isEmpty())
        data = raw;

    QJsonParseError err{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    if (err.error) {
#ifdef QT_DEBUG
        qDebug() << "JSON PARSE ERROR";
        qDebug() << err.errorString();
#endif
        emit requestError(type, Api::ErrorJsonInvalid, typeString);
        return;
    }

    // logging
    if (logging())
        logData(QStringLiteral("webhook_reply_") + typeString, doc.toJson(QJsonDocument::Indented));

    // emit data
    emit requestFinished(type, doc);
}

void ApiInterface::logData(const QString &identifier, const QByteArray &data)
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

void ApiInterface::refreshStates()
{
    States states = m_states;

    // token
    if (m_credentials.token.isEmpty())
        states &= ~StateHasCredentialsToken;
    else
        states |= StateHasCredentialsToken;

    // webhook id
    if (m_credentials.webhookId.isEmpty())
        states &= ~StateHasCredentialsWebhook;
    else
        states |= StateHasCredentialsWebhook;

    // connection
    if ( m_config->internalUrl().startsWith(QLatin1String("http")) ) {
        states |= StateHasConnectionInfos;
    } else {
        states &= ~StateHasConnectionInfos;
    }

    // apply
    setStates(states);

    // refresh related
    refreshUrls();
}

void ApiInterface::refreshUrls()
{
    if (m_connectionMode != ConnectionAutomatic) {
        m_baseUrl = m_config->internalUrl();

        m_webhookUrl = m_config->internalUrl()
                + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;

        if ( m_connectionMode == ConnectionExternal
             && !m_config->externalUrl().isEmpty() ) {

            m_baseUrl = m_config->externalUrl();
            m_webhookUrl = m_config->externalUrl()
                    + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;

        } else if ( m_connectionMode == ConnectionCloud
                    && !m_credentials.cloudhookUrl.isEmpty() ) {

            m_baseUrl = m_config->externalUrl();
            m_webhookUrl = m_credentials.cloudhookUrl;
        }

    } else {
        // base url
        if ( m_atHome || m_config->externalUrl().isEmpty() ) {
            m_baseUrl = m_config->internalUrl();
        } else {
            m_baseUrl = m_config->externalUrl();
        }

        // webhook url
        if ( !m_credentials.cloudhookUrl.isEmpty() ) {
            m_webhookUrl = m_credentials.cloudhookUrl;
        } else if ( !m_credentials.remoteUiUrl.isEmpty() ) {
            m_webhookUrl = m_credentials.remoteUiUrl + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
        } else if ( !m_config->externalUrl().isEmpty() && !m_atHome ) {
            m_webhookUrl = m_config->externalUrl()
                    + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
        } else {
            m_webhookUrl = m_config->internalUrl()
                    + HASS_API_ENDPOINT_WEBHOOK + m_credentials.webhookId;
        }
    }

    // reconnect websocket
    m_websocketUrl = m_baseUrl;
    m_websocketUrl.replace(QStringLiteral("http"), QStringLiteral("ws"));
    m_websocketUrl.append(HASS_API_ENDPOINT_WEBSOCKET);

    websocketReconnect();
}

void ApiInterface::readSettings()
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
            internalUrl = hostname + QStringLiteral(":%1").arg(port);
        else
            internalUrl = (ssl ? QStringLiteral("https://") : QStringLiteral("http://")) + hostname + QStringLiteral(":%1").arg(port);

        settings.remove("");
        settings.endGroup();

    } else {
        settings.beginGroup(QStringLiteral("CONNECTION"));

        externalPort = quint16(settings.value(QStringLiteral("external_port"), 0).toUInt());
        externalUrl = settings.value(QStringLiteral("external_url")).toString();   

        if (externalPort != 0) {
            if (!externalUrl.contains(QRegExp(RegExp::PORT_INCLUDED)))
                externalUrl.append(QStringLiteral(":%1").arg(externalPort));
            settings.remove(QStringLiteral("external_port"));
        }

        internalPort = quint16(settings.value(QStringLiteral("internal_port"), 0).toUInt());
        internalUrl = settings.value(QStringLiteral("internal_url")).toString();

        if (internalPort != 0) {
            if (!internalUrl.contains(QRegExp(RegExp::PORT_INCLUDED)))
                internalUrl.append(QStringLiteral(":%1").arg(internalPort));
            settings.remove(QStringLiteral("internal_port"));
        }

        settings.endGroup();
    }

    if (settings.childGroups().contains(QStringLiteral("WEBHOOK_API"))) {
        settings.beginGroup(QStringLiteral("WEBHOOK_API"));
        settings.remove("");
        settings.endGroup();
    }

    settings.beginGroup(QStringLiteral("CONNECTION"));
    setConnectionMode(settings.value(QStringLiteral("connection_mode"), ConnectionMode::ConnectionAutomatic).toUInt());
    settings.endGroup();

    m_config->setExternalUrl(externalUrl);
    m_config->setInternalUrl(internalUrl);
}

void ApiInterface::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("CONNECTION"));
    settings.setValue(QStringLiteral("connecion_mode"), m_connectionMode);
    settings.setValue(QStringLiteral("external_url"), m_config->externalUrl());
    settings.setValue(QStringLiteral("internal_url"), m_config->internalUrl());
    settings.endGroup();
}

void ApiInterface::addWebsocketEventsSubscription(const QString &event)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket add subscription: ") << event;
#endif

    if ( m_subscriptionEvents.keys().contains(event)
         && m_subscriptionEvents.value(event) >= 0 ) {
        return;
    }

    m_subscriptionEvents.insert(event, 0);

    updateWebsocketSubscriptions();
}

void ApiInterface::removeWebsocketEventsSubscription(const QString &event)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket remove subscription :") << event;
#endif

    if (!m_subscriptionEvents.keys().contains(event))
        return;

    m_subscriptionEvents.insert(event, m_subscriptionEvents.value(event) * -1);

    updateWebsocketSubscriptions();
}

void ApiInterface::onWebsocketConnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket connected: ") << m_websocket->requestUrl().toString();
#endif

    connect(m_websocket, &QWebSocket::textMessageReceived, this, &ApiInterface::onWebsocketMessageReceived);
}

void ApiInterface::onWebsocketDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket disconnected: ") << m_websocket->requestUrl().toString();
#endif

    if ( m_websocket->closeCode() == QWebSocketProtocol::CloseCodeGoingAway
         && m_websocket->closeReason() == QLatin1String("RECONNECT") ) {

        websocketOpen();
    }
}

void ApiInterface::onWebsocketMessageReceived(const QString &msg)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket message received");
    //qDebug() << msg;
#endif

    QJsonParseError error{};

    const auto obj = QJsonDocument::fromJson(msg.toLatin1(), &error).object();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
        qDebug() << error.errorString();
#endif
        return;
    }

    const auto type = obj.value(ApiKey::KEY_TYPE).toString();

    if (type == ApiKey::KEY_EVENT) {
        parseWebsocketEvent(obj);

    } else if (type == ApiKey::KEY_AUTH_REQUIRED) {
        websocketAuthenticate();

    } else if (type == ApiKey::KEY_AUTH_OK) {
        updateWebsocketSubscriptions();
    }
}

void ApiInterface::websocketAuthenticate()
{
    QJsonObject payload;
    payload.insert(ApiKey::KEY_TYPE, ApiKey::KEY_AUTH);
    payload.insert(ApiKey::KEY_ACCESS_TOKEN, m_credentials.token);

    sendWebsocketMessage(payload);
}

void ApiInterface::websocketClose()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket close");
#endif

    m_websocket->close();
}

void ApiInterface::websocketReconnect()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket reconnect");
#endif

    if (!m_websocket->isValid()) {
        websocketOpen();
        return;
    }

    // m_websocktUrl didn't changed
    if (m_websocket->requestUrl().toString().startsWith(m_websocketUrl))
        return;

    m_websocket->close(QWebSocketProtocol::CloseCodeGoingAway, QStringLiteral("RECONNECT"));
}

void ApiInterface::websocketOpen()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket open");
#endif

    if ( !(m_states & StateHasConnectionInfos)
         || !(m_states & StateHasCredentialsToken) )
        return;

    if (m_websocket->isValid())
        return;

    if ( m_websocketUrl.isEmpty() || m_subscriptionEvents.isEmpty())
        return;

    QHashIterator<QString, int> iter(m_subscriptionEvents);
    while (iter.hasNext()) {
        iter.next();

        m_subscriptionEvents.insert(iter.key(), 0);
    }

    m_websocket->open(m_websocketUrl);
}

void ApiInterface::updateWebsocketSubscriptions()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket subscription update");
    qDebug() << m_subscriptionEvents.keys();
#endif

    if ( !m_subscriptionEvents.isEmpty()
         && !m_websocket->isValid() ) {
        websocketOpen();
        return;
    }

    if ( m_subscriptionEvents.isEmpty()
         && m_websocket->isValid() ) {
        websocketClose();
        return;
    }

    QHashIterator<QString, int> iter(m_subscriptionEvents);
    while (iter.hasNext()) {
        iter.next();

        QJsonObject payload;

        if (iter.value() < 0) {
            payload.insert(ApiKey::KEY_TYPE, ApiKey::KEY_UNSUBSCRIBE_EVENTS);
            payload.insert(ApiKey::KEY_SUBSCRIPTION, iter.value() * -1);

            m_subscriptionEvents.remove(iter.key());

        } else if (iter.value() == 0) {
            payload.insert(ApiKey::KEY_TYPE, ApiKey::KEY_SUBSCRIBE_EVENTS);
            payload.insert(ApiKey::KEY_EVENT_TYPE, iter.key());

            m_subscriptionEvents.insert(iter.key(), m_interactions);

        } else {
            continue;
        }

        sendWebsocketCommand(payload);
    }
}

void ApiInterface::parseWebsocketEvent(const QJsonObject &payload)
{
    const QJsonObject event = payload.value(ApiKey::KEY_EVENT).toObject();
    const QJsonObject data = event.value(ApiKey::KEY_DATA).toObject();

    if (data.isEmpty())
        return;

    emit websocketEvent(event.value(ApiKey::KEY_EVENT_TYPE).toString(),
                        data);
}

void ApiInterface::sendWebsocketCommand(QJsonObject payload)
{
    payload.insert(ApiKey::KEY_ID, m_interactions);
    m_interactions++;

    m_websocket->sendTextMessage(QJsonDocument(payload).toJson(QJsonDocument::Compact));
}

void ApiInterface::sendWebsocketMessage(const QJsonObject &payload)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket message send: ");
    qDebug() << payload;
#endif

    m_websocket->sendTextMessage(QJsonDocument(payload).toJson(QJsonDocument::Compact));
}
