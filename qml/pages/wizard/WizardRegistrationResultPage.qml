import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Dialog {
    property bool busy: true
    property string errorMsg

    id: dialog
    allowedOrientations: Orientation.Portrait

    acceptDestination: Qt.resolvedUrl("WizardSelectHomeAccessPointPage.qml")

    canAccept: false

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: busy
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    DialogHeader {
        id: header
        //% "Continue"
        acceptText: qsTrId("id-continue")
        //% "Back"
        cancelText: qsTrId("id-back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingMedium

        opacity: busy ? 0 : 1

        Behavior on opacity {
            FadeAnimation {}
        }

        // ERROR
        Label {
            visible: !canAccept
            width: parent.width

            //% "Registration failed"
            text: qsTrId("id-registration-failed")

            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            visible: !canAccept
            width: parent.width
            wrapMode: Text.Wrap

            text: errorMsg

            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeSmall
        }

        // SUCCESS
        Label {
            visible: canAccept
            width: parent.width

            //% "Registration completed"
            text: qsTrId("id-registration-completed")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            visible: canAccept
            width: parent.width
            wrapMode: Text.Wrap

            //% "In the next step you can choose your home network access point. This information is used to recognize whether you are at home."
            text: qsTrId("id-registration-next-step-info")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
        }

    }

    Connections {
        target: App.api()
        onRequestFinished: {
            if (requestType !== Api.RequestPostApiRegisterDevice) return;

            busy = false
            canAccept = true 
        }
        onRequestError: {
            if (requestType !== Api.RequestPostApiRegisterDevice) return;

            busy = false
            errorMsg = msg
        }
    }

    onAccepted: {
        App.locationService().getZones()
    }
}
