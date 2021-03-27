import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    function applyChanges() {
        if (externalHostnameField.acceptableInput && externalPortField.acceptableInput) {
            App.api().serverConfig().externalUrl = externalHostnameField.text
            App.api().serverConfig().externalPort = externalPortField.text
        }

        if (internalHostnameField.acceptableInput && internalPortField.acceptableInput) {
            App.api().serverConfig().internalUrl = internalHostnameField.text
            App.api().serverConfig().internalPort = internalPortField.text
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

            SectionHeader {
                text: qsTr("Internal Connection")
            }

            TextField {
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
            }

            Label {
                width: parent.width
                visible: !internalHostnameField.acceptableInput
                text: qsTr("Valid hostname or IP required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            TextField {
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
                visible: !internalPortField.acceptableInput
                text: qsTr("Valid port required!") +  " (1-65535)"
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            SectionHeader {
                text: qsTr("External Connection")
            }

            TextField {
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
            }

            Label {
                width: parent.width
                visible: !externalHostnameField.acceptableInput
                text: qsTr("Valid hostname or IP required!")
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            TextField {
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
                visible: !externalPortField.acceptableInput
                text: qsTr("Valid port required!") +  " (1-65535)"
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: if (status == PageStatus.Deactivating) page.applyChanges();
}

