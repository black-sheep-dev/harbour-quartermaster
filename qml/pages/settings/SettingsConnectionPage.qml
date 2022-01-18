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
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Connections"
                title: qsTrId("id-connections")
            }

            ComboBox {
                id: modeComboBox
                width: parent.width
                //% "Preffered mode"
                label: qsTrId("id-preffered-mode")

                menu: ContextMenu {
                    //% "Automatic"
                    MenuItem { text: qsTrId("id-automatic") }
                    //% "External URL"
                    MenuItem { text: qsTrId("id-external-url") }
                    //% "Internal URL"
                    MenuItem { text: qsTrId("id-internal-url") }
                    //% "Cloud"
                    MenuItem { text: qsTrId("id-cloud") }
                }

                Component.onCompleted: currentIndex = App.api().connectionMode
            }

            SectionHeader {
                //% "Internal connection"
                text: qsTrId("id-internal-connection")
            }

            TextField {
                id: internalUrlField
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
                EnterKey.onClicked: internalUrlField.focus = true

                autoScrollEnabled: true

            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                visible: !internalUrlField.acceptableInput
                //% "Valid URL required!"
                text: qsTrId("id-error-valid-url")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            SectionHeader {
                //% "External connection"
                text: qsTrId("id-external-connection")
            }

            TextField {
                id: externalUrlField
                width: parent.width

                //% "URL"
                label: qsTrId("id-url")
                //% "Enter URL (e.g. http://server:8123)"
                placeholderText: qsTrId("id-enter-url-placeholder")

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
                //% "Valid URL required!"
                text: qsTrId("id-error-valid-url")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status == PageStatus.Deactivating) page.applyChanges();
}

