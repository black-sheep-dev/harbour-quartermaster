import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.quartermaster 1.0

Page {
    id: page
    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: page.width - 2 * x
            spacing: Theme.paddingLarge

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
                        width: parent.width / 2
                        text: qsTr("Connect")

                        horizontalAlignment: Text.AlignRight

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }

            Label {
                width: parent.width

                text: qsTr("Access Token")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap

                text: qsTr("You need to provide a valid Long-Lived Access Token.\n"
                           + "You can create one on profile page in your Homeassistant webfrontend and copy the token string into the input field below.")
                color: Theme.highlightColor
            }

            Label {
                width: parent.width
                text: qsTr("Enter Long-Lived Access Token")

                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            TextArea {
                id: tokenField
                width: parent.width

                label: qsTr("Token")
                placeholderText: qsTr("Enter token")

                text: Client.token
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("WizardInfoPage.qml"))
        } else if (status == PageStatus.Deactivating) {
            Client.token = tokenField.text
            Client.connectToHost()
        }
    }
}
