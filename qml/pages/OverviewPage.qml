import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: !Client.ready
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            visible: Client.ready

            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("settings/SettingsPage.qml"))
            }
            MenuItem {
                text: qsTr("Entities")
                onClicked: pageStack.push(Qt.resolvedUrl("entities/EntitiesOverviewPage.qml"))
            }
        }

        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Overview")
            }
            Label {
                x: Theme.horizontalPageMargin
                text: Client.debugOutput
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
        }
    }

    Connections {
        target: Client
        onReadyChanged: {
            if (!Client.ready) return

            if (!Client.isRegistered()) {
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("wizard/WizardIntroPage.qml"))
            }
        }
    }
}
