#include "wifinetwork.h"

WifiNetwork::WifiNetwork(QObject *parent) :
    QObject(parent),
    m_identifier(QString()),
    m_name(QString()),
    m_selected(false)
{

}

QString WifiNetwork::identifier() const
{
    return m_identifier;
}

QString WifiNetwork::name() const
{
    return m_name;
}

bool WifiNetwork::selected() const
{
    return m_selected;
}

void WifiNetwork::setIdentifier(const QString &identifier)
{
    if (m_identifier == identifier)
        return;

    m_identifier = identifier;
    emit identifierChanged(m_identifier);
}

void WifiNetwork::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void WifiNetwork::setSelected(bool selected)
{
    if (m_selected == selected)
        return;

    m_selected = selected;
    emit selectedChanged(m_selected);
}
