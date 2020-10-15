#ifndef WIFINETWORK_H
#define WIFINETWORK_H

#include <QObject>

class WifiNetwork : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool defined READ defined WRITE setDefined NOTIFY definedChanged)
    Q_PROPERTY(bool discovered READ discovered WRITE setDiscovered NOTIFY discoveredChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

public:
    explicit WifiNetwork(QObject *parent = nullptr);

    bool defined() const;
    bool discovered() const;
    QString identifier() const;
    QString name() const;
    bool selected() const;

signals:
    void definedChanged(bool defined);
    void discoveredChanged(bool discovered);
    void identifierChanged(const QString &identifier);
    void nameChanged(const QString &name);
    void selectedChanged(bool selected);

public slots:
    void setDefined(bool defined);
    void setDiscovered(bool discovered);
    void setIdentifier(const QString &identifier);
    void setName(const QString &name);
    void setSelected(bool selected);

private:
    bool m_defined{false};
    bool m_discovered{false};
    QString m_identifier;
    QString m_name;
    bool m_selected{false};
};

#endif // WIFINETWORK_H
