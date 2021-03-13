#ifndef APICONNECTOR_H
#define APICONNECTOR_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QWebSocket>

#include "api.h"
#include "credentials.h"
#include "serverconfig.h"

class ApiConnector : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool atHome READ atHome WRITE setAtHome NOTIFY atHomeChanged)
    Q_PROPERTY(quint8 connectionModes READ connectionModes WRITE setConnectionModes NOTIFY connectionModesChanged)
    Q_PROPERTY(Credentials credentials READ credentials WRITE setCredentials NOTIFY credentialsChanged)
    Q_PROPERTY(bool encryption READ encryption WRITE setEncryption NOTIFY encryptionChanged)
    Q_PROPERTY(bool logging READ logging WRITE setLogging NOTIFY loggingChanged)

    // WEBSOCKET PROPERTIES
    Q_PROPERTY(quint8 subscriptions READ subscriptions WRITE setSubscriptions NOTIFY subscriptionsChanged)
    Q_PROPERTY(bool websocketAtHomeOnly READ websocketAtHomeOnly WRITE setWebsocketAtHomeOnly NOTIFY websocketAtHomeOnlyChanged)

public:
    enum ConnectionFailure {
        ConnectionFailureNone           = 0x00,
        ConnectionFailureInternal       = 0x01,
        ConnectionFailureExternal       = 0x02,
        ConnectionFailureCloudhook      = 0x04,
        ConnectionFailureRemoteUi       = 0x08,
        ConnectionFailureWebhook        = 0x10
    };
    Q_DECLARE_FLAGS(ConnectionFailures, ConnectionFailure)

    enum ConnectionMode {
        UndefinedConnection             = 0x0,
        InternalConnection              = 0x1,
        ExternalConnection              = 0x2
    };
    Q_ENUM(ConnectionMode)
    Q_DECLARE_FLAGS(ConnectionModes, ConnectionMode)

    explicit ApiConnector(QObject *parent = nullptr);

    Q_INVOKABLE QString baseUrl() const;
    Q_INVOKABLE void connectToHost();
    void initialize();
    Q_INVOKABLE ServerConfig *serverConfig();

    // api calls
    Q_INVOKABLE void callService(const QString &domain,
                                 const QString &service,
                                 const QString &entityId);
    Q_INVOKABLE void callService(const QString &domain,
                                 const QString &service,
                                 const QJsonObject &payload = QJsonObject());
    Q_INVOKABLE void checkApi();
    Q_INVOKABLE void checkConfig();
    Q_INVOKABLE void fireEvent(const QString &event, const QJsonObject &payload = QJsonObject());
    Q_INVOKABLE void getCameraProxy(const QString &entityId);
    Q_INVOKABLE void getConfig();
    Q_INVOKABLE void getDiscoveryInfo(const QString &hostname, quint16 port);
    Q_INVOKABLE void getEntityState(const QString &entityId);
    Q_INVOKABLE void getErrorLog();
    Q_INVOKABLE void getEvents();
    Q_INVOKABLE void getHistory(const QDateTime &timestamp = QDateTime(),
                                const QStringList &entityIds = QStringList(),
                                const QDateTime &endTime = QDateTime(),
                                bool minimal = false,
                                bool significantChangesOnly = false);
    Q_INVOKABLE void getLogbook(const QDateTime &timestamp = QDateTime(),
                                const QString &entityId = QString(),
                                const QDateTime &endTime = QDateTime());
    Q_INVOKABLE void getServices();
    Q_INVOKABLE void getStates();
    Q_INVOKABLE void registerDevice(const QJsonObject &object);
    Q_INVOKABLE void setEntityState(const QString &entityId, const QJsonObject &payload);

    // properties
    bool atHome() const;
    quint8 connectionModes() const;
    Credentials credentials() const;
    bool encryption() const;
    bool logging() const;  


signals:
    void error(quint8 requestType, quint8 code, const QString &msg = QString());
    void connectionFailure(ApiConnector::ConnectionFailures failures);
    void requestFinished(quint8 requestType, bool success);
    void requestDataFinished(quint64 requestType, const QJsonDocument &payload = QJsonDocument());
    void requestRegistrationRefresh();

    // websocket
    void entityStateChanged(const QJsonObject &data);
    void notificationAvailable(const QJsonObject &data);

    // properties
    void atHomeChanged(bool atHome);
    void connectionModesChanged(quint8 modes);
    void credentialsChanged(const Credentials &credentials);
    void encryptionChanged(bool encryption);
    void loggingChanged(bool logging);

public slots:
    void sendRequest(quint8 type,
                     const QString &query = QString(),
                     const QJsonObject &payload = QJsonObject(),
                     bool token = true);

    void sendWebhookRequest(quint8 type,
                            const QJsonObject &payload = QJsonObject());

    // properties
    void setAtHome(bool atHome);
    void setConnectionModes(quint8 modes);
    void setCredentials(const Credentials &credentials);
    void setEncryption(bool encryption);
    void setLogging(bool logging);


private slots:
    void onFinished();
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void onWebhookFinished();

private:
    // helper functions
    QByteArray gunzip(const QByteArray &data);
    void logData(const QString &identifier, const QByteArray &data);
    void parseDeviceRegistration(const QJsonObject &data);
    void parseConfigCheck(const QJsonObject &data);
    void refreshBaseUrl();
    void refreshWebhookUrl();
    void updateConnectionFailures(const QString &url);

    // members
    QHash<quint8, QString> m_apiEndpoints;
    QString m_baseUrl;
    ConnectionFailures m_connectionFailures;
    QNetworkAccessManager *m_manager{new QNetworkAccessManager(this)};
    QMutex *m_mutex{new QMutex()};
    QList<quint8> m_runningRequest;
    ServerConfig *m_serverConfig{new ServerConfig(this)};
    QHash<quint8, QString> m_webhookTypes;
    QString m_webhookUrl;

    // properties
    bool m_atHome{false};
    quint8 m_connectionModes{ConnectionMode::ExternalConnection | ConnectionMode::InternalConnection};
    Credentials m_credentials;
    bool m_encryption{false};
    bool m_logging{false};

// -----------------------------------------------------------------------------------------------------------------------------------------------
// WEBSOCKET CONNECTIVITY
// -----------------------------------------------------------------------------------------------------------------------------------------------
public:
    // properties
    quint8 subscriptions() const;
    bool websocketAtHomeOnly() const;

signals:
    // properties
    void subscriptionsChanged(quint8 subscriptions);
    void websocketAtHomeOnlyChanged(bool only);

public slots:
    // properties
    void setSubscriptions(quint8 subscriptions);
    void setWebsocketAtHomeOnly(bool only);

private slots:

private:
    void closeWebsocket();
    void openWebsocket();
    void reconnectWebsocket();


    QWebSocket m_websocket;

    // properties
    quint8 m_subscriptions{Api::SubscriptionNone};
    bool m_websocketAtHomeOnly{false};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ApiConnector::ConnectionFailures)
Q_DECLARE_OPERATORS_FOR_FLAGS(ApiConnector::ConnectionModes)

#endif // APICONNECTOR_H
