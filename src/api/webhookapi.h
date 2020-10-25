#ifndef WEBHOOKAPI_H
#define WEBHOOKAPI_H

#include "apiinterface.h"


#include "src/device/device.h"

class WebhookApi : public ApiInterface
{
    Q_OBJECT

    Q_PROPERTY(bool encryption READ encryption WRITE setEncryption NOTIFY encryptionChanged)

public:
    enum ConnectionFailure {
        ConnectionFailureNone           = 0x0,
        ConnectionFailureCloudhook      = 0x1,
        ConnectionFailureRemoteUi       = 0x2,
        ConnectionFailureWebhook        = 0x4
    };
    Q_DECLARE_FLAGS(ConnectionFailures, ConnectionFailure)

    explicit WebhookApi(Wallet *wallet, QObject *parent = nullptr);

    bool isRegistered();
    void reset();
    void setRegistrationData(const QJsonObject &obj);

    // api
    void getZones();
    void registerSensor(const DeviceSensor *sensor);
    void updateLocation(const QJsonObject &location);
    void updateRegistration(const Device *device);
    void updateSensor(const QJsonObject &sensor);

    // properties
    bool encryption() const;

signals:
    void secretsNeedSave(const Secrets &secrets);

    // properties
    void encryptionChanged(bool enabled);

public slots:
    // properties
    void setEncryption(bool enable);

private slots:
    void onReplyFinished(const QString &identifier, QNetworkReply *reply);
    void updateWebhookUrl();

private:
    void sendRequest(const QString &type, const QJsonObject &payload = QJsonObject());

    QStringList m_activeRequests;
    ConnectionFailures m_connectionFailures;
    QString m_webhookUrl;

    // properties
    bool m_encryption{false};

    // ApiInterface interface
public:
    void initialize() override; 
};
Q_DECLARE_OPERATORS_FOR_FLAGS(WebhookApi::ConnectionFailures)

#endif // WEBHOOKAPI_H
