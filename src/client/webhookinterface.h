#ifndef WEBHOOKINTERFACE_H
#define WEBHOOKINTERFACE_H

#include <QObject>

#include <QJsonObject>
#include <QNetworkAccessManager>

#include "src/device/device.h"

class WebhookInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString cloudhookUrl READ cloudhookUrl WRITE setCloudhookUrl NOTIFY cloudhookUrlChanged)
    Q_PROPERTY(bool encryption READ encryption WRITE setEncryption NOTIFY encryptionChanged)
    Q_PROPERTY(QString remoteUiUrl READ remoteUiUrl WRITE setRemoteUiUrl NOTIFY remoteUiUrlChanged)
    Q_PROPERTY(QString secret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(QString webhookId READ webhookId WRITE setWebhookId NOTIFY webhookIdChanged)

public:
    enum ConnectionFailure {
        ConnectionFailureNone           = 0x0,
        ConnectionFailureCloudhook      = 0x1,
        ConnectionFailureRemoteUi       = 0x2,
        ConnectionFailureWebhook        = 0x4
    };
    Q_DECLARE_FLAGS(ConnectionFailures, ConnectionFailure)

    explicit WebhookInterface(Device *device, QObject *parent = nullptr);
    ~WebhookInterface();

    bool isValid() const;
    void resetSettings();
    void saveSettings();
    void setBaseUrl(const QString &url);
    void setSsl(bool enable);

    // properties
    QString cloudhookUrl() const;
    bool encryption() const;
    QString remoteUiUrl() const;
    QString secret() const;
    QString webhookId() const;

signals:
    // properties
    void cloudhookUrlChanged(const QString &url);
    void encryptionChanged(bool enabled);
    void remoteUiUrlChanged(const QString &url);
    void secretChanged(const QString &secret);
    void webhookIdChanged(const QString &id);

public slots:
    // webhook api
    void registerSensors();
    void updateLocation(const QJsonObject &location);
    void updateSensor(const QJsonObject &sensor);

    // properties
    void setCloudhookUrl(const QString &cloudhookUrl);
    void setEncryption(bool enabled);
    void setRemoteUiUrl(const QString &url);
    void setSecret(const QString &secret);
    void setWebhookId(const QString &id);

private slots:
    void onReply();
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void sendRequest(const QString &type, const QJsonObject &payload);

private:
    // api helper
    void updateWebhookUrl();

    // settings
    void readSettings();
    void writeSettings();

    QString m_baseUrl;
    ConnectionFailures m_connectionFailures;
    Device *m_device;
    QNetworkAccessManager *m_manager;
    bool m_ssl;
    QString m_webhookUrl;

    // properties
    QString m_cloudhookUrl;
    bool m_encryption;
    QString m_remoteUiUrl;
    QString m_secret;
    QString m_webhookId;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(WebhookInterface::ConnectionFailures)

#endif // WEBHOOKINTERFACE_H
