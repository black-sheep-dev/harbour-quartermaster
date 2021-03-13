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
                text: qsTr("Attributes")
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity })
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    page.busy = true;
                    App.api().getEntityState(entity.entityId)
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
                title: qsTr("Automation")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                text: entity.name
                wrapMode: Text.WordWrap

                color: Theme.highlightColor
            }

            SectionHeader {
                text: qsTr("Features")
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                text: qsTr("Automation on/off")
                checked: entity.state === "on"

                onClicked: {
                    App.api().callService("homeassistant",
                                          checked ? "turn_on" : "turn_off",
                                          {
                                              entity_id: entity.entityId
                                          })
                }
            }

            SectionHeader{
                text: qsTr("Info")
            }

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Last triggered:")
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


