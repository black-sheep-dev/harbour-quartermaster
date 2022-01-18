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
                //% "Automation"
                title: qsTrId("id-automation")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                text: entity.name
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
            }

            SectionHeader {
                //% "Features"
                text: qsTrId("id-features")
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                //% "Automation on/off"
                text: qsTrId("id-automation-on-off")
                checked: entity.state === "on"

                onClicked: {
                    App.entitiesService().callService("homeassistant",
                                                      checked ? "turn_on" : "turn_off",
                                                      {
                                                          entity_id: entity.entityId
                                                      })
                }
            }

            SectionHeader{
                //% "Info"
                text: qsTrId("id-info")
            }

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    //% "Last triggered"
                    text: qsTrId("id-last-triggered") + ":"
                    width: parent.width * 0.5
                    color: Theme.highlightColor
                }

                Label {
                    text: new Date(entity.attributes.last_triggered).toLocaleString(Qt.locale(), Locale.ShortFormat);
                    color: Theme.highlightColor
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


