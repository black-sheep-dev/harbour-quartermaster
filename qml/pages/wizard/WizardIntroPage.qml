import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog

    allowedOrientations: Orientation.Portrait

    acceptDestination: Qt.resolvedUrl(Qt.resolvedUrl("WizardConnectionPage.qml"))

    onRejected: Qt.quit()

    DialogHeader {
        id: header
        acceptText: qsTr("Continue")
        cancelText: ""
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x
        spacing: Theme.paddingMedium

        Image {
            id: logo
            source: "/usr/share/icons/hicolor/512x512/apps/harbour-quartermaster.png"
            smooth: true
            height: parent.width / 2
            width: parent.width / 2
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item {
            width: 1
            height: Theme.paddingMedium
        }

        Label {
            width: parent.width

            text: qsTr("Welcome to Quartermaster")

            color: Theme.secondaryHighlightColor
            font.bold: true
            font.pixelSize: Theme.fontSizeLarge
        }

        Item {
            width: 1
            height: Theme.paddingMedium
        }

        Label {
            width: parent.width

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall

            wrapMode: Text.WordWrap

            text: qsTr("This device is not connected to a Home Assistant server yet.")
        }

        Label {
            width: parent.width

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall

            wrapMode: Text.WordWrap

            text: qsTr("This setup wizard will lead you through the connection process.")
        }

        Item {
            width: 1
            height: Theme.paddingMedium
        }

        Separator {
            width: parent.width
            color: Theme.highlightBackgroundColor
        }

        Item {
            width: 1
            height: Theme.paddingMedium
        }

        Label {
            width: parent.width

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall

            wrapMode: Text.WordWrap

            text: qsTr("If you facing problems with the setup process you can try to reset the data from a previous install.")
        }

        Item {
            width: 1
            height: Theme.paddingMedium
        }

        ButtonLayout {
            Button {
                text: qsTr("Reset")
                onClicked: App.wallet().reset()
            }
        }
    }
}
