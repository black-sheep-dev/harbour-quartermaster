import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property bool busy: false
    property Group entity

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
                    App.entitiesService().callService("homeassistant",
                                                      checked ? "turn_on" : "turn_off",
                                                      {
                                                          entity_id: entity.entityId
                                                      })
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
                sortRole: EntitiesModel.NameRole
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
                    spacing: Theme.paddingMedium

                    Image {
                        id: itemIcon
                        source: App.entitiesService().getEntityIcon(type)
                        anchors.verticalCenter: parent.verticalCenter
                    }


                    Column {
                        width: parent.width - itemIcon.width - Theme.paddingMedium
                        anchors.verticalCenter: itemIcon.verticalCenter

                        Label {
                            width: parent.width
                            text: name
                            color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                            font.pixelSize: Theme.fontSizeMedium
                        }
                        Label {
                            text: entityState

                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }
            }

            VerticalScrollDecorator {}
        }
    }

    Connections {
        target: App.api()
        onRequestFinished: if (requestType === Api.RequestGetApiStatesEntity) busy = false
    }

    //Component.onCompleted: if ((Client.updateModes & Client.UpdateModeSingleEntity) === Client.UpdateModeSingleEntity) Client.entitiesProvider().updateEntity(entity.entityId)
}
