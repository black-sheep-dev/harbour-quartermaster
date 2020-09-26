#include <QtQuick>

#include <sailfishapp.h>

#include "client/clientinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("Quartermaster"));
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qmlRegisterType<Device>("org.nubecula.harbour.quartermaster", 1, 0, "Device");
    qmlRegisterType<DeviceSensor>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceSensor");
    qmlRegisterType<DeviceSensorModel>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceSensorModel");
    qmlRegisterType<DeviceTracker>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceTracker");
    qmlRegisterType<HomeassistantInfo>("org.nubecula.harbour.quartermaster", 1, 0, "HomeassistantInfo");

    qmlRegisterSingletonType<ClientInterface>("org.nubecula.harbour.quartermaster",
                                              1,
                                              0,
                                              "Client",
                                              [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        ClientInterface *client = new ClientInterface();

        return client;
    });

    return SailfishApp::main(argc, argv);
}
