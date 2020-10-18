import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x

            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Automatic Updates")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeMedium

                text: qsTr("Here you can activate / deactivate automatic updates for your entities.")
                      + "\n"
                      + qsTr("Activating these options leads to higher battery consumption and network traffic.");
            }

            TextSwitch {
                id: entityUpdateSwitch
                text: qsTr("Single entity")
                description: qsTr("When active the entity will be updated when his property page is shown.")

                onCheckedChanged: Client.updateSingleEntity = checked
                Component.onCompleted: checked = Client.updateSingleEntity
            }

            TextSwitch {
                id: modelUpdateSwitch
                text: qsTr("Entity list")
                description: qsTr("When active all entities of one type will be updated when the corresponding list view is shown.")

                onCheckedChanged: Client.updateEntityModel = checked
                Component.onCompleted: checked = Client.updateEntityModel
            }

        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) Client.saveSettings()
}
