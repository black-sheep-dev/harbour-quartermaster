import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardConnectionAdvancedPage.qml")

    canAccept: hostnameField.acceptableInput && portField.acceptableInput

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
                  + qsTr("Please provide a full url for this!")

            font.pixelSize: Theme.fontSizeSmall
            color: Theme.highlightColor
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap

            text: "<ul><li>http://server</li><li>http://192.168.168.2</li><li>https://home.example.org</li></ul>"

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

            label: qsTr("URL")
            placeholderText: qsTr("Enter url (e.g. http://server)")

            text: App.api().serverConfig().internalUrl

            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: portField.focus = true

            autoScrollEnabled: true
        }

        Label {
            width: parent.width
            visible: !hostnameField.acceptableInput
            text: qsTr("Valid url required!")
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
        }

        Label {
            width: parent.width
            visible: !portField.acceptableInput
            text: qsTr("Valid port required!") +  " (1-65535)"
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    onAccepted: {
        App.api().getDiscoveryInfo(hostnameField.text, portField.text)
    }
}
