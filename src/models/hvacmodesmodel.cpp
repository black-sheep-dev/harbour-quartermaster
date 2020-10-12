#include "hvacmodesmodel.h"

HvacModesModel::HvacModesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

quint16 HvacModesModel::hvacModes() const
{
    return m_hvacModes;
}

void HvacModesModel::setHvacModes(quint16 hvacModes)
{
    beginResetModel();

    m_hvacModes = hvacModes;

    // clear data
    m_data.clear();

    // add none by default
    m_data.insert(tr("None"), Climate::HVAC_MODE_NONE);

    // add modes
    if (m_hvacModes & Climate::HVAC_MODE_AUTO)
        m_data.insert(tr("Auto"), Climate::HVAC_MODE_AUTO);

    if (m_hvacModes & Climate::HVAC_MODE_COOL)
        m_data.insert(tr("Cool"), Climate::HVAC_MODE_COOL);

    if (m_hvacModes & Climate::HVAC_MODE_DRY)
        m_data.insert(tr("Dry"), Climate::HVAC_MODE_DRY);

    if (m_hvacModes & Climate::HVAC_MODE_FAN_ONLY)
        m_data.insert(tr("Fan only"), Climate::HVAC_MODE_FAN_ONLY);

    if (m_hvacModes & Climate::HVAC_MODE_HEAT)
        m_data.insert(tr("Heat"), Climate::HVAC_MODE_HEAT);

    if (m_hvacModes & Climate::HVAC_MODE_HEAT_COOL)
        m_data.insert(tr("Heat/Cool"), Climate::HVAC_MODE_HEAT_COOL);

    if (m_hvacModes & Climate::HVAC_MODE_OFF)
        m_data.insert(tr("Off"), Climate::HVAC_MODE_OFF);

    endResetModel();
}

int HvacModesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.count();
}

QVariant HvacModesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case KeyRole:
        return m_data.keys().at(index.row());

    case ValueRole:
        return m_data.values().at(index.row());

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> HvacModesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[KeyRole]      = "key";
    roles[ValueRole]    = "value";

    return roles;
}
