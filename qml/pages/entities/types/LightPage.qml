import QtQuick 2.0

import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../../components"

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
            x: Theme.horizontalPageMargin
            width: parent.width - 2*x
            spacing: Theme.paddingMedium

            PageHeader {
                title: entity.name
            }

            SectionHeader {
                text: qsTr("Functions")
            }

            TextSwitch {
                text: qsTr("Switch light on/off")
                checked: entity.state === "on"

                onClicked: {
                    Client.entitiesProvider().callService("light",
                                                          checked ? "turn_on" : "turn_off",
                                                          page.entity.entityId)
                }
            }

            Label {
                visible: page.entity.features & Entity.LightBrightness
                text: qsTr("Brightness")
                color: Theme.highlightColor
            }

            Slider {
                id: brightnessSlider

                visible: page.entity.features & Entity.LightBrightness

                width: parent.width
                stepSize: 1
                minimumValue: 0
                maximumValue: 255

                value: page.entity.attributes.brightness

                onPressedChanged: {
                    if (pressed)
                        return;

                    Client.entitiesProvider().callService("light",
                                                          "turn_on",
                                                          page.entity.entityId,
                                                          {
                                                              brightness: value
                                                          })

                    page.entity.attributes.brightness = value
                }
            }

            Label {
                visible: page.entity.features & Entity.LightColor
                text: qsTr("Brightness")
                color: Theme.highlightColor
            }

            ColorPicker {
                id: colorPicker
                width: 512
                height: 512

                onColorsChanged: Client.entitiesProvider().callService("light",
                                                                       "turn_on",
                                                                       page.entity.entityId,
                                                                       {
                                                                           rgb_color: [
                                                                               125,
                                                                               255,
                                                                               0
                                                                           ]
                                                                       })
            }
        }
    }
}

