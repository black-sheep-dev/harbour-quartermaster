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

    enum Subscription {
        SubscriptionNone                = 0x0,
        SubscriptionNotifyEvents        = 0x1,
        SubscriptionStateChanged        = 0x2
    };
    Q_ENUM(Subscription)
    Q_DECLARE_FLAGS(Subscriptions, Subscription)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Api::Subscriptions)

#endif // API_H
