#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <QAbstractListModel>

#include "src/entities/entity.h"

class EntityModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    enum EntityRole {
        AttributesRole          = Qt::UserRole + 1,
        ContextRole,
        EntityIdRole,
        NameRole,
        StateRole,
        TypeRole
    };
    Q_ENUM(EntityRole)

    explicit EntityModel(QObject *parent = nullptr);
    ~EntityModel() override;

    Q_INVOKABLE Entity *entityAt(int index);
    QList<Entity *> entities() const;
    bool isEmpty() const;

    void addEntity(Entity *entity);
    void setEntities(const QList<Entity *> &entities);
    void updateEntity(Entity *entity);

    // properties
    bool loading() const;

signals:
    // properties
    void loadingChanged(bool loading);

public slots:
    void reset();

    // properties
    void setLoading(bool loading);

private:
    QList<Entity *> m_entities;

    // properties
    bool m_loading{false};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ENTITYMODEL_H
