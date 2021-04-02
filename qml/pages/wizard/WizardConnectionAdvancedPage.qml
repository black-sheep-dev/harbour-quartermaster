import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    property bool busy: true
    property bool discovered: false

    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardTokenPage.qml")

    canAccept: internalHostnameField.acceptableInput && internalPortField.acceptableInput

    PageBusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: busy
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + header.height

        DialogHeader {
            id: header
            acceptText: qsTr("Continue")
            cancelText: qsTr("Back")
        }

        Column {
            id: column
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

                text: discovered ? qsTr("Server connection infos received. Please check external port!") :
                          qsTr("Failed to connect to server. Go back and check your data!")

                font.pixelSize: Theme.fontSizeSmall
                color: Theme.highlightColor
            }

            ComboBox {
                visible: !busy && discovered
                width: parent.width
                label: qsTr("Preffered Mode")

                menu: ContextMenu {
                    MenuItem { text: qsTr("Automatic") }
                    MenuItem { text: qsTr("External Url") }
                    MenuItem { text: qsTr("Internal Url") }
                    MenuItem { text: qsTr("Cloud") }
                }

                Component.onCompleted: currentIndex = App.api().connectionMode
                onCurrentIndexChanged: App.api().connectionMode = currentIndex
            }

            SectionHeader {
                visible: discovered
                text: qsTr("Internal Connection")
            }

            TextField {
                visible: discovered

                id: internalHostnameField
                width: parent.width

                label: qsTr("URL")
                placeholderText: qsTr("Enter url")

                text: App.api().serverConfig().internalUrl

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: internalPortField.focus = true

                autoScrollEnabled: true

            }

            Label {
                width: parent.width
                visible: discovered && !internalHostnameField.acceptableInput
                text: qsTr("Valid url required!")
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

                label: qsTr("URL")
                placeholderText: qsTr("Enter url")

                text: App.api().serverConfig().externalUrl

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: externalPortField.focus = true

                autoScrollEnabled: true
            }

            Label {
                width: parent.width
                visible: discovered && !externalHostnameField.acceptableInput
                text: qsTr("Valid url required!")
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
            }

            Label {
                width: parent.width
                visible: discovered && !externalPortField.acceptableInput
                text: qsTr("Valid port required!") +  " (1-65535)"
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        VerticalScrollDecorator {}
    }

    onAccepted: {
        App.api().serverConfig().externalUrl = externalHostnameField.text
        App.api().serverConfig().externalPort = externalPortField.text
        App.api().serverConfig().internalUrl = internalHostnameField.text
        App.api().serverConfig().internalPort = internalPortField.text      
        App.saveSettings()
    }

    Connections {
        target: App.api()
        onRequestFinished: {
            if (requestType !== Api.RequestGetApiDiscoveryInfo) return;

            busy = false
            dialog.discovered = true
        }
        onRequestError: {
            if (requestType !== Api.RequestGetApiDiscoveryInfo) return;

            busy = false
            dialog.discovered = false
        }
    }
}
