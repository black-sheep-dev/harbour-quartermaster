#include "websocketapi.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonParseError>
#include <QJsonDocument>
#include <QTimer>

WebsocketApi::WebsocketApi(Wallet *wallet, QObject *parent) :
    ApiInterface(wallet, parent)
{
    m_websocket.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    connect(&m_websocket, &QWebSocket::connected, this, &WebsocketApi::onConnected);
    connect(&m_websocket, &QWebSocket::disconnected, this, &WebsocketApi::onDisconnected);
    connect(wallet, &Wallet::initialized, this, &WebsocketApi::open);
}

bool WebsocketApi::active() const
{
    return m_active;
}

void WebsocketApi::close()
{
    m_websocket.close(QWebSocketProtocol::CloseCodeNormal);
}

void WebsocketApi::open()
{
    if (!m_active)
        return;

    m_websocket.open(baseUrl() + QStringLiteral(HASS_WEBSOCKET_API_ENDPOINT));
}

void WebsocketApi::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged(m_active);

    m_active ? open() : close();
}

void WebsocketApi::onConnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket connected");
#endif

    connect(&m_websocket, &QWebSocket::textMessageReceived, this, &WebsocketApi::onMessageRecieved);
}

void WebsocketApi::onDisconnected()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket disconnected");
#endif

    if ( m_websocket.closeCode() != QWebSocketProtocol::CloseCodeNormal
         && m_active ) {

        open();
    }

    m_interactions = 1;
    m_subscribtionStateEvents = 0;
}

void WebsocketApi::onMessageRecieved(const QString &message)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Websocket message recieved");
    qDebug() << message;
#endif

    QJsonParseError error;

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
        parseStateChanged(data);
    } else if (type == QStringLiteral("auth_required")) {
        authenticate();
    } else if (type == QStringLiteral("auth_ok")) {
        subscribeToStateEvents();
    }
}

void WebsocketApi::onError(QAbstractSocket::SocketError error)
{

}

void WebsocketApi::authenticate()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("auth"));
    payload.insert(QStringLiteral("access_token"), wallet()->token());

    sendMessage(payload);
}

void WebsocketApi::parseStateChanged(const QJsonObject &obj)
{
    const QJsonObject data = obj.value(QStringLiteral("event")).toObject().value(QStringLiteral("data")).toObject();

    if (data.isEmpty())
        return;

    emit stateChanged(data.value(QStringLiteral("new_state")).toObject());
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

void WebsocketApi::subscribeToStateEvents()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("subscribe_events"));
    payload.insert(QStringLiteral("event_type"), QStringLiteral("state_changed"));

    m_subscribtionStateEvents = m_interactions;

    sendCommand(payload);
}

void WebsocketApi::unsubscribeToStateEvents()
{
    QJsonObject payload;
    payload.insert(QStringLiteral("type"), QStringLiteral("unsubscribe_events"));
    payload.insert(QStringLiteral("subscription"), m_subscribtionStateEvents);

    m_subscribtionStateEvents = 0;

    sendCommand(payload);
}
