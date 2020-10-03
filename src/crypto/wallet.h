#ifndef WALLET_H
#define WALLET_H

#include <QObject>

#include <Sailfish/Secrets/secretmanager.h>

#include "secrets.h"

class Wallet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString cloudhookUrl READ cloudhookUrl WRITE setCloudhookUrl NOTIFY cloudhookUrlChanged)
    Q_PROPERTY(QString remoteUiUrl READ remoteUiUrl WRITE setRemoteUiUrl NOTIFY remoteUiUrlChanged)
    Q_PROPERTY(QString secret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(QString webhookId READ webhookId WRITE setWebhookId NOTIFY webhookIdChanged)

public:
    explicit Wallet(QObject *parent = nullptr);

    void initialize();

    // properties
    QString cloudhookUrl() const;
    QString remoteUiUrl() const;
    QString secret() const;
    QString token() const;
    QString webhookId() const;

signals:
    void initialized(bool);

    // properties
    void cloudhookUrlChanged(const QString &url);
    void remoteUiUrlChanged(const QString &url);
    void secretChanged(const QString &secret);
    void tokenChanged(const QString &token);
    void webhookIdChanged(const QString &id);

public slots:
    void reset();
    void storeSecrets();

    // properties
    void setCloudhookUrl(const QString &url);
    void setRemoteUiUrl(const QString &url);
    void setSecret(const QString &secret);
    void setToken(const QString &token);
    void setWebhookId(const QString &id);

private slots:
    void onSecretsLoaded();

private:
    void createCollection();
    void deleteCollection();
    void getSecrets();

    Sailfish::Secrets::SecretManager m_secretManager;
    Sailfish::Secrets::Secret::Identifier m_secretsIdentifier;

    // properties
    Secrets m_secrets;
};

#endif // WALLET_H
