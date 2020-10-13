#ifndef ENTITIESMODEL_H
#define ENTITIESMODEL_H

#include <QAbstractListModel>

#include "src/entities/entity.h"

class EntitiesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EntityRole {
        AttributesRole          = Qt::UserRole + 1,
        ContextRole,
        EntityIdRole,
        NameRole,
        StateRole,
        SupportedFeaturesRole,
        TypeRole
    };
    Q_ENUM(EntityRole)

    explicit EntitiesModel(QObject *parent = nullptr);
    ~EntitiesModel() override;

    Q_INVOKABLE int entitiesCount() const;
    Q_INVOKABLE Entity *entityById(const QString &entityId);
    Q_INVOKABLE Entity *entityAt(int index);
    Entity *entityAt(const QModelIndex &index);
    QList<Entity *> entities() const;
    bool isEmpty() const;
    void setParentMode(bool enable);


    void addEntity(Entity *entity);
    void setEntities(const QList<Entity *> &entities);
    void updateEntity(Entity *entity);

signals:
    void changed();

public slots:
    void reset();

private slots:
    void onEntityChanged();

private:
    QList<Entity *> m_entities;
    bool m_parentMode{true};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ENTITIESMODEL_H
