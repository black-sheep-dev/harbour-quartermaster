import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.WebView 1.0

import org.nubecula.harbour.quartermaster 1.0

WebViewPage {
    id: page

    allowedOrientations: Orientation.All

    WebViewFlickable {
        anchors.fill: parent

        WebView {
            anchors.fill: parent
            active: true
            url: Client.baseUrl()
        }
    }
}
