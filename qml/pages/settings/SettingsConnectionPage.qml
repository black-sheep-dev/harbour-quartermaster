import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    function applyChanges() {
        if (externalUrlField.acceptableInput) {
            App.api().serverConfig().externalUrl = externalUrlField.text
        }

        if (internalUrlField.acceptableInput) {
            App.api().serverConfig().internalUrl = internalUrlField.text
        }

        App.api().connectionMode = modeComboBox.currentIndex

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
                id: modeComboBox
                width: parent.width
                label: qsTr("Preffered Mode")

                menu: ContextMenu {
                    MenuItem { text: qsTr("Automatic") }
                    MenuItem { text: qsTr("External URL") }
                    MenuItem { text: qsTr("Internal URL") }
                    MenuItem { text: qsTr("Cloud") }
                }

                Component.onCompleted: currentIndex = App.api().connectionMode
            }

            SectionHeader {
                text: qsTr("Internal Connection")
            }

            TextField {
                id: internalUrlField
                width: parent.width

                label: qsTr("URL")
                placeholderText: qsTr("Enter URL (e.g. http://server:8123)")

                text: App.api().serverConfig().internalUrl

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http(s?):\/\/(www\.)?)[a-zA-Z0-9]+([\-\.]{1}[a-zA-Z0-9]+)*(\(.[a-zA-Z]{2,5})?(:[0-9]{1,5})?(\/.*)?$/g
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: internalUrlField.focus = true

                autoScrollEnabled: true

            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                visible: !internalUrlField.acceptableInput
                text: qsTr("Valid URL required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            SectionHeader {
                text: qsTr("External Connection")
            }

            TextField {
                id: externalUrlField
                width: parent.width

                label: qsTr("URL")
                placeholderText: qsTr("Enter URL (e.g. http://server:8123)")

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
                visible: !externalUrlField.acceptableInput
                text: qsTr("Valid URI required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status == PageStatus.Deactivating) page.applyChanges();
}

