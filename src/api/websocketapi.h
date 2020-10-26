#ifndef WEBSOCKETAPI_H
#define WEBSOCKETAPI_H

#define     HASS_WEBSOCKET_API_ENDPOINT         "/api/websocket"

#include <QObject>

#include <QWebSocket>

#include "src/crypto/wallet.h"

class WebsocketApi : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(quint8 subscriptions READ subscriptions WRITE setSubscriptions NOTIFY subscriptionsChanged)

public:
    enum Subscription {
        SubscriptionNone                = 0x0,
        SubscriptionNotifyEvents        = 0x1,
        SubscriptionStateChanged        = 0x2
    };
    Q_ENUM(Subscription)
    Q_DECLARE_FLAGS(Subscriptions, Subscription)

    explicit WebsocketApi(Wallet *wallet, QObject *parent = nullptr);

    // properties
    QString baseUrl() const;
    quint8 subscriptions() const;

signals:
    void stateChanged(const QJsonObject &obj);

    // properties
    void baseUrlChanged(const QString &url);
    void subscriptionsChanged(quint8 subscriptions);

public slots:
    void close();
    void open();

    // properties   
    void setBaseUrl(const QString &url);
    void setSubscriptions(quint8 subscriptions);

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QString &message);
    void onError(QAbstractSocket::SocketError error);

private:
    void authenticate();
    void parseEvent(const QJsonObject &obj);
    void sendCommand(QJsonObject payload);
    void sendMessage(const QJsonObject &payload);
    void sendNotification(const QJsonObject &data);
    void subscribeToNotifyEvents();
    void subscribeToStateChanged();
    void unsubscribeFromNotifyEvents();
    void unsubscribeFromStateChanged();
    void updateSubscriptions();

    int m_interactions{1};
    int m_subscriptionNotifyEvents{0};
    int m_subscriptionStateEvents{0};
    Wallet *m_wallet{nullptr};
    QWebSocket m_websocket;

    // properties
    QString m_baseUrl;
    quint8 m_subscriptions{SubscriptionNone};

};
Q_DECLARE_OPERATORS_FOR_FLAGS(WebsocketApi::Subscriptions)

#endif // WEBSOCKETAPI_H
