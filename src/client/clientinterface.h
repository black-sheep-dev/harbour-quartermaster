#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QObject>

#include "src/api/homeassistantapi.h"
#include "src/api/webhookapi.h"
#include "src/api/websocketapi.h"

#include "entitiesprovider.h"
#include "homeassistantinfo.h"
#include "src/crypto/wallet.h"
#include "src/device/device.h"
#include "src/device/trackers/devicetrackergps.h"
#include "src/device/trackers/devicetrackerwifi.h"

class ClientInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool apiLogging READ apiLogging WRITE setApiLogging NOTIFY apiLoggingChanged)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(bool ssl READ ssl WRITE setSsl NOTIFY sslChanged)
    Q_PROPERTY(QString token READ token WRITE setToken)
    Q_PROPERTY(quint8 trackingModes READ trackingModes WRITE setTrackingModes NOTIFY trackingModesChanged)
    Q_PROPERTY(quint16 updateModes READ updateModes WRITE setUpdateModes NOTIFY updateModesChanged)

    Q_PROPERTY(QString debugOutput READ debugOutput WRITE setDebugOutput NOTIFY debugOutputChanged)

public:
    enum TrackingMode {
        TrackingNone                = 0x0,
        TrackingGPS                 = 0x1,
        TrackingWifi                = 0x2
    };
    Q_ENUM(TrackingMode)
    Q_DECLARE_FLAGS(TrackingModes, TrackingMode)

    enum UpdateMode {
        UpdateModeNone              = 0x00,
        UpdateModeSingleEntity      = 0x01,
        UpdateModeEntityModel       = 0x02,
        UpdateModeWebsocket         = 0x04
    };
    Q_ENUM(UpdateMode)
    Q_DECLARE_FLAGS(UpdateModes, UpdateMode)

    explicit ClientInterface(QObject *parent = nullptr);
    ~ClientInterface() override;

    Q_INVOKABLE QString baseUrl() const;
    Q_INVOKABLE void connectToHost();
    Q_INVOKABLE Device *device();
    Q_INVOKABLE EntitiesProvider *entitiesProvider();
    Q_INVOKABLE HomeassistantInfo *homeassistantInfo();
    Q_INVOKABLE void initialize();
    Q_INVOKABLE bool isRegistered();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE void saveZonesSettings();
    Q_INVOKABLE WifiNetworkModel *networksModel();
    Q_INVOKABLE void updateNetworksModel();
    Q_INVOKABLE void updateRegistration();
    Q_INVOKABLE ZonesModel *zonesModel();

    // api
    Q_INVOKABLE void getConfig();
    Q_INVOKABLE void getZones();
    Q_INVOKABLE void registerDevice();

    // properties
    bool apiLogging() const;
    bool busy() const;
    QString hostname() const;
    quint16 port() const;
    bool ready() const;
    bool ssl() const;
    QString token() const;
    quint8 trackingModes() const;
    quint16 updateModes() const;

    QString debugOutput() const;

signals:
    // properties
    void apiLoggingChanged(bool enabled);
    void busyChanged(bool busy);
    void hostnameChanged(const QString &hostname);
    void portChanged(quint16 port);
    void readyChanged(bool ready);
    void sslChanged(bool ssl); 
    void trackingModesChanged(quint8 modes);
    void updateModesChanged(quint16 modes);

    void debugOutputChanged(QString debugOutput);

public slots:
    // properties
    void setApiLogging(bool enable);
    void setBusy(bool busy);
    void setHostname(const QString &hostname);
    void setPort(quint16 port);
    void setReady(bool ready = true);
    void setSsl(bool ssl);
    void setToken(const QString &token);
    void setTrackingModes(quint8 modes);
    void setUpdateModes(quint16 modes);

    void setDebugOutput(const QString &output);

private slots:
    void onDataAvailable(const QString &endpoint, const QJsonDocument &doc);
    void onWebhookDataAvailable(const QString &identifier, const QJsonDocument &doc);

    void onHomeassistantUpdateAvailable(const QString &version);

private:
    void onReadyChanged();
    void updateBaseUrl();

    // settings
    void readSettings();
    void writeSettings();

    HomeassistantApi *m_api{nullptr};
    QString m_baseUrl;
    Device *m_device{nullptr};
    EntitiesProvider *m_entitiesProvider{nullptr};
    DeviceTrackerGPS *m_gpsTracker{nullptr};
    HomeassistantInfo *m_homeassistantInfo{nullptr};
    QString m_homeassistantLastUpdateVersion;
    DeviceTrackerWifi *m_wifiTracker{nullptr};
    Wallet *m_wallet{nullptr};
    WebhookApi *m_webhook{nullptr};
    WebsocketApi *m_websocket{nullptr};
    ZonesModel *m_zones{nullptr};

    // properties
    bool m_apiLogging{false};
    bool m_busy{false};
    QString m_hostname;
    quint16 m_port{8123};
    bool m_ready{false};
    bool m_ssl{false};
    quint8 m_trackingModes{TrackingNone};
    quint16 m_updateModes{UpdateModeNone};

    QString m_debugOutput;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(ClientInterface::TrackingModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(ClientInterface::UpdateModes)

#endif // CLIENTINTERFACE_H
