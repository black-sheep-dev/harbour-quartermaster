import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components/"

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView

        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Persons")
        }

        model: EntitiesSortFilterModel {
            id: filterModel
            sourceModel: Client.entitiesProvider().model(EntitiesProvider.PersonsModel)
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

                CircleImage {
                    id: avatar
                    width: parent.height - 2 * Theme.paddingSmall
                    source: Client.baseUrl() + attributes.entity_picture
                    anchors.verticalCenter: parent.verticalCenter

                    fallbackText: listView.getInitials(name)
                    fallbackItemVisible: attributes.entity_picture ? false : true
                }

                Item {
                    width:Theme.paddingMedium
                    height:1
                }

                Column {
                    id: data
                    width: parent.width - avatar.width - Theme.paddingMedium
                    anchors.verticalCenter: avatar.verticalCenter

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

        function getInitials(name) {
            var initials = name.match(/\b\w/g) || [];
            initials = ((initials.shift() || '') + (initials.pop() || '')).toUpperCase();
            return initials
        }
    }
}

