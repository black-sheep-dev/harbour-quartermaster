#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>

#include "src/api/apiinterface.h"

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(ServiceState state READ state WRITE setState NOTIFY stateChanged)

public:
    enum ServiceError {
        ErrorNone,
        ErrorUndefined,
        ErrorConfiguration,
        ErrorInitializationFailed,
        ErrorCustom                     = 50
    };
    Q_ENUM(ServiceError)

    enum ServiceState {
        StateUndefined,
        StateInitializing,
        StateInitialized,
        StateCustom                     = 50
    };
    Q_ENUM(ServiceState)

    explicit Service(QObject *parent = nullptr);
    ~Service() = default;

    ApiInterface *api();
    void setApi(ApiInterface *api);

    Q_INVOKABLE void saveSettings();

    // properties
    quint8 error() const;
    ServiceState state() const;

signals:
    void apiRequest(quint8 requestType, const QString &query = QString(), const QJsonObject &payload = QJsonObject());
    void webhookRequest(quint8 requestType, const QJsonValue &payload = QJsonValue());

    // properties
    void errorChanged(quint8 error);
    void stateChanged(Service::ServiceState state);

public slots: 
    // properties
    void setError(quint8 error);
    void setState(Service::ServiceState state);

private:
    ApiInterface *m_api{nullptr};
    quint8 m_error{ErrorNone};
    ServiceState m_state{StateUndefined};

    // virtual interface
public:
    virtual void connectToApi();
    virtual void initialize();
    virtual QString errorString() const;
    virtual void readSettings();
    virtual void writeSettings();

public slots:
    virtual void onRequestError(quint8 requestType, quint8 code, const QString &msg);
    virtual void onRequestFinished(quint8 requestType, const QJsonDocument &data);
    virtual void onWebsocketEvent(const QString &event, const QJsonValue &data);


};

#endif // SERVICE_H
