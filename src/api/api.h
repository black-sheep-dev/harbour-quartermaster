#ifndef API_H
#define API_H

#include <QObject>

class Api {

    Q_GADGET

public:
    enum Error {
        ErrorNone,
        ErrorBadRequest,
        ErrorDataInvalid,
        ErrorJsonInvalid,
        ErrorIntegrationDeleted,
        ErrorNotFound,
        ErrorMethodNotAllowed,
        ErrorMobileAppNotLoaded,
        ErrorUnauthorized,
        ErrorUnkown
    };
    Q_ENUM(Error)

    enum Request {
        RequestGetApi,
        RequestGetApiConfig,
        RequestGetApiDiscoveryInfo,
        RequestGetApiEvents,
        RequestGetApiServices,
        RequestGetApiHistoryPeriod,
        RequestGetApiLogBook,
        RequestGetApiStates,
        RequestGetApiStatesEntity,
        RequestGetApiErrorLog,
        RequestGetApiCameraProxy,
        RequestPostApiStates,
        RequestPostApiEvents,
        RequestPostApiServices,
        RequestPostApiConfigCheckConfig,
        RequestPostApiRegisterDevice,

        RequestWebhookCallService                   = 100,
        RequestWebhookEnableEncryption,
        RequestWebhookFireEvent,
        RequestWebhookGetConfig,
        RequestWebhookGetZones,
        RequestWebhookRenderTemplate,
        RequestWebhookRegisterSensor,
        RequestWebhookStreamCamera,
        RequestWebhookUpdateLocation,
        RequestWebhookUpdateRegistration,
        RequestWebhookUpdateSensorStates
    };
    Q_ENUM(Request)
};

#endif // API_H
