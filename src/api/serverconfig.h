#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QObject>

#include <QVariantList>
#include <QVersionNumber>

#include "src/entities/zone.h"

class ServerConfig : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QVariantList componentList READ componentList WRITE setComponentList NOTIFY componentListChanged)  

    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
//    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)

    Q_PROPERTY(QString configError READ configError WRITE setConfigError NOTIFY configErrorChanged)
    Q_PROPERTY(bool configValid READ configValid WRITE setConfigValid NOTIFY configValidChanged)
    Q_PROPERTY(quint16 components READ components WRITE setComponents NOTIFY componentsChanged)
    Q_PROPERTY(quint16 externalPort READ externalPort WRITE setExternalPort NOTIFY externalPortChanged)
    Q_PROPERTY(QString externalUrl READ externalUrl WRITE setExternalUrl NOTIFY externalUrlChanged)
    Q_PROPERTY(quint16 internalPort READ internalPort WRITE setInternalPort NOTIFY internalPortChanged)
    Q_PROPERTY(QString internalUrl READ internalUrl WRITE setInternalUrl NOTIFY internalUrlChanged)
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

    //Q_INVOKABLE Zone *homezone() const;
    Q_INVOKABLE bool isCompatible();
    bool isUpdateAvailable(const QString &version) const;

    void setData(const QJsonObject &object);

    // properties
    bool available() const;
    QVariantList componentList() const;
    quint16 components() const;
    QString configError() const;
    bool configValid() const;
    QString error() const;
    quint16 externalPort() const;
    QString externalUrl() const;
    quint16 internalPort() const;
    QString internalUrl() const;
    bool loading() const;
//    QString locationName() const;
    QString unitLength() const;
    QString unitMass() const;
    QString unitPressure() const;
    QString unitTemperature() const;
    QString unitVolume() const;
    QString version() const;
    bool versionCompatibility() const;

signals:
    // properties
    void availableChanged(bool available);
    void componentListChanged(const QVariantList &components);
    void componentsChanged(quint16 components);
    void configErrorChanged(const QString &error);
    void configValidChanged(bool valid);
    void errorChanged(const QString &error);
    void externalPortChanged(quint16 port);
    void externalUrlChanged(const QString &url);
    void internalPortChanged(quint16 port);
    void internalUrlChanged(const QString &url);
    void loadingChanged(bool loading);
//    void locationNameChanged(const QString &name);
    void unitLengthChanged(const QString &unitLength);
    void unitMassChanged(const QString &unitMass);
    void unitPressureChanged(const QString &unitPressure);
    void unitTemperatureChanged(const QString &unitTemperature);
    void unitVolumeChanged(const QString &unitVolume);
    void versionChanged(const QString &version);
    void versionCompatibilityChanged(bool compatibility);

public slots:
    // properties
    void setAvailable(bool available);
    void setComponentList(const QVariantList &components);
    void setComponents(quint16 components);
    void setConfigError(const QString &error);
    void setConfigValid(bool valid);
    void setError(const QString &error);
    void setExternalPort(quint16 port);
    void setExternalUrl(const QString &url);
    void setInternalPort(quint16 port);
    void setInternalUrl(const QString &url);
    void setLoading(bool loading);
//    void setLocationName(const QString &name);
    void setUnitLength(const QString &unit);
    void setUnitMass(const QString &unit);
    void setUnitPressure(const QString &unit);
    void setUnitTemperature(const QString &unit);
    void setUnitVolume(const QString &unit);
    void setVersion(const QString &version);
    void setVersionCompatibility(bool compatibility);

private:
//    Zone *m_zone{new Zone(this};

    // properties
    bool m_available{false};
    QVariantList m_componentList;
    quint16 m_components{ServerConfig::ComponentNone};
    QString m_configError;
    bool m_configValid{true};
    QString m_error;
    quint16 m_externalPort{8123};
    QString m_externalUrl;
    quint16 m_internalPort{8123};
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
    bool m_busy{};

};
Q_DECLARE_OPERATORS_FOR_FLAGS(ServerConfig::Components)

#endif // SERVERCONFIG_H
