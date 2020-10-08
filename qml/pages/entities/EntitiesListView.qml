import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    property string title
    property int type
    property string icon

    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView

        anchors.fill: parent
        header: PageHeader {
            title: title
        }

        model: EntitiesSortFilterModel {
            id: filterModel
            sourceModel: Client.entitiesProvider().model(type)
        }

        delegate: ListItem {
            id: delegate
            width: parent.width
            contentHeight: Theme.itemSizeExtraLarge

            Row {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * x
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIcon
                    source: page.icon
                    anchors.verticalCenter: parent.verticalCenter
                }

                Item {
                    width:Theme.paddingMedium
                    height:1
                }

                Column {
                    id: data
                    width: parent.width - itemIcon.width - Theme.paddingMedium
                    anchors.verticalCenter: itemIcon.verticalCenter

                    Label {
                        width: parent.width
                        text: name
                        color: pressed ? Theme.secondaryHighlightColor : Theme.highlightColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label {
                        text: entity_id

                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeMedium
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }
}


