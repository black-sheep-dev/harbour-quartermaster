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

        PullDownMenu {
            MenuItem {
                text: qsTr("Reset")
                onClicked: resetPopup.execute(qsTr("Resetting"), function() { Client.reset() })
            }
        }

        RemorsePopup { id: resetPopup }

        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingLarge

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

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: portField.focus = true
            }

            TextSwitch {
                id: sslSwitch
                text: qsTr("Use SSL")
                description: qsTr("It is highly recommend to use a ssl connection if your homeasstiant server support this!")

                checked: Client.ssl
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
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        page.applyChanges()
    }
}

