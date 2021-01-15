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
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                Row {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width / 2
                        text: qsTr("Back")

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }

                    Label {
                        id: continueLable
                        width: parent.width / 2
                        text: qsTr("Continue")

                        horizontalAlignment: Text.AlignRight

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x

                text: qsTr("Connection Settings")

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                wrapMode: Text.WordWrap

                text: qsTr("You need to provide the connection details to your Home Assistant server.\n"
                           + "This can either be the hostname / IP in your local network or a remote url if your Home Assistant is reachable from the internet.")
                color: Theme.highlightColor
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
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
                validator: RegExpValidator {
                    regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.?[a-z]{2,8}(:[0-9]{1,5})?(\/.*)?$|^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$|[a-zA-Z0-9-_]{1,}/gm
                }

                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: portField.focus = true

                autoScrollEnabled: true

                onTextChanged: checkInput()
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                visible: !hostnameField.acceptableInput
                text: qsTr("Valid hostname or IP required!")
                color: Theme.errorColor
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
                EnterKey.onClicked: sslSwitch.focus = true

                autoScrollEnabled: true

                onTextChanged: checkInput()
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                visible: !portField.acceptableInput
                text: qsTr("Valid port required!") +  " (1-65535)"
                color: Theme.errorColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            TextSwitch {
                id: sslSwitch
                text: qsTr("Use SSL")
                description: qsTr("It is highly recommend to use a ssl connection if your Home Assistant server support this!")

                checked: Client.ssl
            }
        }

        VerticalScrollDecorator {}
    }

    function checkInput() {
        if (hostnameField.acceptableInput && portField.acceptableInput) {
            canNavigateForward = true
            continueLable.visible = true
        } else {
            canNavigateForward = false
            continueLable.visible = false
        }
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

    Component.onCompleted: checkInput()
}
