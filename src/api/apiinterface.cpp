#include "apiinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QJsonDocument>

#include <QNetworkReply>

ApiInterface::ApiInterface(Wallet *wallet, QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this)),
    m_wallet(wallet),
    m_baseUrl(QString())
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &ApiInterface::onRequestFinished);
    connect(m_manager, &QNetworkAccessManager::sslErrors, this ,&ApiInterface::onSslErrors);
}

Wallet *ApiInterface::wallet()
{
    return m_wallet;
}

void ApiInterface::request(const QNetworkRequest &request, const QJsonObject &data, const QString &identifier)
{
    // GET
    if (data.isEmpty()) {
        m_manager->get(request);
        return;
    }

    // POST
    QNetworkReply *reply = m_manager->post(request, QJsonDocument(data).toJson(QJsonDocument::Compact));

    if (!identifier.isEmpty()) {
        connect(reply, &QNetworkReply::finished, this, &ApiInterface::onReplyFinished);
        reply->setProperty("identifier", identifier);
    }
}

QString ApiInterface::baseUrl() const
{
    return m_baseUrl;
}

Secrets *ApiInterface::secrets()
{
    return m_secrets;
}

bool ApiInterface::ssl() const
{
    return m_ssl;
}

void ApiInterface::setBaseUrl(const QString &url)
{
    if (m_baseUrl == url)
        return;

    m_baseUrl = url;
    emit baseUrlChanged(m_baseUrl);
}

void ApiInterface::setSecrets(Secrets *secrets)
{
    if (m_secrets == secrets)
        return;

    m_secrets = secrets;
    emit secretsChanged(m_secrets);
}

void ApiInterface::setSsl(bool ssl)
{
    if (m_ssl == ssl)
        return;

    m_ssl = ssl;
    emit sslChanged(m_ssl);
}

void ApiInterface::onReplyFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());

    if (!reply)
        return;

    const QString identifier = reply->property("identifier").toString();

    if (identifier.isEmpty())
        return;

    emit requestFinished(identifier, reply);
}

void ApiInterface::onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->deleteLater();

#ifdef QT_DEBUG
    for (const QSslError &error : errors) {
        qDebug() << error.errorString();
    }
#endif
}

QNetworkRequest ApiInterface::getRequest(const QString &endpoint)
{
    QNetworkRequest request(m_baseUrl + endpoint);

    if (m_ssl) {
        QSslConfiguration sslConfig(QSslConfiguration::defaultConfiguration());
        request.setSslConfiguration(sslConfig);
    }

    request.setRawHeader("Content-Type", "application/json");

    return request;
}

void ApiInterface::initialize()
{

}

void ApiInterface::onRequestFinished(QNetworkReply *reply)
{
    reply->deleteLater();
}
