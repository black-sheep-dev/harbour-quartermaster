#include <QtQuick>

#include <sailfishapp.h>

#include "client/clientinterface.h"
#include "models/entityattributesmodel.h"
#include "models/entitiessortfiltermodel.h"
#include "models/hvacmodesmodel.h"
#include "models/sortfiltermodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Quartermaster");
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qRegisterMetaTypeStreamOperators<Secrets>("WebhookSecrets");

    qmlRegisterType<Climate>("org.nubecula.harbour.quartermaster", 1, 0, "Climate");
    qmlRegisterType<Group>("org.nubecula.harbour.quartermaster", 1, 0, "Group");
    qmlRegisterType<Light>("org.nubecula.harbour.quartermaster", 1, 0, "Light");

    qmlRegisterType<Device>("org.nubecula.harbour.quartermaster", 1, 0, "Device");
    qmlRegisterType<DeviceSensor>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceSensor");
    qmlRegisterType<DeviceSensorModel>("org.nubecula.harbour.quartermaster", 1, 0, "DeviceSensorModel");
    qmlRegisterType<Entity>("org.nubecula.harbour.quartermaster", 1, 0, "Entity");
    qmlRegisterType<EntityAttributesModel>("org.nubecula.harbour.quartermaster", 1, 0, "EntityAttributesModel");
    qmlRegisterType<EntitiesModel>("org.nubecula.harbour.quartermaster", 1, 0, "EntitiesModel");
    qmlRegisterType<EntitiesProvider>("org.nubecula.harbour.quartermaster", 1, 0, "EntitiesProvider");
    qmlRegisterType<EntitiesSortFilterModel>("org.nubecula.harbour.quartermaster", 1, 0, "EntitiesSortFilterModel");
    qmlRegisterType<EntityTypesModel>("org.nubecula.harbour.quartermaster", 1, 0, "EntityTypesModel");
    qmlRegisterType<HomeassistantInfo>("org.nubecula.harbour.quartermaster", 1, 0, "HomeassistantInfo");
    qmlRegisterType<SortFilterModel>("org.nubecula.harbour.quartermaster", 1, 0, "SortFilterModel");
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
