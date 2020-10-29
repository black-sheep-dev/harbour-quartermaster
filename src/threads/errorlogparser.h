#ifndef ERRORLOGPARSER_H
#define ERRORLOGPARSER_H

#include <QObject>

#include <QPair>

class ErrorLogParser : public QObject
{
    Q_OBJECT

signals:
    void logParsed(QStringList headers, QStringList messages);

public slots:
    void parseData(const QByteArray &data);
};

#endif // ERRORLOGPARSER_H
