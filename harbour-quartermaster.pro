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
VERSION = 0.1.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-quartermaster
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

PKGCONFIG += \
    sailfishmdm

CONFIG += sailfishapp

SOURCES += src/harbour-quartermaster.cpp \
    src/client/clientinterface.cpp \
    src/client/homeassistantinfo.cpp \
    src/client/webhookinterface.cpp \
    src/device/device.cpp

DISTFILES += qml/harbour-quartermaster.qml \
    qml/components/ConnectionSettings.qml \
    qml/components/InfoItem.qml \
    qml/components/TestResultItem.qml \
    qml/cover/CoverPage.qml \
    qml/pages/OverviewPage.qml \
    qml/pages/SettingsConnectionPage.qml \
    qml/pages/SettingsDeviceInfoPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/wizard/WizardConnectionPage.qml \
    qml/pages/wizard/WizardDeviceRegistrationPage.qml \
    qml/pages/wizard/WizardInfoPage.qml \
    qml/pages/wizard/WizardIntroPage.qml \
    qml/pages/wizard/WizardLastPage.qml \
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
    src/client/clientinterface.h \
    src/client/homeassistantinfo.h \
    src/client/webhookinterface.h \
    src/device/device.h
