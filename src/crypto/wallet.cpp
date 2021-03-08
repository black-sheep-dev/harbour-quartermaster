#include "wallet.h"

const QString WALLET_COLLECTION_NAME            = QStringLiteral("quartermaster");
const QString WALLET_COLLECTION_NAME_DEBUG      = QStringLiteral("quartermasterdebug");

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <Sailfish/Secrets/createcollectionrequest.h>
#include <Sailfish/Secrets/deletecollectionrequest.h>
#include <Sailfish/Secrets/storesecretrequest.h>
#include <Sailfish/Secrets/storedsecretrequest.h>


Wallet::Wallet(QObject *parent) :
    QObject(parent),
    m_secretsIdentifier(Sailfish::Secrets::Secret::Identifier(
                            QStringLiteral("secrets"),
                            #ifdef QT_DEBUG
                                WALLET_COLLECTION_NAME_DEBUG,
                            #else
                                WALLET_COLLECTION_NAME,
                            #endif
                            Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName))
{

}

void Wallet::initialize()
{
    getCredentials();
}

bool Wallet::isValid() const
{
    return m_credentials.isValid();
}

Sailfish::Secrets::Result::ErrorCode Wallet::lastError() const
{
    return m_lastError;
}

QString Wallet::cloudhookUrl() const
{
    return m_credentials.cloudhookUrl;
}

Credentials Wallet::credentials() const
{
    return m_credentials;
}

QString Wallet::remoteUiUrl() const
{
    return m_credentials.remoteUiUrl;
}

QString Wallet::secret() const
{
    return m_credentials.secret;
}

QString Wallet::token() const
{
    return m_credentials.token;
}

void Wallet::reset()
{
    deleteCollection();
}

void Wallet::setCloudhookUrl(const QString &url)
{
    if (m_credentials.cloudhookUrl == url)
        return;

    m_credentials.cloudhookUrl = url;
    emit cloudhookUrlChanged(m_credentials.cloudhookUrl);
    emit credentialsChanged(m_credentials);
}

void Wallet::setCredentials(const Credentials &credentials)
{
    if (m_credentials == credentials)
        return;

    m_credentials = credentials;
    emit credentialsChanged(m_credentials);

    storeCredentials();
}

void Wallet::setRemoteUiUrl(const QString &url)
{
    if (m_credentials.remoteUiUrl == url)
        return;

    m_credentials.remoteUiUrl = url;
    emit remoteUiUrlChanged(m_credentials.remoteUiUrl);
    emit credentialsChanged(m_credentials);
}

void Wallet::setSecret(const QString &secret)
{
    if (m_credentials.secret == secret)
        return;

    m_credentials.secret = secret;
    emit secretChanged(m_credentials.secret);
    emit credentialsChanged(m_credentials);
}

void Wallet::setToken(const QString &token)
{
    if (m_credentials.token == token)
        return;

    m_credentials.token = token;
    emit credentialsChanged(m_credentials);
    emit tokenChanged(m_credentials.token);
}

void Wallet::storeCredentials()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("CREDENTIALS STORE");
#endif

    // reset and create
    deleteCollection();
    createCollection();

    // create data
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_credentials;

    // store data in wallet
    Sailfish::Secrets::Secret secret(m_secretsIdentifier);
    secret.setData(data);

    Sailfish::Secrets::StoreSecretRequest storeCode;
    storeCode.setManager(&m_secretManager);
    storeCode.setSecretStorageType(Sailfish::Secrets::StoreSecretRequest::CollectionSecret);
    storeCode.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    storeCode.setSecret(secret);
    storeCode.startRequest();
    storeCode.waitForFinished();

    m_lastError = storeCode.result().errorCode();

    emit validityChanged(isValid());

#ifdef QT_DEBUG
    qDebug() << storeCode.result().code();
    qDebug() << storeCode.result().errorMessage();
#endif
}

void Wallet::createCollection()
{
    Sailfish::Secrets::CreateCollectionRequest createCollection;
    createCollection.setManager(&m_secretManager);
    createCollection.setCollectionLockType(Sailfish::Secrets::CreateCollectionRequest::DeviceLock);
    createCollection.setDeviceLockUnlockSemantic(Sailfish::Secrets::SecretManager::DeviceLockKeepUnlocked);
    createCollection.setAccessControlMode(Sailfish::Secrets::SecretManager::OwnerOnlyMode);
    createCollection.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    createCollection.setCollectionName(
                                   #ifdef QT_DEBUG
                                       WALLET_COLLECTION_NAME_DEBUG
                                   #else
                                       WALLET_COLLECTION_NAME
                                   #endif
                                       );
    createCollection.setStoragePluginName(Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
    createCollection.setEncryptionPluginName(Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
    createCollection.startRequest();
    createCollection.waitForFinished();

    m_lastError = createCollection.result().errorCode();

#ifdef QT_DEBUG
    qDebug() << createCollection.result().code();
    qDebug() << createCollection.result().errorMessage();
#endif
}

void Wallet::deleteCollection()
{
    Sailfish::Secrets::DeleteCollectionRequest createCollection;
    createCollection.setManager(&m_secretManager);
    createCollection.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    createCollection.setCollectionName(
                    #ifdef QT_DEBUG
                        WALLET_COLLECTION_NAME_DEBUG
                    #else
                        WALLET_COLLECTION_NAME
                    #endif
                );
    createCollection.setStoragePluginName(Sailfish::Secrets::SecretManager::DefaultEncryptedStoragePluginName);
    createCollection.startRequest();
    createCollection.waitForFinished();

    m_lastError = createCollection.result().errorCode();

    if (!m_lastError) {
        m_credentials = Credentials();
    }

    emit validityChanged(isValid());

#ifdef QT_DEBUG
    qDebug() << createCollection.result().code();
    qDebug() << createCollection.result().errorMessage();
#endif
}

void Wallet::getCredentials()
{
    auto fetchCode = new Sailfish::Secrets::StoredSecretRequest;

    fetchCode->setManager(&m_secretManager);
    fetchCode->setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    fetchCode->setIdentifier(m_secretsIdentifier);

    fetchCode->startRequest();
    fetchCode->waitForFinished();

    m_lastError = fetchCode->result().errorCode();

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("SECRETS LOADED");
    qDebug() << fetchCode->result().code();
    qDebug() << fetchCode->result().errorCode();
    qDebug() << fetchCode->result().errorMessage();
#endif

    if (fetchCode->result().code() != Sailfish::Secrets::Result::Succeeded) {
        return;
    }

    QByteArray data = fetchCode->secret().data();

    QDataStream stream(&data, QIODevice::ReadOnly);
    stream >> m_credentials;

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("WEBHOOK ID");
    qDebug() << m_credentials.webhookId;
#endif

    fetchCode->deleteLater();

    emit credentialsChanged(m_credentials);
    emit validityChanged(isValid());
}
