import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: busy
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    DialogHeader {
        id: header
        acceptText: qsTr("Complete")
        cancelText: qsTr("Back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingMedium

        Label {
            width: parent.width

            text: qsTr("Setup process completed")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            visible: canAccept
            width: parent.width
            wrapMode: Text.Wrap

            text: qsTr("You can now proceed to the main screen.")
                  + "\n"
                  + qsTr("Some of the setting from this wizard can be changed in the applications settings page.")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            visible: canAccept
            width: parent.width
            wrapMode: Text.Wrap

            text: qsTr("Welcome to your smart home!")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeMedium
        }
    }

    onAccepted: {
        App.needSetup = false
        App.initialize()
        pageStack.clear()
        pageStack.push(Qt.resolvedUrl("../OverviewPage.qml"))
    }
}
