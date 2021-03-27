import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy: false
    property Light entity

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            busy: page.busy
            MenuItem {
                text: qsTr("Attributes")
                onClicked: pageStack.push(Qt.resolvedUrl("../EntityAttributesPage.qml"), { entity: entity })
            }
            MenuItem {
                text: qsTr("Refresh")
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
                text: qsTr("Features")
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                text: qsTr("Switch light on/off")
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

                label: qsTr("Brightness")

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

                label: qsTr("Color temperature")

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

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                spacing: Theme.paddingLarge

                Label {
                    visible: entity.hasFeature(Light.LightColor)

                    text: qsTr("Color")


                }

                Rectangle {
                    visible: entity.hasFeature(Light.LightColor)

                    id: colorSelect
                    width: 96
                    height: 96

                    radius: 48

                    color: entity.color

                    MouseArea {
                        anchors.fill: colorSelect

                        onClicked: {
                            var dialog = pageStack.push("Sailfish.Silica.ColorPickerDialog")
                            dialog.accepted.connect(function() {
                                entity.color = dialog.color
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
                            })
                        }
                    }
                }
            }

            // ------------------------------------------------------------------------------------------------------------
            // EFFECT
            // ------------------------------------------------------------------------------------------------------------

            ComboBox {
                id: effectCombo
                visible: entity.hasFeature(Light.LightEffect)
                width: parent.width
                label: qsTr("Effect")

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("off")
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

