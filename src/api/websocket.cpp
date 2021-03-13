#include "websocket.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include "src/constants.h"

static const QString HASS_API_ENDPOINT_WEBSOCKET = QStringLiteral("/api/websocket/");

WebSocket::WebSocket(QObject *parent) : QObject(parent)
{
    connect(&m_websocket, &QWebSocket::connected, this, &WebSocket::onConnected);
    connect(&m_websocket, &QWebSocket::disconnected, this, &WebSocket::onDisconnected);
}

void WebSocket::setBaseUrl(const QString &url)
{
    if (m_baseUrl == url)
        return;

    m_baseUrl = url;
}

void WebSocket::setToken(const QString &token)
{
    if (m_token == token)
        return;

    m_token = token;
}

quint8 WebSocket::subscriptions() const
{
    return m_subscriptions;
}

void WebSocket::close()
{
    m_websocket.close();
}

void WebSocket::open()
{
    m_interactions = 0;
    m_subscriptionEvents.clear();

    m_websocket.open(m_baseUrl + HASS_API_ENDPOINT_WEBSOCKET);
}

void WebSocket::setSubscriptions(quint8 subscriptions)
{
    if (m_subscriptions == subscriptions)
        return;

    m_subscriptions = subscriptions;
    emit subscriptionsChanged(m_subscriptions);

    updateSubcriptions();
}

void WebSocket::onConnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket connected");
#endif

    connect(&m_websocket, &QWebSocket::textMessageReceived, this, &WebSocket::onMessageReceived);
}

void WebSocket::onDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket disconnected");
#endif
}

void WebSocket::onMessageReceived(const QString &msg)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket message received");
    qDebug() << msg;
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
         parseEvent(obj);

     } else if (type == ApiKey::KEY_AUTH_REQUIRED) {
        authenticate();

     } else if (type == ApiKey::KEY_AUTH_OK) {
         updateSubcriptions();
     }
}

void WebSocket::authenticate()
{
    QJsonObject payload;
    payload.insert(ApiKey::KEY_TYPE, ApiKey::KEY_AUTH);
    payload.insert(ApiKey::KEY_ACCESS_TOKEN, m_token);

    sendMessage(payload);
}

void WebSocket::parseEvent(const QJsonObject &obj)
{
    const QJsonObject event = obj.value(ApiKey::KEY_EVENT).toObject();
    const QJsonObject data = event.value(ApiKey::KEY_DATA).toObject();

    if (data.isEmpty())
        return;

    const QString eventType = event.value(ApiKey::KEY_EVENT_TYPE).toString();

    if (eventType == ApiKey::KEY_STATE_CHANGED) {
        emit entityStateChanged(data.value(ApiKey::KEY_NEW_STATE).toObject());

    } else if (eventType == ApiKey::KEY_CALL_SERVICE) {
        if ( data.value(ApiKey::KEY_DOMAIN).toString() == ApiKey::KEY_NOTIFY
             && data.value(ApiKey::KEY_SERVICE).toString() == ApiKey::KEY_NOTIFY ) {
            notificationAvailable(data.value(ApiKey::KEY_SERVICE_DATA).toObject());
        }
    }
}

void WebSocket::sendCommand(QJsonObject payload)
{
    payload.insert(ApiKey::KEY_ID, m_interactions);
    m_interactions++;

    m_websocket.sendTextMessage(QJsonDocument(payload).toJson(QJsonDocument::Compact));
}

void WebSocket::sendMessage(const QJsonObject &payload)
{
    m_websocket.sendTextMessage(QJsonDocument(payload).toJson(QJsonDocument::Compact));
}

void WebSocket::subscribeToEvents(quint8 subscription)
{
    if (m_subscriptionEvents.value(subscription, 0) != 0)
            return;

    QJsonObject payload;
    payload.insert(ApiKey::KEY_TYPE, ApiKey::KEY_SUBSCRIBE_EVENTS);

    switch (subscription) {
    case Api::SubscriptionNotifyEvents:
        payload.insert(ApiKey::KEY_EVENT_TYPE, ApiKey::KEY_CALL_SERVICE);
        break;

    case Api::SubscriptionStateChanged:
        payload.insert(ApiKey::KEY_EVENT_TYPE, ApiKey::KEY_STATE_CHANGED);
        break;

    default:
        return;
    }


    m_subscriptionEvents.insert(subscription, m_interactions);

    sendCommand(payload);
}

void WebSocket::unsubscribeFromEvents(quint8 subscription)
{
    if (m_subscriptionEvents.value(subscription, 0) == 0)
        return;

    QJsonObject payload;
    payload.insert(ApiKey::KEY_TYPE, ApiKey::KEY_UNSUBSCRIBE_EVENTS);
    payload.insert(ApiKey::KEY_SUBSCRIPTION, m_subscriptionEvents.value(subscription));

    m_subscriptionEvents.insert(subscription, 0);

    sendCommand(payload);
}

void WebSocket::updateSubcriptions()
{
    if (!m_websocket.isValid()) {
        open();
        return;
    }

    if (m_subscriptions & Api::SubscriptionNotifyEvents)
        subscribeToEvents(Api::SubscriptionNotifyEvents);
    else
        unsubscribeFromEvents(Api::SubscriptionNotifyEvents);

    if (m_subscriptions & Api::SubscriptionStateChanged)
        subscribeToEvents(Api::SubscriptionStateChanged);
    else
        unsubscribeFromEvents(Api::SubscriptionStateChanged);
}
