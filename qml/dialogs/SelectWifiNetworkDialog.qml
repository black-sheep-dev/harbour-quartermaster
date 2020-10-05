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
                    source: "image://theme/icon-m-wlan"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    width: parent.width - wlanIcon.width - selectedIcon.width - 2 * Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter

                    text: name
                    color: pressed ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeMedium
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

    onAccepted: {
        Client.networksModel().addSelectedToModel(zone.networksModel())
        Client.saveZonesSettings();
    }
}
