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
                title: entity.name
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                text: qsTr("Switch on/off")
                checked: entity.state === "on"

                onClicked: {
                    Client.entitiesProvider().callService("switch",
                                                          checked ? "turn_on" : "turn_off",
                                                          entity.entityId)
                }
            }

            SectionHeader {
                text: qsTr("Info")
            }

            Row {
                visible: entity.attributes.current_power_w !== undefined

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Current power usage:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.current_power_w + " W"
                    color: Theme.highlightColor
                }
            }

            Row {
                visible: entity.attributes.today_energy_kwh !== undefined

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Today energy usage:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.today_energy_kwh + " kWh"
                    color: Theme.highlightColor
                }
            }

            Row {
                visible: entity.attributes.total_energy_kwh !== undefined

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Total energy usage:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.total_energy_kwh + " kWh"
                    color: Theme.highlightColor
                }
            }

            Row {
                visible: entity.attributes.voltage !== undefined

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Voltage:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.voltage + " V"
                    color: Theme.highlightColor
                }
            }

            Row {
                visible: entity.attributes.current_a !== undefined

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Current:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.current_a + " A"
                    color: Theme.highlightColor
                }
            }
        }
    }

    Component.onCompleted: if (Client.updateSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}

