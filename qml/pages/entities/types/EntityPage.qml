import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../../components"

Page {
    property bool busy: false
    property Entity entity

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: page.busy
            MenuItem {
                text: qsTr("Attributes")
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity })
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    page.busy = true;
                    App.entitiesService().getEntityState(entity.entityId)
                }
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            spacing: Theme.paddingMedium

            PageHeader {
                title: entity.name
            }

            SectionHeader {
                text: qsTr("Features")
            }
        }
    }

    Connections {
        target: App.api()
        onRequestFinished: if (requestType === Api.RequestGetApiStatesEntity) busy = false
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}
