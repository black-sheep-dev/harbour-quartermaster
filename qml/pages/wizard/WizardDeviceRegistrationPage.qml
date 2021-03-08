import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardLastPage.qml")

    DialogHeader {
        id: header
        acceptText: qsTr("Register")
        cancelText: qsTr("Back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingMedium

        Label {
            width: parent.width

            text: qsTr("Device registration")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap

            text: qsTr("This is the final step. You can change the device name which shows up in Home Assistant if you want before starting the registration.")
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

            label: qsTr("Device name")
            placeholderText: qsTr("Enter device name")

            text: App.device().name

            onTextChanged: checkInput()
        }
    }

    function checkInput() { canAccept = deviceNameField.length > 0 }

    onAccepted: {
        App.device().name = deviceNameField.text
        App.registerDevice()
    }

    Component.onCompleted: checkInput()
}

