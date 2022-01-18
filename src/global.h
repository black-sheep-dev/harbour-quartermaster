#ifndef GLOBAL_H
#define GLOBAL_H

#include <QStandardPaths>

static const QString DEFAULT_CONFIG_PATH = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/org.nubecula/quartermaster/");
static const QString DEPRECATED_CONFIG_PATH = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/harbour-quartermaster/");

static const QString DEFAULT_CONFIG_FILE = DEFAULT_CONFIG_PATH + QStringLiteral("quartermaster.conf");
static const QString DEPRECATED_CONFIG_FILE = DEPRECATED_CONFIG_PATH + QStringLiteral("harbour-quartermaster.conf");

#endif // GLOBAL_H
