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

            TextSwitch {
                x: Theme.horizontalPageMargin
                //% "Switch on/off"
                text: qsTrId("id-switch-on-off")
                checked: entity.state === "on"

                onClicked: {
                    App.entitiesService().callService("switch",
                                                      checked ? "turn_on" : "turn_off",
                                                      {
                                                          entity_id: entity.entityId
                                                      })
                }
            }

            SectionHeader {
                visible: entity.attributes !== undefined
                //% "Info"
                text: qsTrId("id-info")
            }

            DetailItem {
                visible: entity.attributes.current_power_w !== undefined
                //% "Current power usage"
                label: qsTrId("id-current-power-usage")
                value: entity.attributes.current_power_w + " W"
            }

            DetailItem {
                visible: entity.attributes.today_energy_kwh !== undefined
                //% "Today energy usage"
                label: qsTrId("id-today-energy-usage")
                value: entity.attributes.today_energy_kwh + " kWh"
            }

            DetailItem {
                visible: entity.attributes.today_energy_kwh !== undefined
                //% "Total energy usage"
                label: qsTrId("id-total-energy-usage")
                value: entity.attributes.total_energy_kwh + " kWh"
            }

            DetailItem {
                visible: entity.attributes.voltage !== undefined
                //% "Voltage"
                label: qsTrId("id-voltage")
                value: entity.attributes.voltage + " V"
            }

            DetailItem {
                visible: entity.attributes.voltage !== undefined
                //% "Current"
                label: qsTrId("id-current")
                value: entity.attributes.current_a + " A"
            }
        }
    }

    Connections {
        target: App.api()
        onRequestFinished: if (requestType === Api.RequestGetApiStatesEntity) busy = false
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}

