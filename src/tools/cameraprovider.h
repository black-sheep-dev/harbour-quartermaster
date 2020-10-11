#ifndef CAMERAPROVIDER_H
#define CAMERAPROVIDER_H

#include <QObject>

#include <QImage>
#include <QNetworkAccessManager>
#include <QTimer>

class CameraProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit CameraProvider(QObject *parent = nullptr);

    Q_INVOKABLE bool setActive(bool activate = true);

    // properties
    QImage image() const;
    QString url() const;

signals:
    // properties
    void imageChanged(const QImage &image);
    void urlChanged(const QString &url);

public slots:
    Q_INVOKABLE void updateImage();

    // properties
    void setImage(const QImage &image);
    void setUrl(const QString &url);

private slots:
    void onDownloadFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager{nullptr};
    QTimer *m_timer{nullptr};

    // properties
    QImage m_image;
    QString m_url;

};

#endif // CAMERAPROVIDER_H
