import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Dialog {
    property Zone zone

    id: selectNetworkDialog

    anchors.fill: parent

    DialogHeader {
        id: header
        title: qsTr("Select Wifi networks")
        reserveExtraContent: true
    }

    IconButton {
        icon.source: "image://theme/icon-m-refresh"
        anchors.right: header.right
        anchors.bottom: header.bottom

        onClicked: Client.updateNetworksModel();
    }

    PageBusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Large
        running: Client.networksModel().loading
        anchors.centerIn: parent
    }

    SilicaListView {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        width: parent.width

        model: Client.networksModel()

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
                    source: {
                        if (!defined)
                            return "image://theme/icon-m-wlan"

                        return discovered ? "image://theme/icon-m-wlan" : "image://theme/icon-m-wlan-no-signal"
                    }

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
                        text: defined ? qsTr("Stored network") : qsTr("Available network")

                        color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }

                Image {
                    id: selectedIcon

                    visible: selected

                    source: "image://theme/icon-m-acknowledge"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            onClicked: selected = !selected
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: Client.updateNetworksModel()
}
