#include "group.h"

Group::Group(QObject *parent) :
    Entity(parent)
{
    setType(Entity::Group);
    m_childrenModel->setParentMode(false);
}

EntitiesModel *Group::childrenModel()
{
    return m_childrenModel;
}
