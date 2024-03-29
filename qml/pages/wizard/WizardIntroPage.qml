import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    id: dialog

    allowedOrientations: Orientation.Portrait

    acceptDestination: Qt.resolvedUrl(Qt.resolvedUrl("WizardConnectionPage.qml"))

    onRejected: Qt.quit()

    DialogHeader {
        id: header
        //% "Continue"
        acceptText: qsTrId("id-continue")
        cancelText: ""
    }

    RemorsePopup { id: resetPopup }

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

            //% "Welcome to Quartermaster"
            text: qsTrId("id-welcome-title")

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

            //% "This device is not yet connected to a Home Assistant server."
            text: qsTrId("id-welcome-info-1")
        }

        Label {
            width: parent.width

            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall

            wrapMode: Text.WordWrap

            //% "The setup wizard will guide you through the connection process."
            text: qsTrId("id-welcome-info-2")
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

            //% "If there are errors during the setup process, you can try to reset the data from a previous installation."
            text: qsTrId("id-reset-app-desc")
        }

        Item {
            width: 1
            height: Theme.paddingMedium
        }

        ButtonLayout {
            Button {
                //% "Reset"
                text: qsTrId("id-reset")
                //% "Resetting application"
                onClicked: resetPopup.execute(qsTrId("id-resetting-app"), function() {
                    App.reset()
                })
            }
        }
    }
}
