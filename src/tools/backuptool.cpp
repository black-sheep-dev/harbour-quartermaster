#include "backuptool.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDataStream>
#include <QDateTime>
#include <QDirIterator>
#include <QFile>
#include <QStandardPaths>

BackupTool::BackupTool(QObject *parent) :
    QObject(parent)
{
    refreshAvailableBackups();
}

bool BackupTool::backup(quint16 options)
{
    setBusy(true);

    // write backup
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
               + "/" + QUARTERMASTER_BACKUP_FILE_PREFIX
               + QDateTime::currentDateTime().toString("yyyyMMddhhmmss")
               + QUARTERMASTER_BACKUP_FILE_SUFFIX);

    if (!file.open(QIODevice::WriteOnly)) {
        setBusy(false);
        return false;
    }

    QDataStream out(&file);

    out << quint32(QUARTERMASTER_BACKUP_FILE_MAGIC);
    out << quint16(QUARTERMASTER_BACKUP_FILE_VERSION);

    // wifi option
    if (options & BackupWifi) {
        QFile wifi(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + QStringLiteral("/networks.json"));

        if (wifi.open(QIODevice::ReadOnly | QIODevice::Text)) {
            out << quint16(BackupTool::BackupWifi);
            out << qCompress(wifi.readAll());
            wifi.close();
        }
    }

    file.close();

    refreshAvailableBackups();
    setBusy(false);
    return true;
}

bool BackupTool::deleteBackup(const QString &filename)
{
    bool ok = QFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + filename + QUARTERMASTER_BACKUP_FILE_SUFFIX).remove();

    if (ok)
        refreshAvailableBackups();

    return ok;
}

bool BackupTool::restoreBackup(const QString &filename)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + filename + QUARTERMASTER_BACKUP_FILE_SUFFIX);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream in(&file);

    // read file magic
    quint32 magic;
    in >> magic;

    if (magic != quint32(QUARTERMASTER_BACKUP_FILE_MAGIC))
        return false;

    // read version
    quint16 version;
    in >> version;

    while (!in.atEnd()) {
        quint16 option;
        in >> option;

        QByteArray data;
        in >> data;

#ifdef QT_DEBUG
        qDebug() << option;
        qDebug() << data.size();
#endif

        QString path;

        // wifi
        if (option == BackupTool::BackupWifi) {
            path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + QStringLiteral("/networks.json");
        }

        if (path.isEmpty())
            continue;

        writeFile(path, qUncompress(data));
    }

    file.close();

    return true;
}

QStringList BackupTool::availableBackups() const
{
    return m_availableBackups;
}

bool BackupTool::busy() const
{
    return m_busy;
}

void BackupTool::setAvailableBackups(const QStringList &backups)
{
    if (m_availableBackups == backups)
        return;

    m_availableBackups = backups;
    emit availableBackupsChanged(m_availableBackups);
}

void BackupTool::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged(m_busy);
}

void BackupTool::refreshAvailableBackups()
{
    QStringList backups;

    QDirIterator it(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                    QDir::Files);


    while (it.hasNext()) {
        const QString file = it.next();

        if (!file.endsWith(QUARTERMASTER_BACKUP_FILE_SUFFIX))
            continue;

        backups.append(QFileInfo(file).baseName());
    }

    setAvailableBackups(backups);
}

bool BackupTool::writeFile(const QString &path, const QByteArray &data)
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(data);

    file.close();

    return true;
}
