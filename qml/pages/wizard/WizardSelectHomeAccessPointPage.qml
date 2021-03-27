import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Dialog {
    property bool busy: true
    property string identifier
    property string name

    id: dialog
    allowedOrientations: Orientation.Portrait

    acceptDestination: Qt.resolvedUrl("WizardLastPage.qml")

    canAccept: false

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: busy
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    DialogHeader {
        id: header
        acceptText: qsTr("Select")
        cancelText: qsTr("Back")
    }

    SilicaListView {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        width: parent.width

        model: App.locationService().availableAccessPointsModel()

        delegate: ListItem {
            height: Theme.itemSizeMedium
            width: parent.width

            contentHeight: Theme.itemSizeMedium

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height

                anchors.verticalCenter: parent.verticalCenter

                spacing: Theme.paddingMedium

                Image {
                    id: wlanIcon
                    source: "image://theme/icon-m-wlan"

                    anchors.verticalCenter: parent.verticalCenter
                }

                Column {
                    width: parent.width - wlanIcon.width - selectedIcon.width - 2 * Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width
                        text: name.length > 0 ? name : qsTr("Hidden network")
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeMedium
                    }

                    Label {
                        width: parent.width
                        text: qsTr("Available network")

                        color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }

                Image {
                    id: selectedIcon

                    visible: dialog.identifier === model.identifier

                    source: "image://theme/icon-m-acknowledge"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            onClicked: {
                dialog.identifier = model.identifier
                dialog.name = model.name
                canAccept = true
            }
        }

        VerticalScrollDecorator {}
    }

    Connections {
        target: App.api()
        onRequestFinished: {
            if (requestType !== Api.RequestWebhookGetZones) return;

            busy = false
        }
        onRequestError: {
            if (requestType !== Api.RequestPostApiRegisterDevice) return;

            busy = false
        }
    }

    onAccepted: {
        App.locationService().addAccessPointToZone(
                    App.locationService().homezone().guid,
                    dialog.identifier,
                    dialog.name)
    }
}
