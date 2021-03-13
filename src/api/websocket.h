#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>

#include <QHash>
#include <QWebSocket>

#include "api.h"

class WebSocket : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 subscriptions READ subscriptions WRITE setSubscriptions NOTIFY subscriptionsChanged)

public:
    explicit WebSocket(QObject *parent = nullptr);

    void setBaseUrl(const QString &url);
    void setToken(const QString &token);

    // properties
    quint8 subscriptions() const;

signals:
    void entityStateChanged(const QJsonObject &data);
    void notificationAvailable(const QJsonObject &data);

    // properties
    void subscriptionsChanged(quint8 subscriptions);

public slots:
    void close();
    void open();

    // properties
    void setSubscriptions(quint8 subscriptions);

private slots:
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QString &msg);

private:
    void authenticate();
    void parseEvent(const QJsonObject &obj);
    void sendCommand(QJsonObject payload);
    void sendMessage(const QJsonObject &payload);
    void subscribeToEvents(quint8 subscription);
    void unsubscribeFromEvents(quint8 subscription);
    void updateSubcriptions();

    QString m_baseUrl;
    QWebSocket m_websocket;
    int m_interactions{1};
    QHash<quint8, int> m_subscriptionEvents;
    QString m_token;

    // properties;
    quint8 m_subscriptions{Api::SubscriptionNone};

};

#endif // WEBSOCKET_H
