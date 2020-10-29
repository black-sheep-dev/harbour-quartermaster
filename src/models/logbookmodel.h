#ifndef LOGBOOKMODEL_H
#define LOGBOOKMODEL_H

#include <QAbstractListModel>

#include <QDateTime>
#include <QJsonArray>

struct LogEntry {
    QString domain;
    QString entityId;
    QString message;
    QString name;
    QDateTime timestamp;
};

class LogBookModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)

public:
    enum LogEntryRoles {
        DomainRole          = Qt::UserRole + 1,
        EntityIdRole,
        MessageRole,
        NameRole,
        TimestampRole
    };
    Q_ENUM(LogEntryRoles)

    explicit LogBookModel(QObject *parent = nullptr);

    void setLogEntries(const QJsonArray &array);

    // properties
    bool loading() const;
    QDateTime startTime() const;

signals:
    // properties
    void loadingChanged(bool loading);
    void startTimeChanged(const QDateTime &startTime);



public slots:
    // properties
    void setLoading(bool loading);
    void setStartTime(const QDateTime &startTime);

private:
    // properties
    bool m_loading{false};
    QList<LogEntry> m_entries;

    // QAbstractItemModel interface
    QDateTime m_startTime;



public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // LOGBOOKMODEL_H
