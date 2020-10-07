#include "clientinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

ClientInterface::ClientInterface(QObject *parent) :
    QObject(parent),
    m_zones(new ZonesModel(this)),
    m_wallet(new Wallet(this)),
    m_device(new Device(this)),
    m_homeassistantInfo(new HomeassistantInfo(this))
{
    m_api = new HomeassistantApi(m_wallet, this);
    m_webhook = new WebhookApi(m_wallet, this);

    connect(m_wallet, &Wallet::initialized, this, &ClientInterface::setReady);

    connect(m_wifiTracker, &DeviceTrackerWifi::networkChanged, this, &ClientInterface::setDebugOutput);
    connect(m_api, &HomeassistantApi::dataAvailable, this, &ClientInterface::onDataAvailable);
    connect(m_webhook, &WebhookApi::dataAvailable, this, &ClientInterface::onWebhookDataAvailable);
    connect(m_device, &Device::sensorUpdated, m_webhook, &WebhookApi::updateSensor);

    readSettings();
}

ClientInterface::~ClientInterface()
{
    writeSettings();
}

void ClientInterface::connectToHost()
{
    getConfig();
}

Device *ClientInterface::device()
{
    return m_device;
}

HomeassistantInfo *ClientInterface::homeassistantInfo()
{
    return m_homeassistantInfo;
}

void ClientInterface::initialize()
{
    m_wallet->initialize(); 
}

bool ClientInterface::isRegistered()
{
    return m_webhook->isRegistered();
}

void ClientInterface::reset()
{
    setHostname(QString());
    setPort(8123);
    setSsl(false);
    setToken(QString());

    m_webhook->reset();

    writeSettings();
}

void ClientInterface::saveSettings()
{
    writeSettings();
}

void ClientInterface::saveZonesSettings()
{
    if (!m_wifiTracker)
        return;

    m_wifiTracker->saveNetworkSettings();
}

WifiNetworkModel *ClientInterface::networksModel()
{
    if (!m_wifiTracker)
        return nullptr;

    return m_wifiTracker->localNetworkModel();
}

void ClientInterface::updateNetworksModel()
{
    if (!m_wifiTracker)
        return;

    m_wifiTracker->updateWifiNetworks();
}

void ClientInterface::updateRegistration()
{
    m_webhook->updateRegistration(m_device);
}

ZonesModel *ClientInterface::zonesModel()
{
    return m_zones;
}

void ClientInterface::getConfig()
{
    m_homeassistantInfo->setLoading(true);
    m_api->getConfig();
}

void ClientInterface::getStates()
{
    m_api->getStates();
}

void ClientInterface::getZones()
{
    m_zones->setLoading(true);
    m_webhook->getZones();
}

void ClientInterface::registerDevice()
{
    setBusy(true);
    m_api->registerDevice(m_device);
}

bool ClientInterface::busy() const
{
    return m_busy;
}

QString ClientInterface::hostname() const
{
    return m_hostname;
}

quint16 ClientInterface::port() const
{
    return m_port;
}

bool ClientInterface::ready() const
{
    return m_ready;
}

bool ClientInterface::ssl() const
{
    return m_ssl;
}

QString ClientInterface::token() const
{
    return m_wallet->token();
}

bool ClientInterface::trackingGPS() const
{
    return m_trackingGPS;
}

bool ClientInterface::trackingWifi() const
{
    return m_trackingWifi;
}

QString ClientInterface::debugOutput() const
{
    return m_debugOutput;
}

void ClientInterface::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged(m_busy);
}

void ClientInterface::setHostname(const QString &hostname)
{
    if (m_hostname == hostname)
        return;

    // clean up hostname
    if (hostname.startsWith("http://")) {
        setSsl(false);
        m_hostname = hostname.mid(7, hostname.length());
    } else if (hostname.startsWith("https://")) {
        setSsl(true);
        m_hostname = hostname.mid(8, hostname.length());
    } else {
        m_hostname = hostname;
    }
    emit hostnameChanged(m_hostname);

    updateBaseUrl();
}

void ClientInterface::setPort(quint16 port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(m_port);

    updateBaseUrl();
}

void ClientInterface::setReady(bool ready)
{
    if (m_ready == ready)
        return;

    m_ready = ready;
    emit readyChanged(m_ready);

    onReadyChanged();
}

void ClientInterface::setSsl(bool ssl)
{
    if (m_ssl == ssl)
        return;

    m_ssl = ssl;
    emit sslChanged(m_ssl);

    updateBaseUrl();
}

void ClientInterface::setToken(const QString &token)
{
    m_wallet->setToken(token);
    m_wallet->storeSecrets();
}

void ClientInterface::setTrackingGPS(bool enable)
{
    if (m_trackingGPS == enable)
        return;

    m_trackingGPS = enable;
    emit trackingGPSChanged(m_trackingGPS);

    // enable / disable tracker
    if (m_gpsTracker)
        m_gpsTracker->deleteLater();

    m_gpsTracker = nullptr;

    writeSettings();

    if (enable) {
        m_gpsTracker = new DeviceTrackerGPS(this);
        connect(m_gpsTracker, &DeviceTracker::locationUpdated, m_webhook, &WebhookApi::updateLocation);
        m_gpsTracker->updateLocation();
    }
}

void ClientInterface::setTrackingWifi(bool enable)
{
    if (m_trackingWifi == enable)
        return;

    m_trackingWifi = enable;
    emit trackingWifiChanged(m_trackingWifi);

    // enable / disable tracker
    if (m_wifiTracker)
        m_wifiTracker->deleteLater();

    m_wifiTracker = nullptr;

    writeSettings();

    if (enable) {
        m_wifiTracker = new DeviceTrackerWifi(m_zones, this);
        connect(m_wifiTracker, &DeviceTracker::locationUpdated, m_webhook, &WebhookApi::updateLocation);
        m_wifiTracker->updateWifiNetworks();
        m_wifiTracker->updateLocation();
    }
}

void ClientInterface::setDebugOutput(const QString &output)
{
    if (m_debugOutput == output)
        return;

    m_debugOutput = output;
    emit debugOutputChanged(m_debugOutput);
}

void ClientInterface::onDataAvailable(const QString &endpoint, const QJsonDocument &doc)
{
#ifdef QT_DEBUG
    qDebug() << endpoint;
#endif

    if (endpoint == QStringLiteral(HASS_API_ENDPOINT_CONFIG)) {
        m_homeassistantInfo->setData(doc.object());
        m_homeassistantInfo->setAvailable(true);
        m_homeassistantInfo->setLoading(false);
    } else if (endpoint == QStringLiteral(HASS_API_ENDPOINT_STATES)) {

    } else if (endpoint == QStringLiteral(HASS_API_ENDPOINT_DEVICE_REGISTRATION)) {
        m_webhook->setRegistrationData(doc.object());
        m_device->setRegistered(m_webhook->isRegistered());

        for (const DeviceSensor *sensor : m_device->sensors()) {
            m_webhook->registerSensor(sensor);
        }
        setBusy(false);

        getZones();

        writeSettings();
    }
}

void ClientInterface::onWebhookDataAvailable(const QString &identifier, const QJsonDocument &doc)
{
    if (identifier == QStringLiteral("get_zones")) {
        m_zones->setZones(doc.array());
    }
}

void ClientInterface::onReadyChanged()
{
    if ( m_ready && m_webhook->isRegistered() ) {
        m_device->setRegistered(true);
        m_webhook->updateRegistration(m_device);
        //m_webhook->getZones();
    }

    if (m_ready) {
        m_api->getStates();
    }
}

void ClientInterface::updateBaseUrl()
{
    m_baseUrl = QString("%1://%2:%3").arg(m_ssl ? "https" : "http",
                                          m_hostname,
                                          QString::number(m_port));

    m_api->setBaseUrl(m_baseUrl);
    m_api->setSsl(m_ssl);

    m_webhook->setBaseUrl(m_baseUrl);
    m_webhook->setSsl(m_ssl);
}

void ClientInterface::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    m_hostname = settings.value(QStringLiteral("hostname"), QString()).toString();
    m_port = quint16(settings.value(QStringLiteral("port"), 8123).toInt());
    m_ssl = settings.value(QStringLiteral("ssl"), false).toBool();
    settings.endGroup();

    settings.beginGroup(QStringLiteral("WEBHOOK_API"));
    m_webhook->setEncryption(settings.value(QStringLiteral("encryption"), false).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVICE"));
    const QString name = settings.value(QStringLiteral("name"), QString()).toString();

    if (!name.isEmpty())
        m_device->setName(name);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("TRACKING"));
    setTrackingGPS(settings.value(QStringLiteral("gps"), false).toBool());
    setTrackingWifi(settings.value(QStringLiteral("wifi"), false).toBool());
    settings.endGroup();

    updateBaseUrl();
}

void ClientInterface::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    settings.setValue(QStringLiteral("hostname"), m_hostname);
    settings.setValue(QStringLiteral("port"), m_port);
    settings.setValue(QStringLiteral("ssl"), m_ssl);
    //settings.setValue(QStringLiteral("token"), m_api->token());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("WEBHOOK_API"));
    settings.setValue(QStringLiteral("encryption"), m_webhook->encryption());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVICE"));
    settings.setValue(QStringLiteral("name"), m_device->name());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("TRACKING"));
    settings.setValue(QStringLiteral("gps"), m_trackingGPS);
    settings.setValue(QStringLiteral("wifi"), m_trackingWifi);
    settings.endGroup();
}
