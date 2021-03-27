#ifndef GROUP_H
#define GROUP_H

#include "entity.h"

#include "src/models/entitiesmodel.h"

class Group : public Entity
{
    Q_OBJECT
public:
    explicit Group(QObject *parent = nullptr);

    Q_INVOKABLE EntitiesModel *childrenModel();

private:
    EntitiesModel *m_childrenModel{new EntitiesModel(this)};
};

#endif // GROUP_H
