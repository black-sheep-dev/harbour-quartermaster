#ifndef SENSORSERVICE_H
#define SENSORSERVICE_H

#include <QObject>

class SensorService : public QObject
{
    Q_OBJECT
public:
    explicit SensorService(QObject *parent = nullptr);

    void initialize();

signals:

public slots:

private:

};

#endif // SENSORSERVICE_H
