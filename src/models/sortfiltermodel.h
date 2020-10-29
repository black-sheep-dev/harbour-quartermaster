#ifndef SORTFILTERMODEL_H
#define SORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class SortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SortFilterModel(QObject *parent = nullptr);

    Q_INVOKABLE void setSortOrder(Qt::SortOrder order);
    Q_INVOKABLE void sortModel();

private:
    Qt::SortOrder m_sortOrder{Qt::AscendingOrder};

    // QAbstractProxyModel interface
public:
    void setSourceModel(QAbstractItemModel *sourceModel) override;
};

#endif // SORTFILTERMODEL_H
