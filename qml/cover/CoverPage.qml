import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
        smooth: true
        source: "qrc:///cover/cover-background"
        opacity: 0.1
    }
}
