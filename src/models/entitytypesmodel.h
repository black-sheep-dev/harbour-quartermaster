#ifndef ENTITYTYPESMODEL_H
#define ENTITYTYPESMODEL_H

#include <QAbstractListModel>


struct EntityTypeItem {
    QString description;
    QString title;
    QString icon;
    int type{0};
};

class EntityTypesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EntityTypeRoles {
        TitleRole           = Qt::UserRole + 1,
        DescriptionRole,
        IconRole,
        TypeRole
    };
    Q_ENUM(EntityTypeRoles)

    explicit EntityTypesModel(QObject *parent = nullptr);

    void addItem(const EntityTypeItem &item);
    void setItems(const QList<EntityTypeItem> &items);

private:
    QList<EntityTypeItem> m_items;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ENTITYTYPESMODEL_H
