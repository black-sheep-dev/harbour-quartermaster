#include "climate.h"

Climate::Climate(QObject *parent) :
    Entity(parent)
{
    setType(Entity::Climate);
}

bool Climate::hasHvacMode(quint16 mode) const
{
    return m_hvacModes & mode;
}

bool Climate::hasPresetMode(quint16 mode) const
{
    return m_presetModes & mode;
}

quint16 Climate::hvacModes() const
{
    return m_hvacModes;
}

quint16 Climate::presetModes() const
{
    return m_presetModes;
}

void Climate::parseAttributes()
{
    // hvac modes
    const QStringList itemsHvac = attributes().value(QStringLiteral("hvac_modes")).toStringList();
    for (const auto &item : itemsHvac) {
        if (item == QLatin1String("auto"))
            m_hvacModes |= HVAC_MODE_AUTO;
        else if (item == QLatin1String("cool"))
            m_hvacModes |= HVAC_MODE_COOL;
        else if (item == QLatin1String("dry"))
            m_hvacModes |= HVAC_MODE_DRY;
        else if (item == QLatin1String("fan_only"))
            m_hvacModes |= HVAC_MODE_FAN_ONLY;
        else if (item == QLatin1String("heat"))
            m_hvacModes |= HVAC_MODE_HEAT;
        else if (item == QLatin1String("heat_cool"))
            m_hvacModes |= HVAC_MODE_HEAT_COOL;
        else if (item == QLatin1String("off"))
            m_hvacModes |= HVAC_MODE_OFF;
    }

    // preset modes
    const QStringList itemsPreset = attributes().value(QStringLiteral("preset_modes")).toStringList();
    for (const auto &item : itemsPreset) {
        if (item == QLatin1String("activity"))
            m_presetModes |= PresetActivity;
        else if (item == QLatin1String("away"))
            m_presetModes |= PresetAway;
        else if (item == QLatin1String("boost"))
            m_presetModes |= PresetBoost;
        else if (item == QLatin1String("comfort"))
            m_presetModes |= PresetComfort;
        else if (item == QLatin1String("eco"))
            m_presetModes |= PresetEco;
        else if (item == QLatin1String("home"))
            m_presetModes |= PresetHome;
        else if (item == QLatin1String("sleep"))
            m_presetModes |= PresetSleep;
    }
}
