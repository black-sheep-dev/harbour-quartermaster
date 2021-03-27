#ifndef BACKUPTOOL_H
#define BACKUPTOOL_H

#define     QUARTERMASTER_BACKUP_FILE_MAGIC         0x514d42    // QMB
#define     QUARTERMASTER_BACKUP_FILE_VERSION       1
#define     QUARTERMASTER_BACKUP_FILE_SUFFIX        ".qmb"
#define     QUARTERMASTER_BACKUP_FILE_PREFIX        "quartermaster-backup-"

#include <QObject>

class BackupTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList availableBackups READ availableBackups WRITE setAvailableBackups NOTIFY availableBackupsChanged)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)

public:    
    enum BackupOption {
        BackupNone          = 0x00,
        BackupAccessPoints  = 0x01
    };
    Q_ENUM(BackupOption)
    Q_DECLARE_FLAGS(BackupOptions, BackupOption)

    explicit BackupTool(QObject *parent = nullptr);

    Q_INVOKABLE bool backup(quint16 options);
    Q_INVOKABLE bool deleteBackup(const QString &filename);
    Q_INVOKABLE bool restoreBackup(const QString &filename);

    // properties
    QStringList availableBackups() const;
    bool busy() const;

signals:
    // properties
    void availableBackupsChanged(const QStringList &backups);
    void busyChanged(bool busy);

public slots:
    // properties
    void setAvailableBackups(const QStringList &backups);
    void setBusy(bool busy);

private:
    void refreshAvailableBackups();
    bool writeFile(const QString &path, const QByteArray &data);

    // properties
    QStringList m_availableBackups;
    bool m_busy{false};

};
Q_DECLARE_OPERATORS_FOR_FLAGS(BackupTool::BackupOptions)

#endif // BACKUPTOOL_H
