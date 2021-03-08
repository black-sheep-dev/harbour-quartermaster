import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    property bool busy: true
    property bool discovered: false

    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardTokenPage.qml")

    PageBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: busy
    }

    DialogHeader {
        id: header
        acceptText: qsTr("Continue")
        cancelText: qsTr("Back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x

        spacing: Theme.paddingSmall

        opacity: busy ? 0.2 : 1

        Behavior on opacity {
            FadeAnimation {}
        }
        Label {
            visible: !busy

            width: parent.width

            text: qsTr("Connection Infos")

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            visible: !busy

            width: parent.width
            wrapMode: Text.WordWrap

            text: discovered ? qsTr("Server connection infos recieved. Please check port for external address!") :
                      qsTr("Failed to connect to server. Go back and check your data!")

            font.pixelSize: Theme.fontSizeSmall
            color: Theme.highlightColor
        }

        SectionHeader {
            visible: discovered
            text: qsTr("Internal Connection")
        }

        TextField {
            visible: discovered

            id: internalHostnameField
            width: parent.width

            label: qsTr("Hostname")
            placeholderText: qsTr("Enter hostname")

            text: App.api().serverConfig().internalUrl

            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.?[a-z]{2,8}(:[0-9]{1,5})?(\/.*)?$|^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$|[a-zA-Z0-9-_]{1,}/gm
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: internalPortField.focus = true

            autoScrollEnabled: true

            onTextChanged: checkInput()
        }

        Label {
            width: parent.width
            visible: discovered && !internalHostnameField.acceptableInput
            text: qsTr("Valid hostname or IP required!")
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        TextField {
            visible: discovered

            id: internalPortField
            width: parent.width / 2

            label: qsTr("Port")

            text: App.api().serverConfig().internalPort

            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1; top: 65535;}

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: externalHostnameField.focus = true

            autoScrollEnabled: true

            onTextChanged: checkInput()
        }

        Label {
            width: parent.width
            visible: discovered && !internalPortField.acceptableInput
            text: qsTr("Valid port required!") +  " (1-65535)"
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        SectionHeader {
            visible: discovered
            text: qsTr("External Connection")
        }

        TextField {
            visible: discovered

            id: externalHostnameField
            width: parent.width

            label: qsTr("Hostname")
            placeholderText: qsTr("Enter hostname")

            text: App.api().serverConfig().externalUrl

            inputMethodHints: Qt.ImhUrlCharactersOnly
            validator: RegExpValidator {
                regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.?[a-z]{2,8}(:[0-9]{1,5})?(\/.*)?$|^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$|[a-zA-Z0-9-_]{1,}/gm
            }

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: externalPortField.focus = true

            autoScrollEnabled: true

            onTextChanged: checkInput()
        }

        Label {
            width: parent.width
            visible: discovered && !externalHostnameField.acceptableInput
            text: qsTr("Valid hostname or IP required!")
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        TextField {
            visible: discovered

            id: externalPortField
            width: parent.width / 2

            label: qsTr("Port")

            text: App.api().serverConfig().externalPort

            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1; top: 65535;}

            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: focus = false

            autoScrollEnabled: true

            onTextChanged: checkInput()
        }

        Label {
            width: parent.width
            visible: discovered && !externalPortField.acceptableInput
            text: qsTr("Valid port required!") +  " (1-65535)"
            color: Theme.errorColor
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    function checkInput() {
        canAccept = internalHostnameField.acceptableInput && internalPortField.acceptableInput
    }

    onAccepted: {
        App.api().serverConfig().internalUrl = internalHostnameField.text
        App.api().serverConfig().internalPort = internalPortField.text
        App.api().serverConfig().externalUrl = internalHostnameField.text
        App.api().serverConfig().externalPort = internalPortField.text
    }

    Connections {
        target: App.api()
        onHostDiscovered: {
            busy = false
            dialog.discovered = discovered
        }
    }

    Component.onCompleted: checkInput()
}
