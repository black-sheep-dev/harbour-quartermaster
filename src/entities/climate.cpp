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
    for (const QString &item : attributes().value(QStringLiteral("hvac_modes")).toStringList()) {
        if (item == QStringLiteral("auto"))
            m_hvacModes |= HVAC_MODE_AUTO;
        else if (item == QStringLiteral("cool"))
            m_hvacModes |= HVAC_MODE_COOL;
        else if (item == QStringLiteral("dry"))
            m_hvacModes |= HVAC_MODE_DRY;
        else if (item == QStringLiteral("fan_only"))
            m_hvacModes |= HVAC_MODE_FAN_ONLY;
        else if (item == QStringLiteral("heat"))
            m_hvacModes |= HVAC_MODE_HEAT;
        else if (item == QStringLiteral("heat_cool"))
            m_hvacModes |= HVAC_MODE_HEAT_COOL;
        else if (item == QStringLiteral("off"))
            m_hvacModes |= HVAC_MODE_OFF;
    }

    // preset modes
    for (const QString &item : attributes().value(QStringLiteral("preset_modes")).toStringList()) {
        if (item == QStringLiteral("activity"))
            m_presetModes |= PresetActivity;
        else if (item == QStringLiteral("away"))
            m_presetModes |= PresetAway;
        else if (item == QStringLiteral("boost"))
            m_presetModes |= PresetBoost;
        else if (item == QStringLiteral("comfort"))
            m_presetModes |= PresetComfort;
        else if (item == QStringLiteral("eco"))
            m_presetModes |= PresetEco;
        else if (item == QStringLiteral("home"))
            m_presetModes |= PresetHome;
        else if (item == QStringLiteral("sleep"))
            m_presetModes |= PresetSleep;
    }
}
