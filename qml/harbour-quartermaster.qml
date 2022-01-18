import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0

import "pages"

import org.nubecula.harbour.quartermaster 1.0

ApplicationWindow
{
    Notification {
        function show(message, icn) {
            replacesId = 0
            previewSummary = ""
            previewBody = message
            icon = icn || ""
            publish()
        }
        function showPopup(title, message, icn) {
            replacesId = 0
            previewSummary = title
            previewBody = message
            icon = icn
            publish()
        }

        id: notification
        appName: "SailHub"
        expireTimeout: 3000
    }

    Connections {
        target: App.api()
        onRequestError: {
            //% "Request failed"
            var msg = qsTrId("id-request-failed") + ": "

            switch (code) {
            case Api.ErrorBadRequest:
                //% "Bad request"
                msg += qsTrId("id-error-bad-request")
                break

            case Api.ErrorDataInvalid:
                //% "Data invalid"
                msg += qsTrId("id-error-data-invalid")
                break

            case Api.ErrorJsonInvalid:
                //% "JSON invalid"
                msg += qsTrId("id-error-json-invalid")
                break

            case Api.ErrorIntegrationDeleted:
                //% "Integration deleted"
                msg += qsTrId("id-error-integration-deleted")
                break

            case Api.ErrorIntegrationDeleted:
                //% "Not found"
                msg += qsTrId("id-error-not-found")
                break

            case Api.ErrorIntegrationDeleted:
                //% "Method not allowed"
                msg += qsTrId("id-error-method-not-allowed")
                break

            case Api.ErrorIntegrationDeleted:
                //% "Mobile app extension not loaded"
                msg += qsTrId("id-error-mobile-app-not-loaded")
                break

            case Api.ErrorUnauthorized:
                //% "Unauthorized"
                msg += qsTrId("id-error--unauthorized")
                break

            default:
                return
            }

            notification.show(msg)
        }
    }

    initialPage: Component { OverviewPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: App.initialize()
}
