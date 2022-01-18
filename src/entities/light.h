#ifndef LIGHT_H
#define LIGHT_H

#include "entity.h"

#include <QColor>

class Light : public Entity
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(qreal saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)

public:
    enum LightFeature {
        LightNone                   = 0x00,
        LightBrightness             = 0x01,
        LightColorTemp              = 0x02,
        LightEffect                 = 0x04,
        LightFlash                  = 0x08,
        LightColor                  = 0x10,
        LightTransition             = 0x20,
        LightWhiteValue             = 0x80
    };
    Q_ENUM(LightFeature)
    Q_DECLARE_FLAGS(LightFeatures, LightFeature)

    explicit Light(QObject *parent = nullptr);

    Q_INVOKABLE int blue() const;
    Q_INVOKABLE int green() const;
    Q_INVOKABLE int red() const;

    // properties
    const QColor &color() const;
    qreal hue() const;
    qreal saturation() const;

signals:
    // properties
    void colorChanged();   
    void hueChanged();

    void saturationChanged();

public slots:
    // properties
    void setColor(const QColor &color);
    void setHue(qreal hue);
    void setSaturation(qreal saturation);
private:
    // properties
    QColor m_color;
    qreal m_hue{0};
    qreal m_saturation{0};

    // Entity interface
public:
    void parseAttributes() override;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Light::LightFeatures)

#endif // LIGHT_H
