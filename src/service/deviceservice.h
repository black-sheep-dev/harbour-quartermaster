#ifndef DEVICESERVICE_H
#define DEVICESERVICE_H

#include "service.h"

#include <Sailfish/Secrets/secretmanager.h>
#include <Sailfish/Secrets/result.h>

#include "src/device/devicesensormodel.h"
#include "src/api/credentials.h"

class DeviceService : public Service
{
    Q_OBJECT

    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(bool sensorLiveUpdates READ sensorLiveUpdates WRITE setSensorLiveUpdates NOTIFY sensorLiveUpdatesChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

public:
    enum DeviceError {
        ErrorRegisterDevice         = ServiceError::ErrorCustom + 1,
        ErrorUpdateRegistration,
        ErrorRegisterSensor,
        ErrorUpdateSensor,

        ErrorLoadingCredentials,
        ErrorStoringCredentials,
        ErrorCreatingCredentials,
        ErrorDeletingCredentials
    };
    Q_ENUM(DeviceError)

    explicit DeviceService(QObject *parent = nullptr);
    ~DeviceService();

    // device informations
    Q_INVOKABLE QString id() const;
    Q_INVOKABLE QString manufacturer() const;
    Q_INVOKABLE QString model() const;
    Q_INVOKABLE QString softwareName() const;
    Q_INVOKABLE QString softwareVersion() const;
    Q_INVOKABLE QString wlanMacAddress() const;

    // members
    Q_INVOKABLE DeviceSensorModel *sensorsModel();

    // functions
    Q_INVOKABLE bool isRegistered() const;
    Q_INVOKABLE void registerDevice();
    Q_INVOKABLE void registerSensors();
    Q_INVOKABLE void resetRegistration();
    Q_INVOKABLE void updateDeviceRegistration();

    // properties
    QString deviceName() const;
    bool sensorLiveUpdates() const;
    QString token() const;

signals:
    void credentialsChanged(const Credentials &credentials);
    void deviceRegistered(bool success);

    // properties
    void deviceNameChanged(const QString &deviceName);
    void sensorLiveUpdatesChanged(bool sensorLiveUpdates);
    void tokenChanged(const QString &token);

public slots:
    void sendSensorUpdate(const QJsonObject &data);
    void sendSensorUpdates();

     // properties
    void setDeviceName(const QString &deviceName);
    void setSensorLiveUpdates(bool sensorLiveUpdates);
    void setToken(const QString &token);

private:
    // sailfish secrets
    void createCollection();
    void deleteCollection();
    void deleteSecret();
    void loadCredentials();
    void storeCredentials();

    // sensors
    void registerSensor(DeviceSensor *sensor);

    // helper functions
    void parseDeviceRegistration(const QJsonObject &data);

    Credentials m_credentials;
    Sailfish::Secrets::SecretManager m_secretManager;
    Sailfish::Secrets::Secret::Identifier m_secretsIdentifier;

    DeviceSensorModel *m_sensorsModel{new DeviceSensorModel(this)};

    // properties
    QString m_deviceName;
    bool m_sensorLiveUpdates{false};

    // Service interface
public:
    void connectToApi() override;
    void initialize() override;
    void readSettings() override;
    void writeSettings() override;

public slots:
    void onRequestError(quint8 requestType, quint8 code, const QString &msg) override;
    void onRequestFinished(quint8 requestType, const QJsonDocument &data) override;

    // Service interface
public:

};

#endif // DEVICESERVICE_H
