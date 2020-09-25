import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    x: Theme.horizontalPageMargin

    width: page.width - 2 * x
    spacing: Theme.paddingLarge

    TextField {
        id: hostnameField
        width: parent.width

        label: qsTr("Hostname")
        placeholderText: qsTr("Enter hostname")

        text: hostname

        inputMethodHints: Qt.ImhUrlCharactersOnly

        EnterKey.iconSource: "image://theme/icon-m-enter-next"
        EnterKey.onClicked: portField.focus = true
    }

    TextSwitch {
        id: sslSwitch
        text: qsTr("Use SSL")
        description: qsTr("It is highly recommend to use a ssl connection if your homeasstiant server support this!")

        checked: ssl
    }

    TextField {
        id: portField
        width: parent.width / 2

        label: qsTr("Port")

        text: port

        inputMethodHints: Qt.ImhDigitsOnly
        validator: IntValidator { bottom: 1; top: 65535;}

        EnterKey.iconSource: "image://theme/icon-m-enter-next"
        EnterKey.onClicked: tokenField.focus = true
    }
}

