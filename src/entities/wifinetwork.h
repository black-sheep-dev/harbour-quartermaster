#ifndef WIFINETWORK_H
#define WIFINETWORK_H

#include <QObject>

class WifiNetwork : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

public:
    explicit WifiNetwork(QObject *parent = nullptr);

    QString identifier() const;
    QString name() const;
    bool selected() const;

signals:
    void identifierChanged(const QString &identifier);
    void nameChanged(const QString &name);
    void selectedChanged(bool selected);

public slots:
    void setIdentifier(const QString &identifier);
    void setName(const QString &name);
    void setSelected(bool selected);

private:
    QString m_identifier;
    QString m_name;
    bool m_selected{false};
};

#endif // WIFINETWORK_H
