#include "service.h"

Service::Service(QObject *parent) :
    QObject(parent)
{

}

Service::~Service()
{

}

ApiInterface *Service::api()
{
    return m_api;
}

void Service::setApi(ApiInterface *api)
{
    m_api = api;

    if (m_api != nullptr)
        connectToApi();
}

void Service::saveSettings()
{
    writeSettings();
}

quint8 Service::error() const
{
    return m_error;
}

Service::ServiceState Service::state() const
{
    return m_state;
}

void Service::setError(quint8 error)
{
    if (m_error == error)
        return;

    m_error = error;
    emit errorChanged(m_error);
}

void Service::setState(Service::ServiceState state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
}

void Service::connectToApi()
{

}

void Service::initialize()
{
    setState(ServiceState::StateInitialized);
}

QString Service::errorString() const
{
    switch (error()) {
    case Service::ErrorNone:
        return tr("No error");

    case Service::ErrorUndefined:
        return tr("An undefined error occured");

    case Service::ErrorConfiguration:
        return tr("Error in service configuration");

    case Service::ErrorInitializationFailed:
        return tr("Initialization failed");

    default:
        return QString();
    }
}

void Service::readSettings()
{

}

void Service::writeSettings()
{

}

void Service::onRequestError(quint8 requestType, quint8 code, const QString &msg)
{
    Q_UNUSED(requestType)
    Q_UNUSED(code)
    Q_UNUSED(msg)
}

void Service::onRequestFinished(quint8 requestType, const QJsonDocument &data)
{
    Q_UNUSED(requestType)
    Q_UNUSED(data)
}

void Service::onWebsocketEvent(const QString &event, const QJsonValue &data)
{
    Q_UNUSED(event)
    Q_UNUSED(data)
}
