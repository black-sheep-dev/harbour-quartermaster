import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page
    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: page.width - 2 * x
            spacing: Theme.paddingLarge

            PageHeader {
                Row {
                    x: Theme.paddingMedium
                    width: parent.width - 2*x
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width / 2
                        text: qsTr("Back")

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }

                    Label {
                        width: parent.width / 2
                        text: qsTr("Continue")

                        horizontalAlignment: Text.AlignRight

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }

            Label {
                width: parent.width

                text: qsTr("Connection Settings")

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                text: qsTr("You need to provide the connection details to your Homeassistant instance.\n"
                           + "This can either be the hostname / IP in your local network or a remote url if your Homeassistant is reachable from the internet.")
                color: Theme.highlightColor
            }

            Label {
                width: parent.width
                text: qsTr("Enter connection details")

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
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

                autoScrollEnabled: true
            }

            TextField {
                id: portField
                width: parent.width / 2

                label: qsTr("Port")

                text: Client.port

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1; top: 65535;}

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: sslSwitch.focus = true

                autoScrollEnabled: true
            }

            TextSwitch {
                id: sslSwitch
                text: qsTr("Use SSL")
                description: qsTr("It is highly recommend to use a ssl connection if your homeasstiant server support this!")

                checked: Client.ssl
            }


        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("WizardTokenPage.qml"))
        } else if (status == PageStatus.Deactivating) {
            Client.hostname = hostnameField.text
            Client.port = portField.text
            Client.ssl = sslSwitch.checked
        }
    }
}
