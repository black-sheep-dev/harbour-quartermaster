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
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity})
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            spacing: Theme.paddingMedium

            PageHeader {
                title: entity.name
            }

            Row {
                width: parent.width

                Label {
                    text: qsTr("Current state:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.state
                    color: Theme.highlightColor
                }
            }

            Row {
                visible: entity.hasFeature(Climate.ClimateTemperature)

                width: parent.width

                Label {
                    text: qsTr("Current room temperature:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.current_temperature + " °C"
                    color: Theme.highlightColor
                }
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

                    Client.entitiesProvider().callService("climate",
                                                          "set_temperature",
                                                          entity.entityId,
                                                          {
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
                        text: qsTr("None")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetActivity)
                        text: qsTr("Activity")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetAway)
                        text: qsTr("Away")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetBoost)
                        text: qsTr("Boost")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetComfort)
                        text: qsTr("Comfort")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetEco)
                        text: qsTr("Eco")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetHome)
                        text: qsTr("Home")
                    }
                    MenuItem {
                        visible: entity.hasPresetMode(Climate.PresetSleep)
                        text: qsTr("Sleep")
                    }
                }

                onCurrentIndexChanged: {
                    var mode

                    switch (currentIndex) {
                    case 1:
                        mode = "activity"
                        break

                    case 2:
                        mode = "away"
                        break

                    case 3:
                        mode = "boost"
                        break

                    case 4:
                        mode = "comfort"
                        break

                    case 5:
                        mode = "eco"
                        break

                    case 6:
                        mode = "home"
                        break

                    case 7:
                        mode = "sleep"
                        break

                    default:
                        mode = "none"
                        break
                    }

                    Client.entitiesProvider().callService("climate",
                                                          "set_preset_mode",
                                                          entity.entityId,
                                                          {
                                                              preset_mode: mode
                                                          })

                    entity.attributes.preset_mode = mode
                }
            }


        }
    }
}

