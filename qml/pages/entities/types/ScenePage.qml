import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy: false
    property Entity entity


    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: page.busy
            MenuItem {
                //% "Attributes"
                text: qsTrId("id-attributes")
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity })
            }
            MenuItem {
                //% "Refresh"
                text: qsTrId("id-refresh")
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

            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: entity.name
            }

            SectionHeader {
                //% "Features"
                text: qsTrId("id-features")
            }

            Button {
                x: Theme.horizontalPageMargin
                //% "Activate"
                text: qsTrId("id-scene-activate")

                onClicked: {
                    App.entitiesService().callService("scene",
                                                      "turn_on",
                                                      {
                                                          entity_id: entity.entityId
                                                      })
                }
            }
        }
    }

    Connections {
        target: App.api()
        onRequestFinished: if (requestType === Api.RequestGetApiStatesEntity) busy = false
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}

