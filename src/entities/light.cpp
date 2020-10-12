#include "light.h"

#include <QColor>

Light::Light(QObject *parent) :
    Entity(parent)
{
    setType(Entity::Light);
}

int Light::blue() const
{
    return m_color.blue();
}

int Light::green() const
{
    return m_color.green();
}

int Light::red() const
{
    return m_color.red();
}

QColor Light::color() const
{
    return m_color;
}

void Light::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

void Light::parseAttributes()
{
    const QVariantList rgb = attributes().value(QStringLiteral("rgb_color")).toList();

    setColor(QColor(
                 rgb.value(0).toInt(),
                 rgb.value(1).toInt(),
                 rgb.value(2).toInt()
                 ));
}
