# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# VERSION
VERSION = 0.2.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-quartermaster
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

# custom defines
#DEFINES += DISABLE_SAILFISH_SECRETS

QT += dbus positioning network websockets

PKGCONFIG += \
    sailfishmdm \
    sailfishsecrets \
    nemonotifications-qt5 \
    qt5embedwidget

LIBS += -L../../lib -lkeepalive
LIBS += -lz

CONFIG += sailfishapp

SOURCES += src/harbour-quartermaster.cpp \
    src/api/apiinterface.cpp \
    src/api/serverconfig.cpp \
    src/app.cpp \
    src/constants.cpp \
    src/device/devicesensor.cpp \
    src/device/devicesensormodel.cpp \
    src/device/sensors/devicesensorbattery.cpp \
    src/device/sensors/devicesensorbatterycharging.cpp \
    src/entities/climate.cpp \
    src/entities/entity.cpp \
    src/entities/group.cpp \
    src/entities/light.cpp \
    src/entities/zone.cpp \
    src/models/entitiesmodel.cpp \
    src/models/entitiessortfiltermodel.cpp \
    src/models/entityattributesmodel.cpp \
    src/models/entitytypesmodel.cpp \
#    src/models/errorlogmodel.cpp \
#    src/models/logbookmodel.cpp \
    src/models/sortfiltermodel.cpp \
    src/models/zonesmodel.cpp \
#    src/threads/errorlogparser.cpp \
    src/network/accesspointsmodel.cpp \
    src/service/deviceservice.cpp \
    src/service/entitiesservice.cpp \
    src/service/locationservice.cpp \
    src/service/notificationservice.cpp \
    src/service/service.cpp \
    src/tools/backuptool.cpp \
    src/tools/helper.cpp

DISTFILES += qml/harbour-quartermaster.qml \
    qml/SelectWifiNetworkDialog.qml \
    qml/components/InfoItem.qml \
    qml/components/TestResultItem.qml \
    qml/cover/CoverPage.qml \
    qml/dialogs/BackupDialog.qml \
    qml/dialogs/RestoreBackupDialog.qml \
    qml/pages/ErrorLogPage.qml \
    qml/pages/OverviewPage.qml \
    qml/pages/SettingsConnectionPage.qml \
    qml/pages/SettingsDeviceInfoPage.qml \
    qml/pages/SettingsDeviceSensorsPage.qml \
    qml/pages/SettingsDeviceTrackingPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/SettingsZonePage.qml \
    qml/pages/SettingsZonesPage.qml \
    qml/pages/WebViewPage.qml \
    qml/pages/entities/EntitiesListViewPage.qml \
    qml/pages/entities/EntitiesOverviewPage.qml \
    qml/pages/entities/EntityAttributesPage.qml \
    qml/pages/entities/PersonsListPage.qml \
    qml/pages/entities/SensorsListPage.qml \
    qml/pages/entities/types/AutomationPage.qml \
    qml/pages/entities/types/CameraPage.qml \
    qml/pages/entities/types/ClimatePage.qml \
    qml/pages/entities/types/EntityPage.qml \
    qml/pages/entities/types/GroupPage.qml \
    qml/pages/entities/types/LightPage.qml \
    qml/pages/entities/types/SwitchPage.qml \
    qml/pages/log/ErrorLogEntryPage.qml \
    qml/pages/settings/SettingsBackupPage.qml \
    qml/pages/settings/SettingsCheckConfigPage.qml \
    qml/pages/settings/SettingsDeveloperModePage.qml \
    qml/pages/settings/SettingsHomeAssistantPage.qml \
    qml/pages/settings/SettingsLiveUpdatePage.qml \
    qml/pages/settings/SettingsLogBookPage.qml \
    qml/pages/settings/SettingsNotificationsPage.qml \
    qml/pages/wizard/WizardConnectionAdvancedPage.qml \
    qml/pages/wizard/WizardConnectionPage.qml \
    qml/pages/wizard/WizardDeviceRegistrationPage.qml \
    qml/pages/wizard/WizardInfoPage.qml \
    qml/pages/wizard/WizardIntroPage.qml \
    qml/pages/wizard/WizardLastPage.qml \
    qml/pages/wizard/WizardRegistrationResultPage.qml \
    qml/pages/wizard/WizardSelectHomeAccessPointPage.qml \
    qml/pages/wizard/WizardTokenPage.qml \
    rpm/harbour-quartermaster.changes \
    rpm/harbour-quartermaster.changes.run.in \
    rpm/harbour-quartermaster.spec \
    rpm/harbour-quartermaster.yaml \
    translations/*.ts \
    harbour-quartermaster.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 512x512

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-quartermaster-de.ts

RESOURCES += \
    ressources.qrc

HEADERS += \
    src/api/api.h \
    src/api/apiinterface.h \
    src/api/credentials.h \
    src/api/serverconfig.h \
    src/app.h \
    src/constants.h \
    src/constants.h \
    src/device/devicesensor.h \
    src/device/devicesensormodel.h \
    src/device/sensors/devicesensorbattery.h \
    src/device/sensors/devicesensorbatterycharging.h \
    src/entities/climate.h \
    src/entities/entity.h \
    src/entities/group.h \
    src/entities/light.h \
    src/entities/zone.h \
    src/models/entitiesmodel.h \
    src/models/entitiessortfiltermodel.h \
    src/models/entityattributesmodel.h \
    src/models/entitytypesmodel.h \
#    src/models/errorlogmodel.h \
#    src/models/logbookmodel.h \
    src/models/sortfiltermodel.h \
    src/models/zonesmodel.h \
#    src/threads/errorlogparser.h \
    src/network/accesspoint.h \
    src/network/accesspointsmodel.h \
    src/service/deviceservice.h \
    src/service/entitiesservice.h \
    src/service/locationservice.h \
    src/service/notificationservice.h \
    src/service/service.h \
    src/tools/backuptool.h \
    src/enums.h \
    src/tools/helper.h
