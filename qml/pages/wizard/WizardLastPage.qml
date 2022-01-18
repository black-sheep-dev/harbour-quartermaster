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
        //% "Complete"
        acceptText: qsTrId("id-complete")
        //% "Back"
        cancelText: qsTrId("id-back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingMedium

        Label {
            width: parent.width

            //% "Setup process completed"
            text: qsTrId("id-setup-process-completed")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            visible: canAccept
            width: parent.width
            wrapMode: Text.Wrap

            //% "You can now proceed to the main screen."
            //% "Some of the setting from this wizard can be changed in the applications settings page."
            text: qsTrId("id-setup-process-completed-desc")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            visible: canAccept
            width: parent.width
            wrapMode: Text.Wrap

            //% "Welcome to your smart home!"
            text: qsTrId("id-welcome-to-smart-home")

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
