#ifndef CLIMATE_H
#define CLIMATE_H

#include "entity.h"

class Climate : public Entity
{
    Q_OBJECT

    Q_PROPERTY(quint16 hvacModes READ hvacModes NOTIFY hvacModesChanged)
    Q_PROPERTY(quint16 presetModes READ presetModes NOTIFY presetModesChanged)

public:    
    enum ClimateFeature {
        ClimateNone                 = 0x00,
        ClimateTemperature          = 0x01,
        ClimateTemperatureRange     = 0x02,
        ClimateHumidity             = 0x04,
        ClimateFanMode              = 0x08,
        ClimatePresetMode           = 0x10,
        ClimateSwingMode            = 0x20,
        ClimateAuxMode              = 0x40
    };
    Q_ENUM(ClimateFeature)
    Q_DECLARE_FLAGS(ClimateFeatures, ClimateFeature)

    enum FanMode {
        FanOff,
        FanOn,
        FanAuto,
        FanLow,
        FanMedium,
        FanHigh,
        FanMiddle,
        FanFocus,
        FanDiffuse
    };
    Q_ENUM(FanMode)

    enum HVAC_Action {
        CURRENT_HVAC_OFF,
        CURRENT_HVAC_HEAT,
        CURRENT_HVAC_COOL,
        CURRENT_HVAC_DRY,
        CURRENT_HVAC_IDLE
    };
    Q_ENUM(HVAC_Action)

    enum HVAC_Mode {
        HVAC_MODE_NONE              = 0x00,
        HVAC_MODE_OFF               = 0x01,
        HVAC_MODE_HEAT              = 0x02,
        HVAC_MODE_COOL              = 0x04,
        HVAC_MODE_HEAT_COOL         = 0x08,
        HVAC_MODE_AUTO              = 0x10,
        HVAC_MODE_DRY               = 0x20,
        HVAC_MODE_FAN_ONLY          = 0x40
    };
    Q_ENUM(HVAC_Mode)
    Q_DECLARE_FLAGS(HVAC_Modes, HVAC_Mode)


    enum Preset {
        PresetNone                  = 0x00,
        PresetEco                   = 0x01,
        PresetAway                  = 0x02,
        PresetBoost                 = 0x04,
        PresetComfort               = 0x08,
        PresetHome                  = 0x10,
        PresetSleep                 = 0x20,
        PresetActivity              = 0x40
    };
    Q_ENUM(Preset)
    Q_DECLARE_FLAGS(Presets, Preset)

    enum SwingModes {
        SwingOff,
        SwingVertical,
        SwingHorizontal,
        SwingBoth
    };
    Q_ENUM(SwingModes)

    explicit Climate(QObject *parent = nullptr);

    Q_INVOKABLE bool hasHvacMode(quint16 mode) const;
    Q_INVOKABLE bool hasPresetMode(quint16 mode) const;

    // properties
    quint16 hvacModes() const;
    quint16 presetModes() const;

signals:
    void hvacModesChanged(quint16 hvacModes);
    void presetModesChanged(quint16 presetModes);

private:
    quint16 m_hvacAction{CURRENT_HVAC_OFF};
    quint16 m_hvacModes{HVAC_MODE_NONE};
    quint16 m_presetMode{PresetNone};
    quint16 m_presetModes{PresetNone};
    quint16 m_swingModes{SwingOff};

    // Entity interface
public:
    void parseAttributes() override;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(Climate::ClimateFeatures)
Q_DECLARE_OPERATORS_FOR_FLAGS(Climate::HVAC_Modes)
Q_DECLARE_OPERATORS_FOR_FLAGS(Climate::Presets)

#endif // CLIMATE_H
