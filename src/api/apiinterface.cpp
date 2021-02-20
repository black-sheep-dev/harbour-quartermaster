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

#include <zlib.h>

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

void ApiInterface::requestEmptyPost(const QNetworkRequest &request)
{
    m_manager->post(request, QByteArray());
}

QByteArray ApiInterface::gunzip(const QByteArray &data)
{
    if (data.size() <= 4) {
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
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

void ApiInterface::logData(const QString &identifier, const QByteArray &data)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
               + "/" + APP_TARGET
               + "/log_"
               + identifier
               + "_" + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMddhhmmsszzz"))
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
    for (const auto &error : errors) {
        qDebug() << error.errorString();
    }
#endif
}

QNetworkRequest ApiInterface::getRequest(const QString &endpoint)
{
    QNetworkRequest request(m_baseUrl + endpoint);
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

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
