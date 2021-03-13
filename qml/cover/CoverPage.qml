import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        width: parent.width
        height: width
        smooth: true
        source: "qrc:///cover/cover-background"
        anchors.centerIn: parent
        opacity: 0.1
    }
}
