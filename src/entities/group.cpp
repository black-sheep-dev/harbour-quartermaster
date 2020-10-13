#include "group.h"

Group::Group(QObject *parent) :
    Entity(parent),
    m_childrenModel(new EntitiesModel(this))
{
    setType(Entity::Group);
    m_childrenModel->setParentMode(false);
}

EntitiesModel *Group::childrenModel()
{
    return m_childrenModel;
}
