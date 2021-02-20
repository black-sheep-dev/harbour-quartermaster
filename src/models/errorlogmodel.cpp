#include "errorlogmodel.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

ErrorLogModel::ErrorLogModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

ErrorLogModel::~ErrorLogModel()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("ErrorLogModel deleted");
#endif
}

bool ErrorLogModel::loading() const
{
    return m_loading;
}

void ErrorLogModel::setLogs(const QStringList &headers, const QStringList &messages)
{
    beginResetModel();
    m_headers = headers;
    m_messages = messages;
    endResetModel();

    setLoading(false);
}

void ErrorLogModel::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

int ErrorLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_headers.count();
}

QVariant ErrorLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto header = m_headers.at(index.row());
    const auto msg = m_messages.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return header + " " + msg;

    case HeaderRole:
        return header;

    case MessageRole:
        return msg;

    case PreviewRole:
        return msg.mid(0, 100) + QStringLiteral("...");

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ErrorLogModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[HeaderRole]   = "header";
    roles[MessageRole]  = "message";
    roles[PreviewRole]  = "preview";

    return roles;
}
