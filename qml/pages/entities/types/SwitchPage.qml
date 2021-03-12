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
                //onClicked: Client.entitiesProvider().updateEntity(entity.entityId)
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
                text: qsTr("Features")
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                text: qsTr("Switch on/off")
                checked: entity.state === "on"

                onClicked: {
                    App.api().callService("switch",
                                          checked ? "turn_on" : "turn_off",
                                          {
                                              entity_id: entity.entityId
                                          })
                }
            }

            SectionHeader {
                text: qsTr("Info")
            }

            DetailItem {
                visible: entity.attributes.current_power_w !== undefined
                label: qsTr("Current power usage")
                value: entity.attributes.current_power_w + " W"
            }

            DetailItem {
                visible: entity.attributes.today_energy_kwh !== undefined
                label: qsTr("Today energy usage")
                value: entity.attributes.today_energy_kwh + " kWh"
            }

            DetailItem {
                visible: entity.attributes.today_energy_kwh !== undefined
                label: qsTr("Total energy usage")
                value: entity.attributes.total_energy_kwh + " kWh"
            }

            DetailItem {
                visible: entity.attributes.voltage !== undefined
                label: qsTr("Voltage")
                value: entity.attributes.voltage + " V"
            }

            DetailItem {
                visible: entity.attributes.voltage !== undefined
                label: qsTr("Current")
                value: entity.attributes.current_a + " A"
            }
        }
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}

