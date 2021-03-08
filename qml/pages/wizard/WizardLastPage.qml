import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

import "../../components"

Dialog {
    property bool busy: true

    id: dialog
    allowedOrientations: Orientation.Portrait

    PageBusyIndicator {
        size: BusyIndicatorSize.Large
        running: busy
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    DialogHeader {
        id: header
        acceptText: qsTr("Complete")
        cancelText: qsTr("Back")
    }

    Column {
        anchors.top: header.bottom
        x: Theme.horizontalPageMargin
        width: parent.width - 2*x

        opacity: 0

        Label {
            width: parent.width

            text: qsTr("Registration failed")

            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeLarge
        }
    }

    onAccepted: {
        App.wallet().storeCredentials()
        pageStack.clear()
        pageStack.push(Qt.resolvedUrl("../OverviewPage.qml"))
    }
}
