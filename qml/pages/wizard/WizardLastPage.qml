import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Page {
    id: page

    allowedOrientations: Orientation.Portrait

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: Client.busy
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * x

            opacity: 0

            PageHeader {
                Row {
                    x: Theme.paddingMedium
                    width: parent.width - 2*x
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width / 2
                        text: qsTr("Back")

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }

                    Label {
                        id: labelProceed
                        visible: false
                        width: parent.width / 2
                        text: ""

                        horizontalAlignment: Text.AlignRight

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }

            Label {
                width: parent.width

                text: qsTr("Registration failed")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }
        }
    }

    onStatusChanged: if (status == PageStatus.Active) Client.registerDevice()

    Connections {
        target: Client
        onRegisteredChanged: {
            if (Client.registered) {
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("../OverviewPage.qml"))
            }
        }
    }
}
