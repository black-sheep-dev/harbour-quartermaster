#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QJsonArray>
#include <QJsonObject>

#include <mdm-batteryinfo.h>

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Device(QObject *parent = nullptr);

    Q_INVOKABLE int batteryChargePercentage() const;
    Q_INVOKABLE QString id() const;
    Q_INVOKABLE QString manufacturer() const;
    Q_INVOKABLE QString model() const;
    Q_INVOKABLE QString softwareName() const;
    Q_INVOKABLE QString softwareVersion() const;
    Q_INVOKABLE QString wlanMacAddress() const;

    // internal
    QJsonArray sensors() const;

    // properties
    QString name() const;

signals:
    void sensorUpdated(const QJsonObject &sensor);

    // properties
    void nameChanged(const QString &name);

public slots:
    // properties
    void setName(const QString &name);

private:
    QString batteryIcon(int percentage) const;

    Sailfish::Mdm::BatteryInfo m_battery;

    // properties
    QString m_name;
};

#endif // DEVICE_H
