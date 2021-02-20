#include "websocketapi.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDateTime>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include <nemonotifications-qt5/notification.h>

WebsocketApi::WebsocketApi(Wallet *wallet, QObject *parent) :
    QObject(parent),
    m_wallet(wallet)
{
    connect(&m_websocket, &QWebSocket::connected, this, &WebsocketApi::onConnected);
    connect(&m_websocket, &QWebSocket::disconnected, this, &WebsocketApi::onDisconnected);
    connect(wallet, &Wallet::initialized, this, &WebsocketApi::open);
}

QString WebsocketApi::baseUrl() const
{
    return m_baseUrl;
}

quint8 WebsocketApi::subscriptions() const
{
    return m_subscriptions;
}

void WebsocketApi::close()
{ 
    m_websocket.close();
}

void WebsocketApi::open()
{
    if ( m_websocket.state() != QAbstractSocket::UnconnectedState
         || m_subscriptions == SubscriptionNone ) {

        return;
    }

    m_websocket.open(m_baseUrl + HASS_WEBSOCKET_API_ENDPOINT);
}

void WebsocketApi::setBaseUrl(const QString &url)
{
    if (m_baseUrl == url)
        return;

    m_baseUrl = url;
    emit baseUrlChanged(m_baseUrl);
}

void WebsocketApi::setSubscriptions(quint8 subscriptions)
{
    if (m_subscriptions == subscriptions)
        return;

    m_subscriptions = subscriptions;
    emit subscriptionsChanged(m_subscriptions);

    updateSubscriptions();
}

void WebsocketApi::onConnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket connected");
#endif

    connect(&m_websocket, &QWebSocket::textMessageReceived, this, &WebsocketApi::onMessageReceived);
}

void WebsocketApi::onDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket disconnected");
#endif
}

void WebsocketApi::onMessageReceived(const QString &message)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket message received");
    qDebug() << message;
#endif

    QJsonParseError error{};

    const QJsonObject data = QJsonDocument::fromJson(message.toLatin1(), &error).object();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
        qDebug() << error.errorString();
#endif
        return;
    }

    const QString type = data.value(QStringLiteral("type")).toString();

    if (type == QStringLiteral("event")) {
        parseEvent(data);
    } else if (type == QStringLiteral("auth_required")) {
        authenticate();
    } else if (type == QStringLiteral("auth_ok")) {
        updateSubscriptions();
    }
}

void WebsocketApi::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
}

void WebsocketApi::authenticate()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("auth"));
    payload.insert(QStringLiteral("access_token"), m_wallet->token());

    sendMessage(payload);
}

void WebsocketApi::parseEvent(const QJsonObject &obj)
{
    const QJsonObject event = obj.value(QStringLiteral("event")).toObject();
    const QJsonObject data = event.value(QStringLiteral("data")).toObject();

    if (data.isEmpty())
        return;

    const QString eventType = event.value(QStringLiteral("event_type")).toString();

    if (eventType == QStringLiteral("state_changed")) {
        emit stateChanged(data.value(QStringLiteral("new_state")).toObject());

    } else if (eventType == QStringLiteral("call_service")) {
        if ( data.value(QStringLiteral("domain")).toString() == QStringLiteral("notify")
             && data.value(QStringLiteral("service")).toString() == QStringLiteral("notify") ) {
            sendNotification(data.value(QStringLiteral("service_data")).toObject());
        }
    }
}

void WebsocketApi::sendCommand(QJsonObject payload)
{
    payload.insert(QStringLiteral("id"), m_interactions);
    m_interactions++;

    m_websocket.sendTextMessage(QJsonDocument(payload).toJson(QJsonDocument::Compact));
}

void WebsocketApi::sendMessage(const QJsonObject &payload)
{
    m_websocket.sendTextMessage(QJsonDocument(payload).toJson(QJsonDocument::Compact));
}

void WebsocketApi::sendNotification(const QJsonObject &data)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("NOTIFICATION");
#endif

    Notification notify;
    notify.setCategory("x-nemo.messaging.sms");
    notify.setAppIcon(QStringLiteral("image://theme/harbour-quartermaster"));
    notify.setBody(data.value(QStringLiteral("message")).toString());
    notify.setSummary(data.value(QStringLiteral("title")).toString());
    //notify.setIcon(QStringLiteral("image://theme/icon-lock-information"));
    notify.setUrgency(Notification::Critical);
    //notify.setHintValue("sound-file", "/usr/share/sounds/jolla-ringtones/stereo/poppy-red-alert-1.ogg");
    notify.publish();
}

void WebsocketApi::subscribeToNotifyEvents()
{
    if (m_subscriptionNotifyEvents != 0)
        return;

    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("subscribe_events"));
    payload.insert(QStringLiteral("event_type"), QStringLiteral("call_service"));

    m_subscriptionNotifyEvents = m_interactions;

    sendCommand(payload);
}

void WebsocketApi::subscribeToStateChanged()
{
    if (m_subscriptionStateEvents != 0)
        return;

    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("subscribe_events"));
    payload.insert(QStringLiteral("event_type"), QStringLiteral("state_changed"));

    m_subscriptionStateEvents = m_interactions;

    sendCommand(payload);
}

void WebsocketApi::unsubscribeFromNotifyEvents()
{
    if (m_subscriptionNotifyEvents == 0)
        return;

    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("unsubscribe_events"));
    payload.insert(QStringLiteral("subscription"), m_subscriptionNotifyEvents);

    m_subscriptionNotifyEvents = 0;

    sendCommand(payload);
}

void WebsocketApi::unsubscribeFromStateChanged()
{
    if (m_subscriptionStateEvents == 0)
        return;

    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("unsubscribe_events"));
    payload.insert(QStringLiteral("subscription"), m_subscriptionStateEvents);

    m_subscriptionStateEvents = 0;

    sendCommand(payload);
}

void WebsocketApi::updateSubscriptions()
{
    if (!m_websocket.isValid()) {
        open();
        return;
    }

    if (m_subscriptions & SubscriptionNotifyEvents)
        subscribeToNotifyEvents();
    else
        unsubscribeFromNotifyEvents();

    if (m_subscriptions & SubscriptionStateChanged)
        subscribeToStateChanged();
    else
        unsubscribeFromStateChanged();
}
