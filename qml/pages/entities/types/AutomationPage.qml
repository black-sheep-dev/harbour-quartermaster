import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property Entity entity

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {    
            MenuItem {
                text: qsTr("Attributes")
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity })
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: Client.entitiesProvider().updateEntity(entity.entityId)
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

            TextSwitch {
                x: Theme.horizontalPageMargin
                text: qsTr("Automation on/off")
                checked: entity.state === "on"

                onClicked: {
                    Client.entitiesProvider().callService("homeassistant",
                                                          checked ? "turn_on" : "turn_off",
                                                          entity.entityId)
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

    Component.onCompleted: if (Client.updateSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}


