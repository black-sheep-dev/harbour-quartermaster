#ifndef ACCESSPOINT_H
#define ACCESSPOINT_H

#include <QMetaType>
#include <QString>

struct AccessPoint {
    QString identifier;
    QString name;

    AccessPoint() = default;
    AccessPoint(const AccessPoint &other) {
        identifier = other.identifier;
        name = other.name;
    };

    bool operator==(const AccessPoint &other) const {
        return identifier == other.identifier
                && name == other.name;
    };
    bool operator!=(const AccessPoint &other) const {
        return !(*this == other);
    };
};
Q_DECLARE_METATYPE(AccessPoint)

#endif // ACCESSPOINT_H
