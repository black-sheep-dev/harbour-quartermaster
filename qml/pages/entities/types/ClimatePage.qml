import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy: false
    property Climate entity

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

            Slider {
                id: temperatureSlider

                //% "Target temperature"
                label: qsTrId("id-target-temperature")

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

                    App.entitiesService().callService("climate",
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
                //% "Preset mode"
                label: qsTrId("id-preset-mode")

                menu: ContextMenu {

                    MenuItem {
                        //% "none"
                        text: qsTrId("id-none")
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

                    App.entitiesService().callService("climate",
                                                      "set_preset_mode",
                                                      {
                                                        entity_id: entity.entityId,
                                                        preset_mode: mode
                                                      })

                    entity.attributes.preset_mode = mode
                }
            }

            SectionHeader {
                //% "Info"
                text: qsTrId("id-info")
            }

            DetailItem {
                //% "Current state"
                label: qsTrId("id-current-state")
                value: entity.state
            }

            DetailItem {
                visible: entity.hasFeature(Climate.ClimateTemperature)
                //% "Current room temperature"
                label: qsTrId("id-current-room-temperature")
                value: entity.attributes.current_temperature + " " + App.api().serverConfig().unitTemperature
            }
        }
    }

    Connections {
        target: App.api()
        onRequestFinished: if (requestType === Api.RequestGetApiStatesEntity) busy = false
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}

