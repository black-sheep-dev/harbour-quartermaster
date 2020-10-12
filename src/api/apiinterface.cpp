#include "apiinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QTextStream>

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


    // logging
    if (logging()) {
        QString name;

        if (identifier.isEmpty())
            name = QStringLiteral("rest_api_request");
        else
            name = QStringLiteral("webhook_request_") + identifier;

        logData(name, QJsonDocument(data).toJson(QJsonDocument::Indented));
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

bool ApiInterface::logging() const
{
    return m_logging;
}

Secrets *ApiInterface::secrets()
{
    return m_secrets;
}

bool ApiInterface::ssl() const
{
    return m_ssl;
}

void ApiInterface::logData(const QString &identifier, const QByteArray &data)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
               + "/" + APP_TARGET
               + "/log_"
               + identifier
               + "_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")
               + ".txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << QString(data);

    file.close();
}

void ApiInterface::setBaseUrl(const QString &url)
{
    if (m_baseUrl == url)
        return;

    m_baseUrl = url;
    emit baseUrlChanged(m_baseUrl);
}

void ApiInterface::setLogging(bool logging)
{
    if (m_logging == logging)
        return;

    m_logging = logging;
    emit loggingChanged(m_logging);

    if (!m_logging)
        return;

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + APP_TARGET);

    if (!dir.exists())
        dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + APP_TARGET);
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
