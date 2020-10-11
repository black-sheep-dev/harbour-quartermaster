#include "cameraprovider.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>

CameraProvider::CameraProvider(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this)),
    m_timer(new QTimer(this))
{
    m_timer->setInterval(5000);
    connect(m_timer, &QTimer::timeout, this, &CameraProvider::updateImage);

    connect(m_manager, &QNetworkAccessManager::finished, this, &CameraProvider::onDownloadFinished);
}

bool CameraProvider::setActive(bool activate)
{
    if (m_url.isEmpty())
        return false;

    if (activate) {
        updateImage();
        m_timer->start();
    } else {
        m_timer->stop();
    }

    return true;
}

QImage CameraProvider::image() const
{
    return m_image;
}

QString CameraProvider::url() const
{
    return m_url;
}

void CameraProvider::updateImage()
{
    if (m_url.isEmpty())
        return;

    QNetworkRequest request(m_url);

    m_manager->get(request);
}

void CameraProvider::setImage(const QImage &image)
{
    if (m_image == image)
        return;

    m_image = image;
    emit imageChanged(m_image);
}

void CameraProvider::setUrl(const QString &url)
{
    if (m_url == url)
        return;

    m_url = url;
    emit urlChanged(m_url);
}

void CameraProvider::onDownloadFinished(QNetworkReply *reply)
{
#ifdef QT_DEBUG
    qDebug() << reply->url();
#endif

    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("DOWNLOAD FAILED");
        qDebug() << reply->errorString();
#endif
        reply->deleteLater();
        return;
    }

    QPixmap pixmap;
    pixmap.loadFromData(reply->readAll());

    setImage(pixmap.toImage());
}
