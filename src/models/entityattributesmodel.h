#ifndef ENTITYATTRIBUTESMODEL_H
#define ENTITYATTRIBUTESMODEL_H

#include <QAbstractListModel>

#include "src/entities/entity.h"

class EntityAttributesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Entity* entity READ entity WRITE setEntity NOTIFY entityChanged)

public:
    enum DetailRoles {
        KeyRole       = Qt::UserRole + 1,
        ValueRole
    };
    Q_ENUM(DetailRoles)

    explicit EntityAttributesModel(QObject *parent = nullptr);

    Entity *entity() const;

signals:
    void entityChanged(Entity* entity);

public slots:
    void setEntity(Entity *entity);

private:
    Entity *m_entity{nullptr};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ENTITYATTRIBUTESMODEL_H
