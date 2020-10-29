#include "clientinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QSettings>
#include <QStandardPaths>
#include <QThreadPool>
#include <QTimer>

#include <nemonotifications-qt5/notification.h>

#include "src/threads/errorlogparser.h"

ClientInterface::ClientInterface(QObject *parent) :
    QObject(parent),
    m_device(new Device(this)),
    m_entitiesProvider(new EntitiesProvider(this)),
    m_homeassistantInfo(new HomeassistantInfo(this)),
    m_ncm(new QNetworkConfigurationManager(this)),
    m_wallet(new Wallet(this)),
    m_zones(new ZonesModel(this))
{
    m_api = new HomeassistantApi(m_wallet, this);
    m_webhook = new WebhookApi(m_wallet, this);
    m_websocket = new WebsocketApi(m_wallet, this);

    // logging
    connect(this, &ClientInterface::apiLoggingChanged, m_api, &HomeassistantApi::setLogging);
    connect(this, &ClientInterface::apiLoggingChanged, m_webhook, &WebhookApi::setLogging);

    m_entitiesProvider->setApi(m_api);

    connect(m_wallet, &Wallet::initialized, this, &ClientInterface::setReady);

    connect(m_wifiTracker, &DeviceTrackerWifi::networkChanged, this, &ClientInterface::setDebugOutput);
    connect(m_api, &HomeassistantApi::dataAvailable, this, &ClientInterface::onDataAvailable);
    connect(m_webhook, &WebhookApi::dataAvailable, this, &ClientInterface::onWebhookDataAvailable);
    connect(m_device, &Device::sensorUpdated, m_webhook, &WebhookApi::updateSensor);
    connect(m_entitiesProvider, &EntitiesProvider::homeassistantVersionAvailable, this, &ClientInterface::onHomeassistantUpdateAvailable);
    connect(m_websocket, &WebsocketApi::stateChanged, m_entitiesProvider, &EntitiesProvider::updateState);
    connect(m_ncm, &QNetworkConfigurationManager::configurationChanged, this, &ClientInterface::onNetworkConfigurationChanged);

    readSettings();
}

ClientInterface::~ClientInterface()
{   
    m_workerThread.quit();
    m_workerThread.wait(1000);

    writeSettings();
}

QString ClientInterface::baseUrl() const
{
    return m_baseUrl;
}

void ClientInterface::connectToHost()
{
    getConfig();
}

Device *ClientInterface::device()
{
    return m_device;
}

EntitiesProvider *ClientInterface::entitiesProvider()
{
    return m_entitiesProvider;
}

ErrorLogModel *ClientInterface::errorLogModel()
{
    if (!m_errorLogModel) {
        m_errorLogModel = new ErrorLogModel;
    }

    ErrorLogParser *parser = new ErrorLogParser;
    parser->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, parser, &QObject::deleteLater);
    connect(m_api, &HomeassistantApi::errorLogAvailable, parser, &ErrorLogParser::parseData);
    connect(parser, &ErrorLogParser::logParsed, m_errorLogModel, &ErrorLogModel::setLogs);
    m_workerThread.start();

    refreshErrorLog();

    return m_errorLogModel;
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

LogBookModel *ClientInterface::logBookModel(const QDateTime &timestamp)
{
    if (!m_logBookModel)
        m_logBookModel = new LogBookModel;

    m_logBookModel->setLoading(true);

    m_api->getLogBook(timestamp);

    return m_logBookModel;
}

void ClientInterface::reloadConfig()
{
    if (m_wifiTracker) {
        m_wifiTracker->reloadConfig();
    }
}

void ClientInterface::reset()
{
    setHostname(QString());
    setPort(8123);
    setSsl(false);
    setToken(QString());

    m_webhook->reset();
    m_wallet->reset();

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

void ClientInterface::getZones()
{
    m_zones->setLoading(true);
    m_webhook->getZones();
}

void ClientInterface::refreshErrorLog()
{
    if (!m_errorLogModel)
        return;

    m_errorLogModel->setLoading(true);

    m_api->getErrorLog();
}

void ClientInterface::registerDevice()
{
    setBusy(true);
    m_api->registerDevice(m_device);
}

bool ClientInterface::apiLogging() const
{
    return m_apiLogging;
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

quint8 ClientInterface::trackingModes() const
{
    return m_trackingModes;
}

quint16 ClientInterface::updateModes() const
{
    return m_updateModes;
}

bool ClientInterface::websocketNotify() const
{
    return m_websocketNotify;
}

QString ClientInterface::debugOutput() const
{
    return m_debugOutput;
}

void ClientInterface::setApiLogging(bool enable)
{
    if (m_apiLogging == enable)
        return;

    m_apiLogging = enable;
    emit apiLoggingChanged(m_apiLogging);

    //writeSettings();
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

void ClientInterface::setTrackingModes(quint8 modes)
{
    if (m_trackingModes == modes)
        return;

    m_trackingModes = modes;
    emit trackingModesChanged(m_trackingModes);

    // enable / disable gps tracker
    if (m_trackingModes & ClientInterface::TrackingGPS) {
        if (!m_gpsTracker) {
            m_gpsTracker = new DeviceTrackerGPS(this);
            connect(m_gpsTracker, &DeviceTracker::locationUpdated, m_webhook, &WebhookApi::updateLocation);
            m_gpsTracker->updateLocation();
        }
    } else {
        if (m_gpsTracker)
            m_gpsTracker->deleteLater();

        m_gpsTracker = nullptr;
    }

    // enable / disable  wifi tracker
    if (m_trackingModes & ClientInterface::TrackingWifi) {
        if (!m_wifiTracker) {
            m_wifiTracker = new DeviceTrackerWifi(m_zones, this);
            connect(m_wifiTracker, &DeviceTracker::locationUpdated, m_webhook, &WebhookApi::updateLocation);
            m_wifiTracker->updateWifiNetworks();
            m_wifiTracker->updateLocation();
        }
    } else {
        if (m_wifiTracker)
            m_wifiTracker->deleteLater();

        m_wifiTracker = nullptr;
    }
}

void ClientInterface::setUpdateModes(quint16 modes)
{
    if (m_updateModes == modes)
        return;

    m_updateModes = modes;
    emit updateModesChanged(m_updateModes);

    // websocket
    quint8 subscriptions = m_websocket->subscriptions();

    if ((m_updateModes & ClientInterface::UpdateModeWebsocket) == ClientInterface::UpdateModeWebsocket) {
        subscriptions |= WebsocketApi::SubscriptionStateChanged;
    } else {
        subscriptions &= ~WebsocketApi::SubscriptionStateChanged;
    }
    m_websocket->setSubscriptions(subscriptions);
}

void ClientInterface::setWebsocketNotify(bool enable)
{
    if (m_websocketNotify == enable)
        return;

    m_websocketNotify = enable;
    emit websocketNotifyChanged(m_websocketNotify);

    // websocket
    quint8 subscriptions = m_websocket->subscriptions();

    if (m_websocketNotify) {
        subscriptions |= WebsocketApi::SubscriptionNotifyEvents;
    } else {
        subscriptions &= ~WebsocketApi::SubscriptionNotifyEvents;
    }
    m_websocket->setSubscriptions(subscriptions);
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

    if (endpoint == HASS_API_ENDPOINT_CONFIG) {
        m_homeassistantInfo->setData(doc.object());
        m_homeassistantInfo->setAvailable(true);
        m_homeassistantInfo->setLoading(false);
    } else if (endpoint == HASS_API_ENDPOINT_DEVICE_REGISTRATION) {
        m_webhook->setRegistrationData(doc.object());
        m_device->setRegistered(m_webhook->isRegistered());

        for (const DeviceSensor *sensor : m_device->sensors()) {
            m_webhook->registerSensor(sensor);
        }
        setBusy(false);

        // write settings
        writeSettings();

        // load intital data
        getZones();
        entitiesProvider()->refresh();
    } else if (endpoint.startsWith(HASS_API_ENDPOINT_LOGBOOK)) {
        if (!m_logBookModel)
            return;

        m_logBookModel->setLogEntries(doc.array());
    }
}

void ClientInterface::onWebhookDataAvailable(const QString &identifier, const QJsonDocument &doc)
{
    if (identifier== QLatin1String("get_zones")) {
        m_zones->setZones(doc.array());
    }
}

void ClientInterface::onHomeassistantUpdateAvailable(const QString &version)
{ 
    if (!m_homeassistantInfo->isUpdateAvailable(version))
        return;

    if (m_homeassistantLastUpdateVersion == version)
        return;

    m_homeassistantLastUpdateVersion = version;

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("UPDATE AVAILABLE");
#endif

    Notification notify;
    notify.setCategory(QStringLiteral("x-nemo.software-update"));
    notify.setBody(tr("There is an update to version %1 available.").arg(version));
    notify.setSummary(tr("Home Assistant update available!"));
    notify.setIcon(QStringLiteral("image://theme/icon-lock-application-update"));
    notify.publish();
}

void ClientInterface::onNetworkConfigurationChanged(const QNetworkConfiguration &config)
{
    if (config.state() != QNetworkConfiguration::Active) {
        return;
    }

#ifdef QT_DEBUG
    qDebug() << config.name();
    qDebug() << config.identifier();
#endif

    if (m_lastNetworkIdentifier == config.identifier())
        return;

    for (const auto *network : homeassistantInfo()->homezone()->networksModel()->networks()) {
        if (network->identifier() != config.identifier())
            continue;

        m_lastNetworkIdentifier = config.identifier();

        QJsonArray position;
        position.append(m_homeassistantInfo->homezone()->latitude());
        position.append(m_homeassistantInfo->homezone()->longitude());

        m_lastLocation.insert(QStringLiteral("gps"), position);
        m_lastLocation.insert(QStringLiteral("battery"), 100);
        m_lastLocation.insert(QStringLiteral("gps_accuracy"), 10);

        QTimer::singleShot(2000, this, &ClientInterface::updateLocation);
    }
}

void ClientInterface::updateLocation()
{
    m_webhook->updateLocation(m_lastLocation);
}

void ClientInterface::onReadyChanged()
{
    m_api->getConfig();

    if ( m_ready && m_webhook->isRegistered() ) {
        m_device->setRegistered(true);
        m_webhook->updateRegistration(m_device);
        m_webhook->getZones();
    }

    if (m_ready) {
        m_entitiesProvider->refresh();
    }
}

void ClientInterface::updateBaseUrl()
{
    m_baseUrl = QString("%1://%2:%3").arg(m_ssl ? "https" : "http",
                                          m_hostname,
                                          QString::number(m_port));

    m_api->setBaseUrl(m_baseUrl);
    m_webhook->setBaseUrl(m_baseUrl);

    const QString websocketUrl = QString("%1://%2:%3").arg(m_ssl ? "wss" : "ws",
                                                           m_hostname,
                                                           QString::number(m_port));

    m_websocket->setBaseUrl(websocketUrl);
}

void ClientInterface::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    m_homeassistantInfo->setExternalUrl(settings.value(QStringLiteral("external_url")).toString());
    m_homeassistantInfo->setInternalUrl(settings.value(QStringLiteral("internal_url")).toString());
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

    settings.beginGroup(QStringLiteral("APP"));
    setTrackingModes(quint8(settings.value(QStringLiteral("tracking_modes"), ClientInterface::TrackingNone).toInt()));
    setUpdateModes(quint16(settings.value(QStringLiteral("update_modes"), ClientInterface::UpdateModeNone).toInt()));
    setWebsocketNotify(settings.value(QStringLiteral("websocket_notify"), false).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("HOMEZONE"));
    int size = settings.beginReadArray(QStringLiteral("networks"));
    QList<WifiNetwork *> networks;

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        WifiNetwork *network = new WifiNetwork;
        network->setName(settings.value(QStringLiteral("name")).toString());
        network->setIdentifier(settings.value(QStringLiteral("identifier")).toString());

        networks.append(network);
    }

    m_homeassistantInfo->homezone()->networksModel()->setNetworks(networks);
    settings.endArray();
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVELOPER_MODE"));
    setApiLogging(settings.value(QStringLiteral("api_logging"), false).toBool());
    settings.endGroup();

    updateBaseUrl();
}

void ClientInterface::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("API"));
    settings.setValue(QStringLiteral("external_url"), m_homeassistantInfo->externalUrl());
    settings.setValue(QStringLiteral("internal_url"), m_homeassistantInfo->internalUrl());
    settings.setValue(QStringLiteral("hostname"), m_hostname);
    settings.setValue(QStringLiteral("port"), m_port);
    settings.setValue(QStringLiteral("ssl"), m_ssl);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("WEBHOOK_API"));
    settings.setValue(QStringLiteral("encryption"), m_webhook->encryption());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVICE"));
    settings.setValue(QStringLiteral("name"), m_device->name());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("APP"));
    settings.setValue(QStringLiteral("tracking_modes"), m_trackingModes);
    settings.setValue(QStringLiteral("update_modes"), m_updateModes);
    settings.setValue(QStringLiteral("websocket_notify"), m_websocketNotify);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("HOMEZONE"));
    settings.beginWriteArray(QStringLiteral("networks"));
    const QList<WifiNetwork *> networks = m_homeassistantInfo->homezone()->networksModel()->networks();

    for (int i = 0; i < networks.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("name"), networks.at(i)->name());
        settings.setValue(QStringLiteral("identifier"), networks.at(i)->identifier());
    }
    settings.endArray();
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DEVELOPER_MODE"));
    settings.setValue(QStringLiteral("api_logging"), m_apiLogging);
    settings.endGroup();
}
