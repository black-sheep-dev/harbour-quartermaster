#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>
#include <QNetworkAccessManager>

#include "api.h"
#include "credentials.h"
#include "serverconfig.h"

class ApiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool atHome READ atHome WRITE setAtHome NOTIFY atHomeChanged)
    Q_PROPERTY(Credentials credentials READ credentials WRITE setCredentials NOTIFY credentialsChanged)
    Q_PROPERTY(bool logging READ logging WRITE setLogging NOTIFY loggingChanged)

public:
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
    Credentials credentials() const;
    bool logging() const;

signals:
    void requestError(quint8 requestType, quint8 code, const QString &msg = QString());
    void requestFinished(quint8 requestType, const QJsonDocument &payload = QJsonDocument());

    // properties
    void atHomeChanged(bool atHome);
    void credentialsChanged(const Credentials &credentials);
    void loggingChanged(bool logging);

public slots:
    void sendRequest(quint8 type,
                     const QString &query = QString(),
                     const QJsonObject &payload = QJsonObject());

    void sendWebhookRequest(quint8 type,
                            const QJsonValue &payload = QJsonValue());

    // properties
    void setAtHome(bool atHome);
    void setCredentials(const Credentials &credentials);
    void setLogging(bool logging);

private slots:
    void onRequestFinished();
    void onWebhookRequestFinished();

private:
    void logData(const QString &identifier, const QByteArray &data);
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
    Credentials m_credentials;
    bool m_logging{false};
};

#endif // APIINTERFACE_H
