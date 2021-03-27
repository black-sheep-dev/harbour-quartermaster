#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include "service.h"

class NotificationService : public Service
{
    Q_OBJECT

    Q_PROPERTY(QString homeAssistantVersion READ homeAssistantVersion WRITE setHomeAssistantVersion NOTIFY homeAssistantVersionChanged)
    Q_PROPERTY(quint8 options READ options WRITE setOptions NOTIFY optionsChanged)

public:
    enum NotificationOption {
        None                    = 0x00,
        ServerUpdates           = 0x01,
        WebsocketEvents         = 0x02

    };
    Q_ENUM(NotificationOption)
    Q_DECLARE_FLAGS(NotificationOptions, NotificationOption)

    explicit NotificationService(QObject *parent = nullptr);
    ~NotificationService();

    // properties
    QString homeAssistantVersion() const;
    quint8 options() const;

signals:
    // properties
    void homeAssistantVersionChanged(const QString &version);
    void optionsChanged(quint8 options);

public slots:
    void onHomeAssistantUpdateAvailable(const QString &version);

    // properties
    void setHomeAssistantVersion(const QString &version);
    void setOptions(quint8 options);

private:
    void updateOptions(quint8 before, quint8 after);

    bool m_updateNotified{false};

    // properties
    QString m_homeAssistantVersion;
    quint8 m_options{NotificationOption::None};

    // Service interface
public:
    void initialize() override;
    void readSettings() override;
    void writeSettings() override;

public slots:
    void onWebsocketEvent(const QString &event, const QJsonValue &data) override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(NotificationService::NotificationOptions)

#endif // NOTIFICATIONSERVICE_H
