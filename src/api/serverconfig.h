#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QObject>

#include <QVariantList>
#include <QVersionNumber>

#include "src/entities/zone.h"

class ServerConfig : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList componentList READ componentList WRITE setComponentList NOTIFY componentListChanged)  
    Q_PROPERTY(QString configError READ configError WRITE setConfigError NOTIFY configErrorChanged)
    Q_PROPERTY(bool configValid READ configValid WRITE setConfigValid NOTIFY configValidChanged)
    Q_PROPERTY(quint16 components READ components WRITE setComponents NOTIFY componentsChanged)
    Q_PROPERTY(QString externalUrl READ externalUrl WRITE setExternalUrl NOTIFY externalUrlChanged)
    Q_PROPERTY(QString internalUrl READ internalUrl WRITE setInternalUrl NOTIFY internalUrlChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(QString unitLength READ unitLength WRITE setUnitLength NOTIFY unitLengthChanged)
    Q_PROPERTY(QString unitMass READ unitMass WRITE setUnitMass NOTIFY unitMassChanged)
    Q_PROPERTY(QString unitPressure READ unitPressure WRITE setUnitPressure NOTIFY unitPressureChanged)
    Q_PROPERTY(QString unitTemperature READ unitTemperature WRITE setUnitTemperature NOTIFY unitTemperatureChanged)
    Q_PROPERTY(QString unitVolume READ unitVolume WRITE setUnitVolume NOTIFY unitVolumeChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(bool versionCompatibility READ versionCompatibility WRITE setVersionCompatibility NOTIFY versionCompatibilityChanged)

public:
    enum Component {
        ComponentNone                   = 0x00,
        ComponentMobileApp              = 0x01,
        ComponentWebhook                = 0x02,
        ComponentWebsocketApi           = 0x04,
        ComponentConfig                 = 0x08
    };
    Q_DECLARE_FLAGS(Components, Component)
    Q_FLAG(Components)
    Q_ENUM(Component)

    explicit ServerConfig(QObject *parent = nullptr);

    Q_INVOKABLE bool isCompatible();
    bool isUpdateAvailable(const QString &version) const;

    void setData(const QJsonObject &object);

    // properties
    QVariantList componentList() const;
    quint16 components() const;
    QString configError() const;
    bool configValid() const;
    QString externalUrl() const;
    QString internalUrl() const;
    double latitude() const;
    QString locationName() const;
    double longitude() const;
    QString unitLength() const;
    QString unitMass() const;
    QString unitPressure() const;
    QString unitTemperature() const;
    QString unitVolume() const;
    QString version() const;
    bool versionCompatibility() const;

signals:
    // properties
    void componentListChanged(const QVariantList &components);
    void componentsChanged(quint16 components);
    void configErrorChanged(const QString &error);
    void configValidChanged(bool valid);
    void externalUrlChanged(const QString &url);
    void internalUrlChanged(const QString &url);
    void latitudeChanged(double latitude);
    void locationNameChanged(const QString &locationName);
    void longitudeChanged(double longitude);
    void unitLengthChanged(const QString &unitLength);
    void unitMassChanged(const QString &unitMass);
    void unitPressureChanged(const QString &unitPressure);
    void unitTemperatureChanged(const QString &unitTemperature);
    void unitVolumeChanged(const QString &unitVolume);
    void versionChanged(const QString &version);
    void versionCompatibilityChanged(bool compatibility);

public slots:
    // properties
    void setComponentList(const QVariantList &components);
    void setComponents(quint16 components);
    void setConfigError(const QString &error);
    void setConfigValid(bool valid);
    void setExternalUrl(const QString &url);
    void setInternalUrl(const QString &url);
    void setLatitude(double latitude);
    void setLocationName(const QString &locationName);
    void setLongitude(double longitude);
    void setUnitLength(const QString &unit);
    void setUnitMass(const QString &unit);
    void setUnitPressure(const QString &unit);
    void setUnitTemperature(const QString &unit);
    void setUnitVolume(const QString &unit);
    void setVersion(const QString &version);
    void setVersionCompatibility(bool compatibility);

private:
    QString cleanUrl(const QString &url) const;

    // properties
    QVariantList m_componentList;
    quint16 m_components{ServerConfig::ComponentNone};
    QString m_configError;
    bool m_configValid{true};
    QString m_externalUrl;
    QString m_internalUrl;
    bool m_loading{false};
    QString m_locationName;
    QString m_unitLength;
    QString m_unitMass;
    QString m_unitPressure;
    QString m_unitTemperature;
    QString m_unitVolume;
    QVersionNumber m_version;
    bool m_versionCompatibility{false};
    double m_latitude{0};
    double m_longitude{0};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ServerConfig::Components)

#endif // SERVERCONFIG_H
