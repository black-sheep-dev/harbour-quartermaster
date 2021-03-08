import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog
    allowedOrientations: Orientation.Portrait
    acceptDestination: Qt.resolvedUrl("WizardInfoPage.qml")

    DialogHeader {
        id: header
        acceptText: qsTr("Connect")
        cancelText: qsTr("Back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingSmall

        Label {
            width: parent.width

            text: qsTr("Access Token")

            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        Label {
            width: parent.width
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall

            text: qsTr("You need to provide a valid Long-Lived Access Token.\n"
                       + "You can create one on profile page in your Home Assistant webfrontend and copy the token string into the input field below.")
            color: Theme.highlightColor
        }

        Item {
            width: 1
            height: Theme.paddingLarge
        }

        Label {
            width: parent.width
            text: qsTr("Enter Long-Lived Access Token")

            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        TextArea {
            id: tokenField
            width: parent.width

            label: qsTr("Token")
            placeholderText: qsTr("Enter token")

            text: App.wallet().token

            onTextChanged: checkInput()
        }
    }

    function checkInput() { canAccept = tokenField.text.length > 0 }

    onAccepted: {
        App.wallet().token = tokenField.text
        App.api().connectToHost()
    }

    Component.onCompleted: checkInput()
}
