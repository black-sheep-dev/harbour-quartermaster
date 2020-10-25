#ifndef WEBSOCKETAPI_H
#define WEBSOCKETAPI_H

#include <QObject>

class WebsocketApi : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketApi(QObject *parent = nullptr);

signals:

public slots:
};

#endif // WEBSOCKETAPI_H
