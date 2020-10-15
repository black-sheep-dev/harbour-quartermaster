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
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                Row {
                    x: Theme.horizontalPageMargin
                    width: page.width - 2 * x
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        width: parent.width / 2
                        text: qsTr("Back")

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }

                    Label {
                        width: parent.width / 2
                        text: qsTr("Register")

                        horizontalAlignment: Text.AlignRight

                        color: Theme.secondaryHighlightColor
                        font.pixelSize: Theme.fontSizeExtraLarge
                    }
                }
            }

            Label {
                x: Theme.horizontalPageMargin
                width: page.width - 2 * x

                text: qsTr("Device registration")

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                x: Theme.horizontalPageMargin
                width: page.width - 2 * x
                wrapMode: Text.WordWrap

                text: qsTr("This is the final step. You can change the device name which shows up in Homeassistant if you want before starting the registration.")
                color: Theme.highlightColor
            }

            Label {
                x: Theme.horizontalPageMargin
                width: page.width - 2 * x
                text: qsTr("Change device name")

                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
            }

            TextField {
                id: deviceNameField
                width: parent.width

                label: qsTr("Device name")
                placeholderText: qsTr("Enter device name")

                text: Client.device().name

                onTextChanged: checkInput()
            }
        }
    }

    function checkInput() {
        canNavigateForward = deviceNameField.length > 0
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(Qt.resolvedUrl("WizardLastPage.qml"))
        } else if (status == PageStatus.Deactivating) {
            Client.device().name = deviceNameField.text
        }
    }

    Component.onCompleted: checkInput()
}

