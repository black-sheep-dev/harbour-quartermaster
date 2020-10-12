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
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity})
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
        }
    }
}


