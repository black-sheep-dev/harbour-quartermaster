#include "logbookmodel.h"

#include <QJsonObject>

#include "src/string_constants.h"

LogBookModel::LogBookModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

void LogBookModel::setLogEntries(const QJsonArray &array)
{
    beginResetModel();
    if (!m_entries.isEmpty())
        m_entries.clear();

    for (const QJsonValue &value : array) {
        const QJsonObject obj = value.toObject();

        LogEntry entry;

        entry.domain = obj.value(API_KEY_DOMAIN).toString();
        entry.entityId = obj.value(API_KEY_ENTITY_ID).toString();
        entry.message = obj.value(API_KEY_MESSAGE).toString();
        entry.name = obj.value(API_KEY_NAME).toString();
        entry.timestamp = QDateTime::fromString(obj.value(API_KEY_WHEN).toString(), Qt::ISODate);

        m_entries.append(entry);
    }

    endResetModel();

    setLoading(false);
}

bool LogBookModel::loading() const
{
    return m_loading;
}

QDateTime LogBookModel::startTime() const
{
    return m_startTime;
}

void LogBookModel::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

void LogBookModel::setStartTime(const QDateTime &startTime)
{
    if (m_startTime == startTime)
        return;

    m_startTime = startTime;
    emit startTimeChanged(m_startTime);
}

int LogBookModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_entries.count();
}

QVariant LogBookModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const LogEntry entry = m_entries.at(index.row());

    switch (role) {
    case DomainRole:
        return entry.domain;

    case EntityIdRole:
        return entry.entityId;

    case MessageRole:
        return entry.message;

    case NameRole:
        return entry.name;

    case TimestampRole:
        return entry.timestamp;

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LogBookModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[DomainRole]       = "domain";
    roles[EntityIdRole]     = "entity_id";
    roles[MessageRole]      = "message";
    roles[NameRole]         = "name";
    roles[TimestampRole]    = "timestamp";

    return roles;
}
