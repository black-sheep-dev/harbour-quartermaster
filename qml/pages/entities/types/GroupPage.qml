import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property Group entity

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

        Column {
            id: column

            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: entity.name
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                text: qsTr("Group on/off")
                checked: entity.state === "on"

                onClicked: {
                    Client.entitiesProvider().callService("homeassistant",
                                                          checked ? "turn_on" : "turn_off",
                                                          entity.entityId)
                }
            }

            SectionHeader{
                text: qsTr("Children")
            }
        }

        SilicaListView {
            id: listView

            width: parent.width
            anchors.top: column.bottom
            anchors.bottom: parent.bottom

            clip: true

            model: EntitiesSortFilterModel {
                id: filterModel
                sourceModel: entity.childrenModel()
            }

            delegate: ListItem {
                id: delegate
                width: parent.width
                contentHeight: Theme.itemSizeLarge

                Row {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * x
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        id: itemIcon
                        source: Client.entitiesProvider().getEntityIcon(type)
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item {
                        width: Theme.paddingMedium
                        height: 1
                    }

                    Column {
                        width: parent.width - itemIcon.width - Theme.paddingMedium
                        anchors.verticalCenter: itemIcon.verticalCenter

                        Label {
                            width: parent.width
                            text: name
                            color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                            font.pixelSize: Theme.fontSizeLarge
                        }
                        Label {
                            text: entity_state

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                    }
                }
            }
        }
    }
}
