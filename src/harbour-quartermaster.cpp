#include <QtQuick>

#include <sailfishapp.h>

#include "client/clientinterface.h"
#include "models/entityattributesmodel.h"
#include "models/entitiessortfiltermodel.h"
#include "models/sortfiltermodel.h"
#include "tools/backuptool.h"
#include "models/errorlogmodel.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setApplicationName(tr("Quartermaster"));
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qRegisterMetaTypeStreamOperators<Secrets>("WebhookSecrets");

    auto uri = "org.nubecula.harbour.quartermaster";

    qmlRegisterType<Climate>(uri, 1, 0, "Climate");
    qmlRegisterType<Group>(uri, 1, 0, "Group");
    qmlRegisterType<Light>(uri, 1, 0, "Light");

    qmlRegisterType<BackupTool>(uri, 1, 0, "BackupTool");
    qmlRegisterType<Device>(uri, 1, 0, "Device");
    qmlRegisterType<DeviceSensor>(uri, 1, 0, "DeviceSensor");
    qmlRegisterType<DeviceSensorModel>(uri, 1, 0, "DeviceSensorModel");
    qmlRegisterType<Entity>(uri, 1, 0, "Entity");
    qmlRegisterType<EntityAttributesModel>(uri, 1, 0, "EntityAttributesModel");
    qmlRegisterType<EntitiesModel>(uri, 1, 0, "EntitiesModel");
    qmlRegisterType<EntitiesProvider>(uri, 1, 0, "EntitiesProvider");
    qmlRegisterType<EntitiesSortFilterModel>(uri, 1, 0, "EntitiesSortFilterModel");
    qmlRegisterType<EntityTypesModel>(uri, 1, 0, "EntityTypesModel");
    qmlRegisterType<ErrorLogModel>(uri, 1, 0, "ErrorLogModel");
    qmlRegisterType<HomeassistantInfo>(uri, 1, 0, "HomeassistantInfo");
    qmlRegisterType<SortFilterModel>(uri, 1, 0, "SortFilterModel");
    qmlRegisterType<WifiNetworkModel>(uri, 1, 0, "WifiNetworkModel");
    qmlRegisterType<Zone>(uri, 1, 0, "Zone");
    qmlRegisterType<ZonesModel>(uri, 1, 0, "ZonesModel");

    qmlRegisterSingletonType<ClientInterface>(uri,
                                              1,
                                              0,
                                              "Client",
                                              [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        auto client = new ClientInterface();

        return client;
    });

    return SailfishApp::main(argc, argv);
}
