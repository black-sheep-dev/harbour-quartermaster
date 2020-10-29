#ifndef HOMEASSISTANTAPI_H
#define HOMEASSISTANTAPI_H

#include "apiinterface.h"

static const QString HASS_API_ENDPOINT_CHECK_CONFIG         = QStringLiteral("/api/config/core/check_config");
static const QString HASS_API_ENDPOINT_CONFIG               = QStringLiteral("/api/config");
static const QString HASS_API_ENDPOINT_DEVICE_REGISTRATION  = QStringLiteral("/api/mobile_app/registrations");
static const QString HASS_API_ENDPOINT_ERROR_LOG            = QStringLiteral("/api/error_log");
static const QString HASS_API_ENDPOINT_LOGBOOK              = QStringLiteral("/api/logbook");
static const QString HASS_API_ENDPOINT_SERVICES             = QStringLiteral("/api/services");
static const QString HASS_API_ENDPOINT_STATES               = QStringLiteral("/api/states");

#include "src/crypto/secrets.h"
#include "src/device/device.h"

class HomeassistantApi : public ApiInterface
{
    Q_OBJECT

public:
    explicit HomeassistantApi(Wallet *wallet, QObject *parent = nullptr);

    // api calls
    void callService(const QString &domain, const QString &service, const QString &entityId, const QJsonObject &data = QJsonObject());
    void checkConfig();
    void getConfig();
    void getErrorLog();
    void getLogBook(const QDateTime &timestamp);
    void getState(const QString &entityId);
    void getStates();
    void registerDevice(Device *device);

signals:
    void errorLogAvailable(const QByteArray &data);

private:
    QStringList m_activeRequests;

    // ApiInterface interface
public:
    QNetworkRequest getRequest(const QString &endpoint) override;

private slots:
    void onRequestFinished(QNetworkReply *reply) override;
};

#endif // HOMEASSISTANTAPI_H
