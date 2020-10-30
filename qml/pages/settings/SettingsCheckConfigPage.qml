import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.Portrait

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: Client.homeassistantInfo().loading
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Home Assistant Config")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("This page shows the status of the configuration files of your Home Assistant server.")
            }

            Label {
                visible: (Client.homeassistantInfo().components & HomeassistantInfo.ComponentConfig) !== HomeassistantInfo.ComponentConfig

                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("The config component is not activated on Home Assistant server.")
            }

            Image {
                visible: !Client.homeassistantInfo().loading
                         && (Client.homeassistantInfo().components & HomeassistantInfo.ComponentConfig) === HomeassistantInfo.ComponentConfig

                id: checkImage
                source: {
                    if (Client.homeassistantInfo().configValid)
                        return "image://theme/icon-l-acknowledge";
                    else
                        return "image://theme/icon-l-attention";
                }

                anchors.horizontalCenter: parent.horizontalCenter

            }

            SectionHeader {
                visible: !Client.homeassistantInfo().configValid
                text: qsTr("Errors")
            }

            Label {
                visible: !Client.homeassistantInfo().configValid

                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: Client.homeassistantInfo().configError
            }
        }
    }
}
