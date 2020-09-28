#ifndef WEBHOOKAPI_H
#define WEBHOOKAPI_H

#include "apiinterface.h"

#include "src/device/device.h"

class WebhookApi : public ApiInterface
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

    explicit WebhookApi(QObject *parent = nullptr);

    bool isRegistered() const;
    void reset();
    void setRegistrationData(const QJsonObject &obj);

    // api
    void getZones();
    void registerSensor(const DeviceSensor *sensor);
    void updateLocation(const QJsonObject &location);
    void updateRegistration(const Device *device);
    void updateSensor(const QJsonObject &sensor);

    // properties
    QString cloudhookUrl() const;
    bool encryption() const;
    QString remoteUiUrl() const;
    QString secret() const;
    QString webhookId() const;

signals:
    // properties
    void cloudhookUrlChanged(const QString &url);
    void encryptionChanged(bool encryption);
    void remoteUiUrlChanged(const QString &url);
    void secretChanged(const QString &secret);
    void webhookIdChanged(const QString &id);

public slots:
    // properties
    void setCloudhookUrl(const QString &url);
    void setEncryption(bool encryption);
    void setRemoteUiUrl(const QString &url);
    void setSecret(const QString &secret);
    void setWebhookId(const QString &id);

private slots:
    void onReplyFinished(const QString &identifier, QNetworkReply *reply);
    void updateWebhookUrl();

private:
    void sendRequest(const QString &type, const QJsonObject &payload = QJsonObject());

    ConnectionFailures m_connectionFailures;
    QString m_webhookUrl;

    // properties
    QString m_cloudhookUrl;
    bool m_encryption;
    QString m_remoteUiUrl;
    QString m_secret;
    QString m_webhookId;

    // ApiInterface interface
public:
    void initialize() override;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(WebhookApi::ConnectionFailures)

#endif // WEBHOOKAPI_H
