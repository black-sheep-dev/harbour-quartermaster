import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    function applyChanges() {
        if (externalUriField.acceptableInput) {
            App.api().serverConfig().externalUrl = externalUriField.text
        }

        if (internalUriField.acceptableInput) {
            App.api().serverConfig().internalUrl = internalUriField.text
        }

        App.saveSettings();
    }

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Connections")
            }

            ComboBox {
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
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
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
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                visible: discovered && !externalUriField.acceptableInput
                text: qsTr("Valid URI required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status == PageStatus.Deactivating) page.applyChanges();
}

