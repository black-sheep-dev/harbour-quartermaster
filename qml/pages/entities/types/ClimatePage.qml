import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property Climate entity

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

            SectionHeader {
                text: qsTr("Features")
            }

            Slider {
                id: temperatureSlider

                label: qsTr("Target temperature")

                visible: entity.hasFeature(Climate.ClimateTemperature)

                width: parent.width
                stepSize: {
                    if (entity.attributes.target_temperature_step === undefined)
                        return 0.1

                    return entity.attributes.target_temperature_step
                }

                minimumValue: entity.attributes.min_temp
                maximumValue: entity.attributes.max_temp

                value: entity.attributes.temperature
                valueText: value + " °C"

                onPressedChanged: {
                    if (pressed)
                        return

                    App.api().callService("climate",
                                          "set_temperature",
                                          {
                                              entity_id: entity.entityId,
                                              temperature: value
                                          })

                    entity.attributes.temperature = value
                }
            }

            ComboBox {
                visible: entity.presetModes > 0
                width: parent.width
                label: qsTr("Preset mode")

                menu: ContextMenu {

                    MenuItem {
                        text: qsTr("none")
                    }

                    Repeater {
                        model: entity.attributes.preset_modes

                        MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    const mode = currentItem.text

                    App.api().callService("climate",
                                          "set_preset_mode",
                                          {
                                            entity_id: entity.entityId,
                                            preset_mode: mode
                                          })

                    entity.attributes.preset_mode = mode
                }
            }

            SectionHeader {
                text: qsTr("Info")
            }

            DetailItem {
                label: qsTr("Current state")
                value: entity.state
            }

            DetailItem {
                visible: entity.hasFeature(Climate.ClimateTemperature)
                label: qsTr("Current room temperature")
                value: entity.attributes.current_temperature + " " + App.api().serverConfig().unitTemperature
            }
        }
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}

