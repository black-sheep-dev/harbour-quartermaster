import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardConnectionAdvancedPage.qml")

    canAccept: urlField.acceptableInput

    DialogHeader {
        id: header
        //% "Connect"
        acceptText: qsTrId("id-connect")
        //% "Back"
        cancelText: qsTrId("id-back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x

        spacing: Theme.paddingSmall

        Label {
            width: parent.width

            //% "Connection settings"
            text: qsTrId("id-connection-settings")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap

            //% "You need to provide the connection details to your Home Assistant server."
            //% "Please provide a full URL with port for this!"
            text: qsTrId("id-wizard-connection-desc")


            font.pixelSize: Theme.fontSizeSmall
            color: Theme.highlightColor
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap

            text: "<ul><li>http://server:8123</li><li>http://192.168.168.2:8123</li><li>https://home.example.org:8123</li></ul>"

            font.pixelSize: Theme.fontSizeSmall
            color: Theme.highlightColor
        }

        Item {
            width: 1
            height: Theme.paddingLarge
        }

        TextField {
            id: urlField
            width: parent.width

            //% "URL"
            label: qsTrId("id-url")
            //% "Enter URL (e.g. http://server:8123)"
            placeholderText: qsTrId("id-enter-url-placeholder")

            text: App.api().serverConfig().internalUrl

            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: focus = false

            autoScrollEnabled: true
        }

        Label {
            width: parent.width
            visible: !urlField.acceptableInput
            //% "Valid URL required!"
            text: qsTrId("id-error-valid-url")
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    onAccepted: {
        App.api().getDiscoveryInfo(urlField.text)
    }
}
