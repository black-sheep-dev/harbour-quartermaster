#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QWebSocket>

#include "api.h"
#include "credentials.h"
#include "serverconfig.h"

class ApiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool atHome READ atHome WRITE setAtHome NOTIFY atHomeChanged)
    Q_PROPERTY(quint8 connectionMode READ connectionMode WRITE setConnectionMode NOTIFY connectionModeChanged)
    Q_PROPERTY(Credentials credentials READ credentials WRITE setCredentials NOTIFY credentialsChanged)
    Q_PROPERTY(bool logging READ logging WRITE setLogging NOTIFY loggingChanged)
    Q_PROPERTY(States states READ states WRITE setStates NOTIFY statesChanged)

public:
    enum ConnectionMode {
        ConnectionAutomatic,
        ConnectionExternal,
        ConnectionInternal,
        ConnectionCloud
    };
    Q_ENUM(ConnectionMode)

    enum State {
        StateUninitialized                      = 0x0,
        StateHasConnectionInfos                 = 0x1,
        StateHasCredentialsToken                = 0x2,
        StateHasCredentialsWebhook              = 0x4
    };
    Q_ENUM(State)
    Q_DECLARE_FLAGS(States, State)

    explicit ApiInterface(QObject *parent = nullptr);
    ~ApiInterface();

    Q_INVOKABLE QString baseUrl();
    Q_INVOKABLE void getConfig();
    Q_INVOKABLE void getDiscoveryInfo(const QString &hostname, quint16 port);
    Q_INVOKABLE void initialize();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE ServerConfig *serverConfig();

    // properties
    bool atHome() const;
    quint8 connectionMode() const;
    Credentials credentials() const;    
    bool logging() const;
    States states() const;

signals:
    void requestError(quint8 requestType, quint8 code, const QString &msg = QString());
    void requestFinished(quint8 requestType, const QJsonDocument &payload = QJsonDocument());

    // properties
    void atHomeChanged(bool atHome);  
    void connectionModeChanged(quint8 connectionMode);
    void credentialsChanged(const Credentials &credentials);
    void loggingChanged(bool logging);
    void statesChanged(ApiInterface::States states);

public slots:
    void sendRequest(quint8 type,
                     const QString &query = QString(),
                     const QJsonObject &payload = QJsonObject());

    void sendWebhookRequest(quint8 type,
                            const QJsonValue &payload = QJsonValue());

    // properties
    void setConnectionMode(quint8 connectionMode);
    void setAtHome(bool atHome);
    void setCredentials(const Credentials &credentials);
    void setLogging(bool logging);
    void setStates(ApiInterface::States states);

private slots:
    void onRequestFinished();
    void onWebhookRequestFinished();

private:
    void logData(const QString &identifier, const QByteArray &data);
    void refreshStates();
    void refreshUrls();

    // settings
    void readSettings();
    void writeSettings();

    QHash<quint8, QString> m_apiEndpoints;
    QString m_baseUrl;
    ServerConfig *m_config{new ServerConfig(this)};
    QNetworkAccessManager *m_manager{new QNetworkAccessManager(this)};
    QMutex *m_mutex{new QMutex()};
    QList<quint8> m_runningRequest;
    QHash<quint8, QString> m_webhookTypes;
    QString m_webhookUrl;

    // properties
    bool m_atHome{true};
    quint8 m_connectionMode{ConnectionMode::ConnectionAutomatic};
    Credentials m_credentials;
    States m_states{State::StateUninitialized};
    bool m_logging{false};

// -----------------------------------------------------------------------------------------------------------------------------------
// WEBSOCKET
// -----------------------------------------------------------------------------------------------------------------------------------
public:
    void addWebsocketEventsSubscription(const QString &event);
    void removeWebsocketEventsSubscription(const QString &event);

    // properties
    quint8 subscriptions() const;

signals:
    void websocketEvent(const QString &event, const QJsonValue &value);

private slots:
    void onWebsocketConnected();
    void onWebsocketDisconnected();
    void onWebsocketMessageReceived(const QString &msg);

private:
    void websocketAuthenticate();
    void websocketClose();
    void websocketReconnect();
    void websocketOpen();

    void updateWebsocketSubscriptions();

    void parseWebsocketEvent(const QJsonObject &payload);
    void sendWebsocketCommand(QJsonObject payload);
    void sendWebsocketMessage(const QJsonObject &payload);

    int m_interactions{1};
    QHash<QString, int> m_subscriptionEvents;
    QWebSocket *m_websocket{new QWebSocket(APP_TARGET, QWebSocketProtocol::VersionLatest, this)};
    QString m_websocketUrl;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ApiInterface::States)

#endif // APIINTERFACE_H
