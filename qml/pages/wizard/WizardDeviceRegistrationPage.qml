import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardRegistrationResultPage.qml")

    DialogHeader {
        id: header
        //% "Register"
        acceptText: qsTrId("id-register")
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

            //% "Device registration"
            text: qsTrId("id-device-registration")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap

            //% "You can change the device name which shows up in Home Assistant before starting the registration."
            text: qsTrId("id-change-device-name-desc")
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
        }

        Item {
            width: 1
            height: Theme.paddingLarge
        }

        TextField {
            id: deviceNameField
            width: parent.width

            //% "Device name"
            label: qsTrId("id-device-name")
            //% "Enter device name"
            placeholderText: qsTrId("id-enter-device-name")

            text: App.deviceService().deviceName

            onTextChanged: checkInput()
        }
    }

    function checkInput() { canAccept = deviceNameField.length > 0 }

    onAccepted: {
        App.deviceService().deviceName = deviceNameField.text
        App.deviceService().registerDevice()
    }

    Component.onCompleted: checkInput()
}

