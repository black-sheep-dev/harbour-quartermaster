#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <QObject>

#include <QJsonObject>
#include <QNetworkAccessManager>

class ApiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(bool ssl READ ssl WRITE setSsl NOTIFY sslChanged)

public:
    explicit ApiInterface(QObject *parent = nullptr);

    // api functions
    void request(const QNetworkRequest &request,
                 const QJsonObject &data = QJsonObject(),
                 const QString &identifier = QString());

    // properties
    QString baseUrl() const;
    bool ssl() const;

signals:
    void dataAvailable(const QString &endpoint, const QJsonDocument &data);
    void requestFinished(const QString &identifier, QNetworkReply *reply);

    // properties
    void baseUrlChanged(const QString &url);
    void sslChanged(bool ssl);

public slots:
    // properties
    void setBaseUrl(const QString &url);
    void setSsl(bool ssl);

private slots:
    void onReplyFinished();
    void onSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    QNetworkAccessManager *m_manager;

    // properties
    QString m_baseUrl;
    bool m_ssl;

    // virtual function
public:
    virtual QNetworkRequest getRequest(const QString &endpoint);
    virtual void initialize();

public slots:
    virtual void onRequestFinished(QNetworkReply *reply);
};

#endif // APIINTERFACE_H
