#ifndef APP_H
#define APP_H

#include <QObject>

#include <QMutex>

#include "api/apiinterface.h"
#include "service/deviceservice.h"
#include "service/entitiesservice.h"
#include "service/locationservice.h"

class App : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool needSetup READ needSetup WRITE setNeedSetup NOTIFY needSetupChanged)

public:
    explicit App(QObject *parent = nullptr);
    ~App();

    // objects
    Q_INVOKABLE ApiInterface *api();
    Q_INVOKABLE DeviceService *deviceService();
    Q_INVOKABLE EntitiesService *entitiesService();
    Q_INVOKABLE LocationService *locationService();

    // functions
    Q_INVOKABLE void initialize();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void saveSettings();

    // properties
    bool needSetup() const;

signals:
    // properties
    void needSetupChanged(bool needSetup);

public slots:
    // properties
    void setNeedSetup(bool needSetup);

private slots:
    void onError(quint8 code, const QString &msg);

private:
    void readSetting();
    void writeSettings();

    ApiInterface *m_api{new ApiInterface(this)};
    DeviceService *m_deviceService{new DeviceService(this)};
    EntitiesService *m_entitiesService{new EntitiesService(this)};
    LocationService *m_locationService{new LocationService(this)};
    QMutex *m_mutex{new QMutex()};

    // properties
    bool m_needSetup{true};

};

#endif // APP_H
