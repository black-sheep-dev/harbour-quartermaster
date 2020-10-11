import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    property Entity entity

    id: page

    allowedOrientations: Orientation.All

    Timer {
        interval: 5000
        running: true
        repeat: true

        onTriggered: {
            cameraView.update()
        }
    }

    SilicaFlickable {
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

            Image {
                id: cameraView
                asynchronous: true
                cache: false
                source: Client.baseUrl() + entity.attributes.entity_picture
                smooth: true
                width: column.width * 0.8

                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
