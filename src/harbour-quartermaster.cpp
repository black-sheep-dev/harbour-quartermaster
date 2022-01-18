#include <QtQuick>

#include <sailfishapp.h>

#include "app.h"

#include "models/entityattributesmodel.h"
#include "models/entitiessortfiltermodel.h"
#include "models/sortfiltermodel.h"
#include "tools/backuptool.h"
#include "models/errorlogmodel.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationVersion(APP_VERSION);
    app->setApplicationName("quartermaster");
    app->setOrganizationDomain("org.nubecula");
    app->setOrganizationName("org.nubecula");

    QScopedPointer<QQuickView> v(SailfishApp::createView());

    qRegisterMetaTypeStreamOperators<Credentials>("Credentials");

#ifdef QT_DEBUG
#define uri "org.nubecula.harbour.quartermaster"
#else
    auto uri = "org.nubecula.harbour.quartermaster";
#endif

//    qmlRegisterType<ErrorLogModel>(uri, 1, 0, "ErrorLogModel");

    qmlRegisterUncreatableType<Api>(uri, 1, 0, "Api", "not createable");

    qmlRegisterType<AccessPointsModel>(uri, 1, 0, "AccessPointsModel");
    qmlRegisterType<ApiInterface>(uri, 1, 0, "ApiInterface");
    qmlRegisterType<BackupTool>(uri, 1, 0, "BackupTool");
    qmlRegisterType<Climate>(uri, 1, 0, "Climate");
    qmlRegisterType<DeviceSensor>(uri, 1, 0, "DeviceSensor");
    qmlRegisterType<DeviceSensorModel>(uri, 1, 0, "DeviceSensorModel");
    qmlRegisterType<DeviceService>(uri, 1, 0, "DeviceService");
    qmlRegisterType<Entity>(uri, 1, 0, "Entity");
    qmlRegisterType<EntityAttributesModel>(uri, 1, 0, "EntityAttributesModel");
    qmlRegisterType<EntitiesModel>(uri, 1, 0, "EntitiesModel");
    qmlRegisterType<EntitiesService>(uri, 1, 0, "EntitiesService");
    qmlRegisterType<EntitiesSortFilterModel>(uri, 1, 0, "EntitiesSortFilterModel");
    qmlRegisterType<EntityTypesModel>(uri, 1, 0, "EntityTypesModel");
    qmlRegisterType<Group>(uri, 1, 0, "Group");
    qmlRegisterType<Light>(uri, 1, 0, "Light");
    qmlRegisterType<LocationService>(uri, 1, 0, "LocationService");
    qmlRegisterType<NotificationService>(uri, 1, 0, "NotificationService");
    qmlRegisterType<ServerConfig>(uri, 1, 0, "ServerConfig");
    qmlRegisterType<SortFilterModel>(uri, 1, 0, "SortFilterModel");
    qmlRegisterType<Zone>(uri, 1, 0, "Zone");
    qmlRegisterType<ZonesModel>(uri, 1, 0, "ZonesModel");

    qmlRegisterSingletonType<App>(uri,
                                  1,
                                  0,
                                  "App",
                                  [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        auto app = new App(qApp);

        return app;
    });


    v->setSource(SailfishApp::pathTo("qml/harbour-quartermaster.qml"));
    v->show();

    return app->exec();
}
