#ifndef ENTITIESSORTFILTERMODEL_H
#define ENTITIESSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class EntitiesSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EntitiesSortFilterModel(QObject *parent = nullptr); 

    Q_INVOKABLE void sortModel();

    // QAbstractProxyModel interface
public:
    void setSourceModel(QAbstractItemModel *sourceModel) override;
};

#endif // ENTITIESSORTFILTERMODEL_H
