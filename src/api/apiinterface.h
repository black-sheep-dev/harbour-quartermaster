#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <QObject>

#include <QJsonObject>
#include <QNetworkAccessManager>

#include "src/crypto/wallet.h"

class ApiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(bool logging READ logging WRITE setLogging NOTIFY loggingChanged)
    Q_PROPERTY(Secrets *secrets READ secrets WRITE setSecrets NOTIFY secretsChanged)

public:
    explicit ApiInterface(Wallet *wallet, QObject *parent = nullptr);

    Wallet *wallet();

    // api functions
    void request(const QNetworkRequest &request,
                 const QJsonObject &data = QJsonObject(),
                 const QString &identifier = QString());

    void requestEmptyPost(const QNetworkRequest &request);

    // helper functions
    QByteArray gunzip(const QByteArray &data);

    // properties
    QString baseUrl() const;
    bool logging() const;
    Secrets *secrets();

signals:
    void dataAvailable(const QString &endpoint, const QJsonDocument &data);
    void requestFinished(const QString &identifier, QNetworkReply *reply);

    // properties
    void baseUrlChanged(const QString &url);
    void loggingChanged(bool logging);
    void secretsChanged(Secrets *secrets);

public slots:
    void logData(const QString &identifier, const QByteArray &data);

    // properties
    void setBaseUrl(const QString &url);
    void setLogging(bool logging);
    void setSecrets(Secrets *secrets);

private slots:
    void onReplyFinished();
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    QNetworkAccessManager *m_manager{nullptr};
    Wallet *m_wallet{nullptr};

    // properties
    QString m_baseUrl;
    bool m_logging{false};
    Secrets *m_secrets{new Secrets()};

    // virtual function
public:
    virtual QNetworkRequest getRequest(const QString &endpoint);
    virtual void initialize();

public slots:
    virtual void onRequestFinished(QNetworkReply *reply);
};

#endif // APIINTERFACE_H
