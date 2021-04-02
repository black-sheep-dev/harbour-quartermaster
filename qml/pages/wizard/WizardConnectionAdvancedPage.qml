import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    property bool busy: true
    property bool discovered: false

    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardTokenPage.qml")

    canAccept: internalUriField.acceptableInput

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

                text: discovered ? qsTr("Server connection info received. Please check the data!") :
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
                    MenuItem {
                        enabled: externalUriField.acceptableInput
                        text: qsTr("External Url")
                    }
                    MenuItem {
                        enabled: internalUriField.acceptableInput
                        text: qsTr("Internal Url")
                    }
                    //MenuItem { text: qsTr("Cloud") }
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

                id: internalUriField
                width: parent.width

                label: qsTr("URI")
                placeholderText: qsTr("Enter URI (e.g. http://server:8123)")

                text: App.api().serverConfig().internalUrl

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: internalUriField.focus = true

                autoScrollEnabled: true

            }

            Label {
                width: parent.width
                visible: discovered && !internalUriField.acceptableInput
                text: qsTr("Valid URI required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            SectionHeader {
                visible: discovered
                text: qsTr("External Connection")
            }

            TextField {
                visible: discovered

                id: externalUriField
                width: parent.width

                label: qsTr("URI")
                placeholderText: qsTr("Enter URI (e.g. http://server:8123)")

                text: App.api().serverConfig().externalUrl

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
                visible: discovered && !externalUriField.acceptableInput
                text: qsTr("Valid URI required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        VerticalScrollDecorator {}
    }

    onAccepted: {
        App.api().serverConfig().externalUrl = externalUriField.text
        App.api().serverConfig().internalUrl = internalUriField.text
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
