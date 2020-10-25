#ifndef WEBSOCKETAPI_H
#define WEBSOCKETAPI_H

#include "apiinterface.h"

#define     HASS_WEBSOCKET_API_ENDPOINT         "/api/websocket"

#include <QWebSocket>

class WebsocketApi : public ApiInterface
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    explicit WebsocketApi(Wallet *wallet, QObject *parent = nullptr);

    // properties
    bool active() const;

signals:
    void stateChanged(const QJsonObject &obj);

    // properties
    void activeChanged(bool active);

public slots:
    void close();
    void open();

    // properties
    void setActive(bool active = true);

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageRecieved(const QString &message);
    void onError(QAbstractSocket::SocketError error);

private:
    void authenticate();
    void parseStateChanged(const QJsonObject &obj);
    void sendCommand(QJsonObject payload);
    void sendMessage(const QJsonObject &payload);
    void subscribeToStateEvents();
    void unsubscribeToStateEvents();

    int m_interactions{1};
    int m_subscribtionStateEvents{0};
    QWebSocket m_websocket;

    // properties
    bool m_active{false};
};

#endif // WEBSOCKETAPI_H
