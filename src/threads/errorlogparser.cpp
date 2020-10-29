#include "errorlogparser.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

void ErrorLogParser::parseData(const QByteArray &data)
{
    const QString raw = QString(data);

#ifdef QT_DEBUG
    qDebug() << "THREAD PARSE START";
#endif

    QStringList headers;
    QStringList messages;

    int idx = raw.indexOf(QRegExp("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}"), 0);

    while (idx >= 0) {
        const int next = raw.indexOf(QRegExp("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}"), idx + 19);

        const int end = raw.indexOf("]", idx);

        headers.append(raw.mid(idx, end - idx + 1));

        if (next > 0)
            messages.append(raw.mid(end + 2, next - end));
        else
            messages.append(raw.mid(end + 2, raw.size() - end));

        idx = next;
    }

#ifdef QT_DEBUG
    qDebug() << "THREAD FINISHED";
#endif

    emit logParsed(headers, messages);
}
