#include <QtQuick>

#include <sailfishapp.h>

#include "client/clientinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("Quartermaster"));
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qRegisterMetaTypeStreamOperators<Secrets>("WebhookSecrets");

    //qRegisterMetaType<EntityContext>("EntityContext");

    qmlRegisterType<Device>("org.nubecula.harbour.quartermaster", 1, 0, "Device");
    qmlRegisterType<DeviceSensor>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceSensor");
    qmlRegisterType<DeviceSensorModel>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceSensorModel");
    qmlRegisterType<HomeassistantInfo>("org.nubecula.harbour.quartermaster", 1, 0, "HomeassistantInfo");
    qmlRegisterType<WifiNetworkModel>("org.nubecula.harbour.quartermaster", 1, 0, "WifiNetworkModel");
    qmlRegisterType<Zone>("org.nubecula.harbour.quartermaster", 1, 0, "Zone");
    qmlRegisterType<ZonesModel>("org.nubecula.harbour.quartermaster", 1, 0, "ZonesModel");

    qmlRegisterSingletonType<ClientInterface>("org.nubecula.harbour.quartermaster",
                                              1,
                                              0,
                                              "Client",
                                              [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        auto *client = new ClientInterface();

        return client;
    });

    return SailfishApp::main(argc, argv);
}
