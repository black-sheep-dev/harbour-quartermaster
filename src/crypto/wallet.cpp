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

    getSecrets();
}

QString Wallet::cloudhookUrl() const
{
    return m_secrets.cloudhookUrl;
}

QString Wallet::remoteUiUrl() const
{
    return m_secrets.remoteUiUrl;
}

QString Wallet::secret() const
{
    return m_secrets.secret;
}

QString Wallet::token() const
{
    return m_secrets.token;
}

QString Wallet::webhookId() const
{
    return m_secrets.webhookId;
}

void Wallet::reset()
{
    deleteCollection();
}

void Wallet::storeSecrets()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("SECRETS STORE");
#endif

    // reset and create
    deleteCollection();
    createCollection();

    // create data
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << m_secrets;

    // store data in wallet
    Sailfish::Secrets::Secret secret(m_secretsIdentifier);
    secret.setData(data);

    Sailfish::Secrets::StoreSecretRequest storeCode;
    storeCode.setManager(&m_secretManager);
    storeCode.setSecretStorageType(Sailfish::Secrets::StoreSecretRequest::CollectionSecret);
    storeCode.setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    storeCode.setSecret(secret);
    storeCode.startRequest();

#ifdef QT_DEBUG
    storeCode.waitForFinished();

    qDebug() << storeCode.result().code();
    qDebug() << storeCode.result().errorMessage();
#endif
}


void Wallet::setCloudhookUrl(const QString &url)
{
    if (m_secrets.cloudhookUrl == url)
        return;

    m_secrets.cloudhookUrl = url;
    emit cloudhookUrlChanged(m_secrets.cloudhookUrl);
}

void Wallet::setRemoteUiUrl(const QString &url)
{
    if (m_secrets.remoteUiUrl == url)
        return;

    m_secrets.remoteUiUrl = url;
    emit remoteUiUrlChanged(m_secrets.remoteUiUrl);
}

void Wallet::setSecret(const QString &secret)
{
    if (m_secrets.secret == secret)
        return;

    m_secrets.secret = secret;
    emit secretChanged(m_secrets.secret);
}

void Wallet::setToken(const QString &token)
{
    if (m_secrets.token == token)
        return;

    m_secrets.token = token;
    emit tokenChanged(m_secrets.token);
}

void Wallet::setWebhookId(const QString &id)
{
    if (m_secrets.webhookId == id)
        return;

    m_secrets.webhookId = id;
    emit webhookIdChanged(m_secrets.webhookId);
}

void Wallet::onSecretsLoaded()
{
    auto *fetchCode = qobject_cast<Sailfish::Secrets::StoredSecretRequest *>(sender());

    if (!fetchCode)
        return;

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("SECRETS LOADED");
    qDebug() << fetchCode->result().code();
    qDebug() << fetchCode->result().errorCode();
    qDebug() << fetchCode->result().errorMessage();
#endif

    if (fetchCode->result().code() != Sailfish::Secrets::Result::Succeeded) {
        emit initialized(true);
        return;
    }

    QByteArray data = fetchCode->secret().data();

    QDataStream stream(&data, QIODevice::ReadOnly);
    stream >> m_secrets;

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("WEBHOOK ID");
    qDebug() << m_secrets.webhookId;
#endif

    fetchCode->deleteLater();

    emit initialized(true);
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

#ifdef QT_DEBUG
    qDebug() << createCollection.result().code();
    qDebug() << createCollection.result().errorMessage();
#endif
}

void Wallet::getSecrets()
{
    auto fetchCode = new Sailfish::Secrets::StoredSecretRequest;

    fetchCode->setManager(&m_secretManager);
    fetchCode->setUserInteractionMode(Sailfish::Secrets::SecretManager::SystemInteraction);
    fetchCode->setIdentifier(m_secretsIdentifier);

    connect(fetchCode, &Sailfish::Secrets::StoredSecretRequest::secretChanged, this, &Wallet::onSecretsLoaded);

    fetchCode->startRequest();
}
