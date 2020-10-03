#ifndef HOMEASSISTANTAPI_H
#define HOMEASSISTANTAPI_H

#include "apiinterface.h"

#define     HASS_API_ENDPOINT_CONFIG                    "/api/config"
#define     HASS_API_ENDPOINT_DEVICE_REGISTRATION       "/api/mobile_app/registrations"

#include "src/crypto/secrets.h"
#include "src/device/device.h"

class HomeassistantApi : public ApiInterface
{
    Q_OBJECT

    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

public:
    explicit HomeassistantApi(Wallet *wallet, QObject *parent = nullptr);

    // api calls
    void getConfig();
    void registerDevice(Device *device);

    // properties
    QString token() const;

signals:
    // properties
    void tokenChanged(const QString &token);

public slots:
    // properties
    void setToken(const QString &token);

private:
    QStringList m_activeRequests;

    // properties
    QString m_token;
    Secrets m_secrets;

    // ApiInterface interface
public:
    QNetworkRequest getRequest(const QString &endpoint) override;

private slots:
    void onRequestFinished(QNetworkReply *reply) override;
};

#endif // HOMEASSISTANTAPI_H
