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

            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                //% "Developer mode"
                title: qsTrId("id-developer-mode")
            }

            TextSwitch {
                id: apiLoggingSwitch
                //% "API logging"
                text: qsTrId("id-api-logging")
                //% "Log all API requests and replies. This helps to fix errors or to include new devices to Quartermaster."
                //% "Only turn on logging if needed, because it will create a lot of data! Files are stored into Documents folder."
                //% "Attach this data to a new issue on GitHub if you need support!"
                description: qsTrId("id-api-logging-desc")

                onCheckedChanged: App.api().logging = checked

                Component.onCompleted: checked = App.api().logging
            }
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) App.saveSettings()
}

