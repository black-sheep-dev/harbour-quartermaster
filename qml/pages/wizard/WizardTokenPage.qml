import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardInfoPage.qml")

    DialogHeader {
        id: header
        //% "Connect"
        acceptText: qsTrId("id-connect")
        //% "Back"
        cancelText: qsTrId("id-back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingSmall

        Label {
            width: parent.width

            //% "Access token"
            text: qsTrId("id-access-token")

            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall

            //% "You need to provide a valid Long-Lived Access Token."
            //% "You can create one on profile page in your Home Assistant webfrontend and copy the token string into the input field below."
            text: qsTrId("id-access-token-desc")
            color: Theme.highlightColor
        }

        Item {
            width: 1
            height: Theme.paddingLarge
        }

        Label {
            width: parent.width
            //% "Enter Long-Lived Access Token"
            text: qsTrId("id-enter-token-desc")

            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        TextArea {
            id: tokenField
            width: parent.width

            //% "Token"
            label: qsTrId("id-token")
            //% "Enter token"
            placeholderText: qsTrId("id-enter-token")

            text: App.deviceService().token

            onTextChanged: checkInput()
        }
    }

    function checkInput() { canAccept = tokenField.text.length > 0 }

    onAccepted: {
        App.deviceService().token = tokenField.text
        App.api().initialize()
    }

    Component.onCompleted: checkInput()
}
