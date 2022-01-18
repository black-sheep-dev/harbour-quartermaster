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

const QColor &Light::color() const
{
    return m_color;
}

void Light::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged();
}

void Light::parseAttributes()
{
    const QVariantList rgb = attributes().value(QStringLiteral("rgb_color")).toList();

    setColor(QColor(
                 rgb.value(0).toInt(),
                 rgb.value(1).toInt(),
                 rgb.value(2).toInt()
                 ));

    if (attributes().value(QStringLiteral("hs_color")).toList().count() != 2) {
        return;
    }

    setHue(attributes().value(QStringLiteral("hs_color")).toList().first().toDouble() / 255);
    setSaturation(attributes().value(QStringLiteral("hs_color")).toList().last().toDouble() / 255);
}

qreal Light::hue() const
{
    return m_hue;
}

void Light::setHue(qreal hue)
{
    if (qFuzzyCompare(m_hue, hue))
        return;
    m_hue = hue;
    emit hueChanged();
}

qreal Light::saturation() const
{
    return m_saturation;
}

void Light::setSaturation(qreal saturation)
{
    if (qFuzzyCompare(m_saturation, saturation))
        return;
    m_saturation = saturation;
    emit saturationChanged();
}
