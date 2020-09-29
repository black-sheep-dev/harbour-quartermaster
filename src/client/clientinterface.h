#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QObject>

#include "src/api/homeassistantapi.h"
#include "src/api/webhookapi.h"

#include "homeassistantinfo.h"
#include "src/device/device.h"

#include "src/device/trackers/devicetrackergps.h"
#include "src/device/trackers/devicetrackerwifi.h"

class ClientInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool ssl READ ssl WRITE setSsl NOTIFY sslChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(bool trackingGPS READ trackingGPS WRITE setTrackingGPS NOTIFY trackingGPSChanged)
    Q_PROPERTY(bool trackingWifi READ trackingWifi WRITE setTrackingWifi NOTIFY trackingWifiChanged)

    Q_PROPERTY(QString debugOutput READ debugOutput WRITE setDebugOutput NOTIFY debugOutputChanged)

public:
    explicit ClientInterface(QObject *parent = nullptr);
    ~ClientInterface();

    Q_INVOKABLE void connectToHost();
    Q_INVOKABLE Device *device();
    Q_INVOKABLE HomeassistantInfo *homeassistantInfo();
    Q_INVOKABLE void initialize();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE ZonesModel *zonesModel();

    // api
    Q_INVOKABLE void getConfig();
    Q_INVOKABLE void getZones();
    Q_INVOKABLE void registerDevice();

    // properties
    bool busy() const;
    QString hostname() const;
    quint16 port() const;
    bool ssl() const;
    QString token() const;
    bool trackingGPS() const;
    bool trackingWifi() const;

    QString debugOutput() const;

signals:
    // properties
    void busyChanged(bool busy);
    void hostnameChanged(const QString &hostname);
    void portChanged(quint16 port);
    void sslChanged(bool ssl);
    void tokenChanged(const QString &token);  
    void trackingGPSChanged(bool enabled);
    void trackingWifiChanged(bool enabled);

    void debugOutputChanged(QString debugOutput);

public slots:
    // properties
    void setBusy(bool busy);
    void setHostname(const QString &hostname);
    void setPort(quint16 port);
    void setSsl(bool ssl);
    void setToken(const QString &token);
    void setTrackingGPS(bool enable);
    void setTrackingWifi(bool enable);

    void setDebugOutput(QString debugOutput);

private slots:
    void onDataAvailable(const QString &endpoint, const QJsonDocument &doc);
    void onWebhookDataAvailable(const QString &identifier, const QJsonDocument &doc);

private:
    void updateBaseUrl();

    // settings
    void readSettings();
    void writeSettings();

    DeviceTrackerGPS *m_gpsTracker;
    DeviceTrackerWifi *m_wifiTracker;

    ZonesModel *m_zones;

    HomeassistantApi *m_api;
    WebhookApi *m_webhook;

    Device *m_device;
    HomeassistantInfo *m_homeassistantInfo;

    QString m_baseUrl;

    // properties
    bool m_busy;
    QString m_hostname;
    quint16 m_port;
    bool m_ssl;
    bool m_trackingGPS;
    bool m_trackingWifi;

    QString m_debugOutput;
};

#endif // CLIENTINTERFACE_H
