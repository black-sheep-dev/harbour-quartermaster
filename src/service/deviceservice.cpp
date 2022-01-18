#include "deviceservice.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QSettings>

#include <mdm-sysinfo.h>
#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/deletecollectionrequest.h>
#include <Sailfish/Secrets/deletesecretrequest.h>
#include <Sailfish/Secrets/storesecretrequest.h>
#include <Sailfish/Secrets/storedsecretrequest.h>

#include "src/api/api.h"
#include "src/constants.h"

#include "src/device/sensors/devicesensorbattery.h"
#include "src/device/sensors/devicesensorbatterycharging.h"

#include "src/global.h"

const QString WALLET_COLLECTION_NAME            = QStringLiteral("quartermasternew");
const QString WALLET_COLLECTION_NAME_DEBUG      = QStringLiteral("quartermasternewdebug");

DeviceService::DeviceService(QObject *parent) :
    Service(parent),
    m_secretsIdentifier(Sailfish::Secrets::Secret::Identifier(
                            QStringLiteral("secrets"),
                            #ifdef QT_DEBUG
                                WALLET_COLLECTION_NAME_DEBUG,
                            #else
                                WALLET_COLLECTION_NAME,
                            #endif
                            Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName))
{
    registerSensor(new DeviceSensorBattery);
    registerSensor(new DeviceSensorBatteryCharging);

    readSettings();
}

DeviceService::~DeviceService()
{
    writeSettings();
}

QString DeviceService::id() const
{
    return Sailfish::Mdm::SysInfo::deviceUid();
}

QString DeviceService::manufacturer() const
{
    return Sailfish::Mdm::SysInfo::manufacturer();
}

QString DeviceService::model() const
{
    return Sailfish::Mdm::SysInfo::deviceModel();
}

QString DeviceService::softwareName() const
{
    return QStringLiteral("Sailfish OS");
}

QString DeviceService::softwareVersion() const
{
    return Sailfish::Mdm::SysInfo::softwareVersion();
}

QString DeviceService::wlanMacAddress() const
{
    return Sailfish::Mdm::SysInfo::wlanMacAddress();
}

DeviceSensorModel *DeviceService::sensorsModel()
{
    return m_sensorsModel;
}

bool DeviceService::isRegistered() const
{
    return !m_credentials.webhookId.isEmpty();
}

void DeviceService::registerDevice()
{
    QJsonObject data;
    data.insert(ApiKey::KEY_DEVICE_ID, id());
    data.insert(ApiKey::KEY_APP_ID, QStringLiteral("org.nubecula.harbour.quartermaster"));
    data.insert(ApiKey::KEY_APP_NAME, QCoreApplication::applicationName());
    data.insert(ApiKey::KEY_APP_VERSION, QCoreApplication::applicationVersion());
    data.insert(ApiKey::KEY_DEVICE_NAME, deviceName());
    data.insert(ApiKey::KEY_MANUFACTURER, manufacturer());
    data.insert(ApiKey::KEY_MODEL, model());
    data.insert(ApiKey::KEY_OS_NAME, softwareName());
    data.insert(ApiKey::KEY_OS_VERSION, softwareVersion());
    data.insert(ApiKey::KEY_SUPPORTS_ENCRYPTION, false);

    emit apiRequest(Api::RequestPostApiRegisterDevice, QString(), data);
}

void DeviceService::registerSensors()
{
    for (const auto sensor : m_sensorsModel->sensors()) {
        emit webhookRequest(Api::RequestWebhookRegisterSensor, sensor->toJson());
    }
}

void DeviceService::resetRegistration()
{
    deleteCollection();
}

void DeviceService::updateDeviceRegistration()
{
    QJsonObject data;

    data.insert(ApiKey::KEY_APP_VERSION, QCoreApplication::applicationVersion());
    data.insert(ApiKey::KEY_DEVICE_NAME, deviceName());
    data.insert(ApiKey::KEY_OS_VERSION, softwareVersion());
    data.insert(ApiKey::KEY_MANUFACTURER, manufacturer());
    data.insert(ApiKey::KEY_MODEL, model());

    emit webhookRequest(Api::RequestWebhookUpdateRegistration, data);
}

QString DeviceService::deviceName() const
{
    return m_deviceName;
}

bool DeviceService::sensorLiveUpdates() const
{
    return m_sensorLiveUpdates;
}

QString DeviceService::token() const
{
    return m_credentials.token;
}

void DeviceService::sendSensorUpdate(const QJsonObject &data)
{
    QJsonArray arr;

    arr.append(data);
    emit webhookRequest(Api::RequestWebhookUpdateSensorStates, arr);
}

void DeviceService::sendSensorUpdates()
{
    QJsonArray data;

    for (const auto sensor : m_sensorsModel->sensors()) {
        data.append(sensor->getBaseSensorJson());
    }

    emit webhookRequest(Api::RequestWebhookUpdateSensorStates, data);
}

void DeviceService::setDeviceName(const QString &deviceName)
{
    if (m_deviceName == deviceName)
        return;

    m_deviceName = deviceName;
    emit deviceNameChanged(m_deviceName);
}

void DeviceService::setSensorLiveUpdates(bool sensorLiveUpdates)
{
    if (m_sensorLiveUpdates == sensorLiveUpdates)
        return;

    m_sensorLiveUpdates = sensorLiveUpdates;
    emit sensorLiveUpdatesChanged(m_sensorLiveUpdates);

    // toogle live updates
    for (auto sensor : m_sensorsModel->sensors()) {
        sensor->setEnabled(m_sensorLiveUpdates);

        if (m_sensorLiveUpdates)
            connect(sensor, &DeviceSensor::sensorUpdated, this, &DeviceService::sendSensorUpdate);
        else
            disconnect(sensor, &DeviceSensor::sensorUpdated, this, &DeviceService::sendSensorUpdate);
    }
}

void DeviceService::setToken(const QString &token)
{
    if (m_credentials.token == token)
        return;

    m_credentials.token = token;
    emit tokenChanged(m_credentials.token);
    emit credentialsChanged(m_credentials);
}

void DeviceService::createCollection()
{
#ifdef DISABLE_SAILFISH_SECRETS
    return;
#endif

    Sailfish::Secrets::CreateCollectionRequest createCollection;
    createCollection.setManager(&m_secretManager);
    createCollection.setCollectionLockType(Sailfish::Secrets::CreateCollectionRequest::DeviceLock);
    createCollection.setDeviceLockUnlockSemantic(Sailfish::Secrets::SecretManager::DeviceLockKeepUnlocked);
    //createCollection.setAccessControlMode(Sailfish::Secrets::SecretManager::SystemAccessControlMode);
    createCollection.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    createCollection.setCollectionName(
                                   #ifdef QT_DEBUG
                                       WALLET_COLLECTION_NAME_DEBUG
                                   #else
                                       WALLET_COLLECTION_NAME
                                   #endif
                                       );
    createCollection.setStoragePluginName(Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
    createCollection.setEncryptionPluginName(Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
    createCollection.startRequest();
    createCollection.waitForFinished();

    if (createCollection.result().errorCode()) {
        setError(DeviceError::ErrorCreatingCredentials);
    }

#ifdef QT_DEBUG
    qDebug() << createCollection.result().code();
    qDebug() << createCollection.result().errorMessage();
#endif
}

void DeviceService::deleteCollection()
{
#ifdef DISABLE_SAILFISH_SECRETS
    return;
#endif

    Sailfish::Secrets::DeleteCollectionRequest deleteCollection;
    deleteCollection.setManager(&m_secretManager);
    deleteCollection.setCollectionName(
                    #ifdef QT_DEBUG
                        WALLET_COLLECTION_NAME_DEBUG
                    #else
                        WALLET_COLLECTION_NAME
                    #endif
                );
    deleteCollection.setStoragePluginName(Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
    deleteCollection.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    deleteCollection.startRequest();
    deleteCollection.waitForFinished();

    if (deleteCollection.result().errorCode()) {
        setError(DeviceError::ErrorDeletingCredentials);
    }

#ifdef QT_DEBUG
    qDebug() << deleteCollection.result().code();
    qDebug() << deleteCollection.result().errorMessage();
#endif
}

void DeviceService::deleteSecret()
{
#ifdef QT_DEBUG
    qDebug() << "DELETE SECRET REQUEST";
#endif

    Sailfish::Secrets::DeleteSecretRequest dsr;
    dsr.setManager(&m_secretManager);
    dsr.setIdentifier(m_secretsIdentifier);
    dsr.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    dsr.startRequest();

    dsr.waitForFinished();

#ifdef QT_DEBUG
    qDebug() << dsr.result().code();
    qDebug() << dsr.result().errorMessage();
#endif
}

void DeviceService::loadCredentials()
{
#ifdef DISABLE_SAILFISH_SECRETS
    QSettings settings;
    settings.beginGroup(QStringLiteral("SECRETS"));
    m_credentials.cloudhookUrl = settings.value(QStringLiteral("cloudhook_url")).toString();
    m_credentials.remoteUiUrl = settings.value(QStringLiteral("remote_ui_url")).toString();
    m_credentials.secret = settings.value(QStringLiteral("secret")).toString();
    m_credentials.token = settings.value(QStringLiteral("token")).toString();
    m_credentials.webhookId = settings.value(QStringLiteral("webhook_id")).toString();
    settings.endGroup();

    emit credentialsChanged(m_credentials);
    return;
#endif

    auto fetchCode = new Sailfish::Secrets::StoredSecretRequest;

    fetchCode->setManager(&m_secretManager);
    fetchCode->setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    fetchCode->setIdentifier(m_secretsIdentifier);

    fetchCode->startRequest();
    fetchCode->waitForFinished();

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("CREDENTIALS LOADED");
    qDebug() << fetchCode->result().code();
    qDebug() << fetchCode->result().errorCode();
    qDebug() << fetchCode->result().errorMessage();
#endif

    if (fetchCode->result().code() != Sailfish::Secrets::Result::Succeeded) {
        setError(DeviceError::ErrorLoadingCredentials);
        return;
    }

    QByteArray data = fetchCode->secret().data();


    QDataStream stream(&data, QIODevice::ReadOnly);
    stream >> m_credentials;

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("WEBHOOK ID");
    qDebug() << m_credentials.webhookId;
#endif

    fetchCode->deleteLater();

    emit credentialsChanged(m_credentials);
}

void DeviceService::storeCredentials()
{
#ifdef DISABLE_SAILFISH_SECRETS
    QSettings settings;
    settings.beginGroup(QStringLiteral("SECRETS"));
    settings.setValue(QStringLiteral("cloudhook_url"), m_credentials.cloudhookUrl);
    settings.setValue(QStringLiteral("remote_ui_url"), m_credentials.remoteUiUrl);
    settings.setValue(QStringLiteral("secret"), m_credentials.secret);
    settings.setValue(QStringLiteral("token"), m_credentials.token);
    settings.setValue(QStringLiteral("webhook_id"), m_credentials.webhookId);
    settings.endGroup();
    return;
#endif

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("CREDENTIALS STORE");
#endif

    // reset and create
    deleteCollection();
    createCollection();

    // create data
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_credentials;

    // store data in wallet
    Sailfish::Secrets::Secret secret(m_secretsIdentifier);
    secret.setData(data);

    Sailfish::Secrets::StoreSecretRequest storeCode;
    storeCode.setManager(&m_secretManager);
    storeCode.setSecretStorageType(Sailfish::Secrets::StoreSecretRequest::CollectionSecret);
    storeCode.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    storeCode.setSecret(secret);
    storeCode.startRequest();
    storeCode.waitForFinished();

    if (storeCode.result().errorCode())
        setError(DeviceError::ErrorStoringCredentials);

#ifdef QT_DEBUG
    qDebug() << storeCode.result().code();
    qDebug() << storeCode.result().errorMessage();
#endif
}

void DeviceService::registerSensor(DeviceSensor *sensor)
{
    m_sensorsModel->addSensor(sensor);
}

void DeviceService::parseDeviceRegistration(const QJsonObject &data)
{
    m_credentials.cloudhookUrl = data.value(ApiKey::KEY_CLOUDHOOK_URL).toString();
    m_credentials.remoteUiUrl = data.value(ApiKey::KEY_REMOTE_UI_URL).toString();
    m_credentials.secret = data.value(ApiKey::KEY_SECRET).toString();
    m_credentials.webhookId = data.value(ApiKey::KEY_WEBHOOK_ID).toString();

    storeCredentials();

    emit credentialsChanged(m_credentials);
}

void DeviceService::connectToApi()
{
    connect(this, &DeviceService::credentialsChanged, api(), &ApiInterface::setCredentials);
    connect(this, &DeviceService::apiRequest, api(), &ApiInterface::sendRequest);
    connect(this, &DeviceService::webhookRequest, api(), &ApiInterface::sendWebhookRequest);
    connect(api(), &ApiInterface::requestFinished, this, &DeviceService::onRequestFinished);
    connect(api(), &ApiInterface::requestError, this, &DeviceService::onRequestError);
}

void DeviceService::readSettings()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("READ DEVICE SERVICE SETTINGS");
#endif

    QSettings settings(QFile(DEFAULT_CONFIG_FILE).exists() ? DEFAULT_CONFIG_FILE : DEPRECATED_CONFIG_FILE, QSettings::NativeFormat);

    settings.beginGroup(QStringLiteral("DEVICE"));
    setDeviceName(settings.value(QStringLiteral("name"), Sailfish::Mdm::SysInfo::productName()).toString());
    setSensorLiveUpdates(settings.value(QStringLiteral("sensor_live_updates"), false).toBool());
    settings.endGroup();
}

void DeviceService::writeSettings()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("WRITE DEVICE SERVICE SETTINGS");
#endif

    QSettings settings(DEFAULT_CONFIG_FILE, QSettings::NativeFormat);
    settings.beginGroup(QStringLiteral("DEVICE"));
    settings.setValue(QStringLiteral("name"), m_deviceName);
    settings.setValue(QStringLiteral("sensor_live_updates"), m_sensorLiveUpdates);
    settings.endGroup();
}

void DeviceService::initialize()
{
    setState(ServiceState::StateInitializing);

    loadCredentials();

    setState(ServiceState::StateInitialized);
}

void DeviceService::onRequestError(quint8 requestType, quint8 code, const QString &msg)
{
    Q_UNUSED(requestType)
    Q_UNUSED(code)
    Q_UNUSED(msg)
}

void DeviceService::onRequestFinished(quint8 requestType, const QJsonDocument &data)
{
    switch (requestType) {
    case Api::RequestPostApiRegisterDevice:
        parseDeviceRegistration(data.object());
        registerSensors();
        break;

    default:
        break;
    }
}
