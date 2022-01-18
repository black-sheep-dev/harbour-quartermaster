import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property Entity entity

    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: Column.height

        Column {
            id: column;
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                //% "Attributes"
                title: qsTrId("id-attributes")
            }

            Repeater {
                model: SortFilterModel {
                    id: sortModel
                    sourceModel: EntityAttributesModel {
                        entity: page.entity
                    }
                    sortRole: EntityAttributesModel.KeyRole
                }

                DetailItem {
                    label: model.key
                    value: model.value
                }
            }

            VerticalScrollDecorator {}
        }
    }
}

