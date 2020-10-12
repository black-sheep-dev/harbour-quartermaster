import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            x: Theme.horizontalPageMargin

            width: page.width - 2 * x
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Developer Mode")
            }

            TextSwitch {
                id: apiLoggingSwitch
                text: qsTr("API logging")
                description: qsTr("Log all API requests and replies. This helps to fix errors or to include new devices to Quartermaster.")
                             + "\n"
                             + qsTr("Only turn on logging if needed, because it will create a lot of data! Files are stored into Documents folder.")
                             + "\n"
                             + qsTr("Attach this data to a new issue on GitHub if you need support!")

                onCheckedChanged: Client.apiLogging = checked

                Component.onCompleted: checked = Client.apiLogging
            }
        }
    }
}

