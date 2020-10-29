#ifndef ERRORLOGMODEL_H
#define ERRORLOGMODEL_H

#include <QAbstractListModel>

#include <QDateTime>

class ErrorLogModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    enum ErrorLogRoles {
        HeaderRole      = Qt::UserRole + 1,
        MessageRole,
        PreviewRole
    };
    Q_ENUM(ErrorLogRoles)

    explicit ErrorLogModel(QObject *parent = nullptr);
    ~ErrorLogModel() override;

    // properties
    bool loading() const;

signals:
    // properties
    void loadingChanged(bool loading);

public slots:
    void setLogs(QStringList headers, QStringList messages);

    // properties
    void setLoading(bool loading);

private:
    QStringList m_headers;
    QStringList m_messages;

    // properties
    bool m_loading{false};


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ERRORLOGMODEL_H
