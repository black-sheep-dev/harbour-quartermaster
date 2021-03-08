#ifndef WALLET_H
#define WALLET_H

#include <QObject>

#include <Sailfish/Secrets/secretmanager.h>
#include <Sailfish/Secrets/result.h>

#include "src/api/credentials.h"

class Wallet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString cloudhookUrl READ cloudhookUrl WRITE setCloudhookUrl NOTIFY cloudhookUrlChanged)
    Q_PROPERTY(Credentials credentials READ credentials WRITE setCredentials NOTIFY credentialsChanged)
    Q_PROPERTY(QString remoteUiUrl READ remoteUiUrl WRITE setRemoteUiUrl NOTIFY remoteUiUrlChanged)
    Q_PROPERTY(QString secret READ secret WRITE setSecret NOTIFY secretChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

public:    
    explicit Wallet(QObject *parent = nullptr);

    void initialize();
    bool isValid() const;
    Sailfish::Secrets::Result::ErrorCode lastError() const;

    // properties 
    QString cloudhookUrl() const;
    Credentials credentials() const; 
    QString remoteUiUrl() const;
    QString secret() const;
    QString token() const;

signals:
    void validityChanged(bool valid);

    // properties
    void cloudhookUrlChanged(const QString &url);
    void credentialsChanged(const Credentials &credentials);
    void remoteUiUrlChanged(const QString &url);
    void secretChanged(const QString &secret);
    void tokenChanged(const QString &token);

public slots:
    void reset();
    void storeCredentials();

    // properties 
    void setCloudhookUrl(const QString &url);
    void setCredentials(const Credentials &credentials);
    void setRemoteUiUrl(const QString &url);
    void setSecret(const QString &secret);
    void setToken(const QString &token);

private:
    void createCollection();
    void deleteCollection();
    void getCredentials();

    Sailfish::Secrets::Result::ErrorCode m_lastError{Sailfish::Secrets::Result::NoError};
    Sailfish::Secrets::SecretManager m_secretManager;
    Sailfish::Secrets::Secret::Identifier m_secretsIdentifier;

    // properties
    Credentials m_credentials;
};

#endif // WALLET_H
