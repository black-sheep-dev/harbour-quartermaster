#ifndef SORTFILTERMODEL_H
#define SORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class SortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SortFilterModel(QObject *parent = nullptr);

    Q_INVOKABLE void sortModel(Qt::SortOrder order = Qt::AscendingOrder);
};

#endif // SORTFILTERMODEL_H
