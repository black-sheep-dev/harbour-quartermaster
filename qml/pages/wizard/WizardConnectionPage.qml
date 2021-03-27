import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardConnectionAdvancedPage.qml")

    DialogHeader {
        id: header
        acceptText: qsTr("Connect")
        cancelText: qsTr("Back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x

        spacing: Theme.paddingSmall

        Label {
            width: parent.width

            text: qsTr("Connection Settings")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap

            text: qsTr("You need to provide the connection details to your Home Assistant server.")
                  + "\n"
                  + qsTr("This can either be the hostname or the IP address in the local network.")

            font.pixelSize: Theme.fontSizeSmall
            color: Theme.highlightColor
        }

        Item {
            width: 1
            height: Theme.paddingLarge
        }

        TextField {
            id: hostnameField
            width: parent.width

            label: qsTr("Hostname")
            placeholderText: qsTr("Enter hostname (e.g. http://server)")

            text: App.api().serverConfig().internalUrl

            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)[a-z0-9]+([\-\.]{1}[a-z0-9]+)*(\(.[a-z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: portField.focus = true

            autoScrollEnabled: true

            onTextChanged: checkInput()
        }

        Label {
            width: parent.width
            visible: !hostnameField.acceptableInput
            text: qsTr("Valid hostname or IP required!")
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        TextField {
            id: portField
            width: parent.width / 2

            label: qsTr("Port")

            text: App.api().serverConfig().internalPort

            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1; top: 65535;}

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: focus = false

            autoScrollEnabled: true

            onTextChanged: checkInput()
        }

        Label {
            width: parent.width
            visible: !portField.acceptableInput
            text: qsTr("Valid port required!") +  " (1-65535)"
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    function checkInput() {
        dialog.canAccept = hostnameField.acceptableInput && portField.acceptableInput
    }

    onAccepted: {
        App.api().getDiscoveryInfo(hostnameField.text, portField.text)
    }

    Component.onCompleted: checkInput()
}
