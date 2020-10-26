import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            spacing: Theme.paddingLarge

            PageHeader {
                Row {
                    x: Theme.paddingMedium
                    width: parent.width - 2*x
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width / 2
                        text: ""

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

                text: qsTr("Welcome to Quartermaster")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            Image {
                id: logo
                source: "/usr/share/icons/hicolor/512x512/apps/harbour-quartermaster.png"
                smooth: true
                height: parent.width / 2
                width: parent.width / 2
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                width: parent.width

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                wrapMode: Text.WordWrap

                text: qsTr("This device is not connected to a Home Assistant server yet.")
            }

            Label {
                width: parent.width

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                wrapMode: Text.WordWrap

                text: qsTr("This setup wizard will lead you through the connection process.")
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("WizardConnectionPage.qml"))
        }
    }
}
