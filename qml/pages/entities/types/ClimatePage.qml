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
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: entity.name
            }

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    width: parent.width * 0.7
                    text: qsTr("Current state:")
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.state
                    color: Theme.highlightColor
                }
            }

            Row {
                visible: entity.hasFeature(Climate.ClimateTemperature)

                x: Theme.horizontalPageMargin
                width: parent.width - 2*x

                Label {
                    text: qsTr("Current room temperature:")
                    width: parent.width * 0.7
                    color: Theme.highlightColor
                }

                Label {
                    text: entity.attributes.current_temperature + " " + Client.homeassistantInfo().unitTemperature
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

