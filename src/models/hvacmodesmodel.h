#ifndef HVACMODESMODEL_H
#define HVACMODESMODEL_H

#include <QAbstractListModel>

#include "src/entities/climate.h"

class HvacModesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(quint16 hvacModes READ hvacModes WRITE setHvacModes)

public:
    enum HvacModesRole {
        KeyRole,
        ValueRole
    };
    Q_ENUM(HvacModesRole)

    explicit HvacModesModel(QObject *parent = nullptr);

    quint16 hvacModes() const;


public slots:
    void setHvacModes(quint16 hvacModes);

private:
    QMap<QString, quint16> m_data;
    quint16 m_hvacModes{Climate::HVAC_MODE_NONE};

public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // HVACMODESMODEL_H
