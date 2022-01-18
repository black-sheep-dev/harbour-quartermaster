import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.DBus 2.0

Page {
    property string appId: "harbour-quartermaster"

    id: page

    allowedOrientations: Orientation.All

    DBusInterface {
        id: sailHubInterface

        service: "harbour.sailhub.service"
        iface: "harbour.sailhub.service"
        path: "/harbour/sailhub/service"
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                //% "About"
                title: qsTrId("id-about")
            }

            Image {
                id: logo
                source: "/usr/share/icons/hicolor/512x512/apps/" + appId + ".png"
                smooth: true
                height: width
                width: parent.width / 2
                sourceSize.width: 512
                sourceSize.height: 512
                anchors.horizontalCenter: parent.horizontalCenter
                opacity: 0.7
            }

            Label {
                width: parent.width
                x : Theme.horizontalPageMargin
                font.pixelSize: Theme.fontSizeExtraLarge
                color: Theme.secondaryHighlightColor

                //% "Quartermaster"
                text: qsTrId("id-quartermaster")
            }

            Label {
                width: parent.width
                x : Theme.horizontalPageMargin
                text: Qt.application.version
            }

            Item {
                height: Theme.paddingMedium
                width: 1
            }

            Label {
                width: parent.width - 2 * x
                x : Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                //% "Quartermaster is a native Sailfish OS Home Assistant client."
                text: qsTrId("id-about-desc")
            }

            SectionHeader{
                //% "Translations"
                text: qsTrId("id-translations")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall

                //% "Your language is not available? You are welcome to support this project by translating it on my self hosted Weblate server."
                text: qsTrId("id-about-translation-desc")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium
                Row{
                    x : Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:///icons/weblate"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "https://weblate.nubecula.org/projects/" + appId
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: Qt.openUrlExternally("https://weblate.nubecula.org/engage/" + appId)
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                height: sourceSize.height * width / sourceSize.width
                smooth: true
                fillMode: Image.PreserveAspectFit
                source: "http://weblate.nubecula.org/widgets/" + appId + "/-/" + appId + "/multi-auto.svg"
            }

            SectionHeader{
                //% "Sources"
                text: qsTrId("id-sources")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium
                Row{
                    x : Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:///icons/git"
                    }

                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall

                        text: "https://github.com/black-sheep-dev/" + appId
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor

                    }
                }
                onClicked: Qt.openUrlExternally("https://github.com/black-sheep-dev/" + appId)
            }

            ButtonLayout {
                width: parent.width

                Button {
                    //% "Give star"
                    text: qsTrId("id-give-star")
                    onClicked: sailHubInterface.call("addStar", ["black-sheep-dev", appId])
                }
            }

            SectionHeader{
                //% "Donations"
                text: qsTrId("id-donations")
            }

            Label {
                x : Theme.horizontalPageMargin
                width: parent.width - 2*x

                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                //% "If you like my work why not buy me a beer?"
                text: qsTrId("id-about-donation-desc")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium

                Row{
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height
                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:///icons/paypal"
                    }
                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor
                        //% "Donate with PayPal"
                        text: qsTrId("id-donate-paypal")
                    }
                }
                onClicked: Qt.openUrlExternally("https://www.paypal.com/paypalme/nubecula/1")
            }

            BackgroundItem{
                width: parent.width
                height: Theme.itemSizeMedium

                Row{
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2*x
                    height: parent.height

                    spacing:Theme.paddingMedium

                    Image {
                        width: parent.height
                        height: width
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:///icons/liberpay"
                    }
                    Label{
                        width: parent.width - parent.height - parent.spacing
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: Theme.fontSizeSmall
                        color: parent.parent.pressed ? Theme.highlightColor : Theme.primaryColor
                        //% "Donate with Liberpay"
                        text: qsTrId("id-donate-liberpay")
                    }
                }
                onClicked: Qt.openUrlExternally("https://liberapay.com/black-sheep-dev/donate")
            }

            Item {
                width: 1
                height: Theme.paddingSmall
            }
        }
    }
}
