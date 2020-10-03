#ifndef HOMEASSISTANTINFO_H
#define HOMEASSISTANTINFO_H

#include <QObject>

#include <QVariantList>

class HomeassistantInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QVariantList componentList READ componentList WRITE setComponentList NOTIFY componentListChanged)
    Q_PROPERTY(HomeassistantInfo::Components components READ components WRITE setComponents NOTIFY componentsChanged)
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
    Q_PROPERTY(QString externalUrl READ externalUrl WRITE setExternalUrl NOTIFY externalUrlChanged)
    Q_PROPERTY(QString internalUrl READ internalUrl WRITE setInternalUrl NOTIFY internalUrlChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(bool versionCompatibility READ versionCompatibility WRITE setVersionCompatibility NOTIFY versionCompatibilityChanged)

public:
    enum Component {
        ComponentNone                   = 0x00,
        ComponentMobileApp              = 0x01,
        ComponentWebhook                = 0x02,
        ComponentWebsocketApi           = 0x04
    };
    Q_DECLARE_FLAGS(Components, Component)
    Q_FLAG(Components)
    Q_ENUM(Component)

    explicit HomeassistantInfo(QObject *parent = nullptr);

    Q_INVOKABLE bool isInstanceValid();

    void setData(const QJsonObject &object);

    // properties
    bool available() const;
    QVariantList componentList() const;
    Components components() const;
    QString error() const;
    QString externalUrl() const;
    QString internalUrl() const;
    bool loading() const;
    QString locationName() const;
    QString version() const;
    bool versionCompatibility() const;

signals:
    // properties
    void availableChanged(bool available);
    void componentListChanged(const QVariantList &components);
    void componentsChanged(HomeassistantInfo::Components components);
    void errorChanged(const QString &error);
    void externalUrlChanged(const QString &url);
    void internalUrlChanged(const QString &url);
    void loadingChanged(bool loading);
    void locationNameChanged(const QString &name);
    void versionChanged(const QString &version);
    void versionCompatibilityChanged(bool compatibility);

public slots:
    // properties
    void setAvailable(bool available);
    void setComponentList(const QVariantList &components);
    void setComponents(HomeassistantInfo::Components components);
    void setError(const QString &error);
    void setExternalUrl(const QString &url);
    void setInternalUrl(const QString &url);
    void setLoading(bool loading);
    void setLocationName(const QString &name);
    void setVersion(const QString &version);
    void setVersionCompatibility(bool compatibility);

private:
    // properties
    bool m_available{false};
    QVariantList m_componentList;
    HomeassistantInfo::Components m_components{HomeassistantInfo::ComponentNone};
    QString m_error;
    QString m_externalUrl;
    QString m_internalUrl;
    bool m_loading{false};
    QString m_locationName;
    QString m_version;
    bool m_versionCompatibility{false};
    bool m_busy{};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(HomeassistantInfo::Components)

#endif // HOMEASSISTANTINFO_H
