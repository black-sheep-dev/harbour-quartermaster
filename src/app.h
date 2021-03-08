#ifndef APP_H
#define APP_H

#include <QObject>

#include "api/apiconnector.h"
#include "crypto/wallet.h"
#include "device/device.h"

class App : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool needSetup READ needSetup WRITE setNeedSetup NOTIFY needSetupChanged)

public:
    explicit App(QObject *parent = nullptr);
    ~App() override;

    // objects
    Q_INVOKABLE ApiConnector *api();
    Q_INVOKABLE Device *device();
    Q_INVOKABLE Wallet *wallet();

    // functions
    Q_INVOKABLE void initialize();
    Q_INVOKABLE void registerDevice();
    Q_INVOKABLE void saveSettings();

    // properties
    bool needSetup() const;

signals:
    // properties
    void needSetupChanged(bool needSetup);

public slots:
    // properties
    void setNeedSetup(bool needSetup);

private:
    void readSetting();
    void writeSettings();

    ApiConnector *m_api{new ApiConnector(this)};
    Device *m_device{new Device(this)};
    Wallet *m_wallet{new Wallet(this)};

    // properties
    bool m_needSetup{true};
};

#endif // APP_H
