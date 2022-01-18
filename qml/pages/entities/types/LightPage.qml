import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../../components"

Page {
    property bool busy: false
    property Light entity

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
            spacing: Theme.paddingLarge

            PageHeader {
                title: entity.name
            }

            SectionHeader {
                //% "Features"
                text: qsTrId("id-features")
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                //% "Switch light on/off"
                text: qsTrId("id-switch-light-on-off")
                checked: entity.state === "on"

                onClicked: {
                    App.entitiesService().callService("light",
                                                      checked ? "turn_on" : "turn_off",
                                                      {
                                                          entity_id: entity.entityId
                                                      })
                }
            }

            // ------------------------------------------------------------------------------------------------------------
            // BRIGHTNESS
            // ------------------------------------------------------------------------------------------------------------

            Slider {
                id: brightnessSlider

                //% "Brightness"
                label: qsTrId("id-brightness")

                visible: entity.hasFeature(Light.LightBrightness)

                width: parent.width
                stepSize: 1
                minimumValue: 0
                maximumValue: 255

                value: entity.attributes.brightness

                onPressedChanged: {
                    if (pressed)
                        return;

                    App.entitiesService().callService("light",
                                                      "turn_on",
                                                      {
                                                          entity_id: entity.entityId,
                                                          brightness: value
                                                      })
                }
            }

            // ------------------------------------------------------------------------------------------------------------
            // COLOR TEMPERATURE
            // ------------------------------------------------------------------------------------------------------------

            Slider {
                id: colorTempSlider

                //% "Color temperature"
                label: qsTrId("id-color-temperature")

                visible: entity.hasFeature(Light.LightColorTemp)

                width: parent.width
                stepSize: 1
                minimumValue: entity.attributes.min_mireds
                maximumValue: entity.attributes.max_mireds

                value: entity.attributes.color_temp

                onPressedChanged: {
                    if (pressed)
                        return;

                    App.entitiesService().callService("light",
                                                      "turn_on",
                                                      {
                                                          entity_id: entity.entityId,
                                                          color_temp: value
                                                      })
                }
            }

            // ------------------------------------------------------------------------------------------------------------
            // COLOR
            // ------------------------------------------------------------------------------------------------------------

            ColorWheel {
                id: colorWheel
                visible: entity.hasFeature(Light.LightColor)
                anchors.horizontalCenter: parent.horizontalCenter
                width: Math.min(parent.width, parent.height) * 0.6
                height: width

                hue: entity.hue
                saturation: entity.saturation

                onRgbChanged: {
                    entity.color = rgb
                    App.entitiesService().callService("light",
                                                      "turn_on",
                                                      {
                                                          entity_id: entity.entityId,
                                                          rgb_color: [
                                                            entity.red(),
                                                            entity.green(),
                                                            entity.blue()
                                                          ]
                                                      })
                }
            }

            // ------------------------------------------------------------------------------------------------------------
            // EFFECT
            // ------------------------------------------------------------------------------------------------------------

            ComboBox {
                id: effectCombo
                visible: entity.hasFeature(Light.LightEffect)
                width: parent.width
                //% "Effect"
                label: qsTrId("id-effect")

                menu: ContextMenu {
                    MenuItem {
                        //% "off"
                        text: qsTrId("id-off")
                    }

                    Repeater {
                        model: entity.attributes.effect_list

                        MenuItem {
                            text: modelData
                        }
                    }
                }

                onCurrentIndexChanged: {
                    var effect
                    if (currentIndex === 0)
                        effect = "none"
                    else
                        effect = currentItem.text

                    App.entitiesService().callService("light",
                                                      "turn_on",
                                                      {
                                                          entity_id: entity.entityId,
                                                          effect: effect
                                                      })
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

