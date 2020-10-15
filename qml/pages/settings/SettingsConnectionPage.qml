import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    function applyChanges() {
        Client.hostname = hostnameField.text;
        Client.ssl = sslSwitch.checked;
        Client.port = portField.text;

        Client.saveSettings();
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
                title: qsTr("Connection")
            }

            TextField {
                id: hostnameField
                width: parent.width

                label: qsTr("Hostname")
                placeholderText: qsTr("Enter hostname")

                text: Client.hostname

                inputMethodHints: Qt.ImhUrlCharactersOnly
                validator: RegExpValidator {
                    regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$|^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$|[a-zA-Z0-9-_]{1,}/gm
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: portField.focus = true
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                visible: !hostnameField.acceptableInput
                text: qsTr("Valid hostname or IP required!")
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            TextField {
                id: portField
                width: parent.width / 2

                label: qsTr("Port")

                text: Client.port

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1; top: 65535;}

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: focus = false
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                visible: !portField.acceptableInput
                text: qsTr("Valid port required!") +  " (1-65535)"
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            TextSwitch {
                id: sslSwitch
                text: qsTr("Use SSL")
                description: qsTr("It is highly recommend to use a ssl connection if your homeassistant server supports it!")

                checked: Client.ssl
            }
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        if (hostnameField.acceptableInput && portField.acceptableInput) page.applyChanges()
    }
}

