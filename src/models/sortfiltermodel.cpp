#include "sortfiltermodel.h"

SortFilterModel::SortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

void SortFilterModel::sortModel(Qt::SortOrder order)
{
    this->sort(0, order);
}
