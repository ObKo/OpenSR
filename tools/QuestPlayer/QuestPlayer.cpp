/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "QuestPlayer.h"
#include <fstream>

#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

namespace Rangers
{
namespace QuestPlayer
{

QuestPlayer::QuestPlayer(QObject *parent): QObject(parent), m_currentLocation(0)
{

}

QuestPlayer::~QuestPlayer()
{

}

QM::Location QuestPlayer::currentLocation() const
{
    if (m_quest.locations.find(m_currentLocation) != m_quest.locations.end())
        return m_quest.locations.at(m_currentLocation);
    else
        return QM::Location();
}

QString QuestPlayer::substituteValues(const QString &str) const
{
    //TODO: Use <regex>
    QString result = str;
    QRegularExpression valueExp("\\[p(\\d)+\\]");

    int deltaPos = 0;

    QRegularExpressionMatchIterator i = valueExp.globalMatch(str);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        uint32_t v = match.captured(1).toUInt();
        auto i = m_parameters.find(v);
        if (i == m_parameters.end())
            continue;
        QString value = QString::number(i->second);
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }

    result.replace("<ToStar>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.toStar) + "</font>");
    result.replace("<ToPlanet>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.toPlanet) + "</font>");
    result.replace("<FromStar>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.fromStar) + "</font>");
    result.replace("<FromPlanet>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.fromPlanet) + "</font>");
    result.replace("<Date>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.date) + "</font>");
    result.replace("<Money>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.money) + "</font>");
    result.replace("<Ranger>", "<font color=\"blue\">" + QString::fromStdWString(m_quest.ranger) + "</font>");

    result.replace("<clr>", "<font color=\"blue\">");
    result.replace("<clrEnd>", "</font>");

    return result;
}

void QuestPlayer::loadQuest(const QString& file)
{
    m_parametersVisibility.clear();
    m_parameters.clear();
    m_transitionCounts.clear();
    m_locationDescriptionsCount.clear();

    std::ifstream f(file.toStdString());
    m_quest = Rangers::QM::readQuest(f);

    for (const std::pair<uint32_t, QM::Parameter>& p : m_quest.parameters)
    {
        QString start = QString::fromStdWString(p.second.start);
        int32_t value = start.mid(1, start.length() - 2).toInt();
        m_parameters[p.first] = value;
        if (!value && !p.second.showOnZero)
            m_parametersVisibility[p.first] = false;
        else
            m_parametersVisibility[p.first] = true;
    }

    setLocation(m_quest.startLocation);
}

void QuestPlayer::setLocation(uint32_t location)
{
    if (m_quest.locations.find(location) == m_quest.locations.end())
    {
        qWarning() << "Invalid location: " << location;
        return;
    }
    m_currentLocation = location;
    const Rangers::QM::Location &l = m_quest.locations.at(location);

    //TODO: Formulas
    if (l.descriptionExpression)
    {
        m_locationText = substituteValues(QString::fromStdWString(l.descriptions.at(0)));
    }
    else
    {
        uint32_t value;
        if (m_locationDescriptionsCount.find(location) == m_locationDescriptionsCount.end())
        {
            m_locationDescriptionsCount[location] = 0;
            value = 0;
        }
        else
            value = m_locationDescriptionsCount[location];

        value = (value + 1) % 10;
        for (int i = 0; (i < 10) && (l.descriptions.at(value).empty()); i++)
            value = (value + 1) % 10;

        m_locationText = substituteValues(QString::fromStdWString(l.descriptions.at(value)));
    }

    for (const QM::Modifier & m : l.modifiers)
    {
        applyModifier(m);
    }

    emit(locationChanged());
}

QString QuestPlayer::locationText() const
{
    return m_locationText;
}

QStringList QuestPlayer::visibleParameters() const
{
    QStringList params;
    for (const std::pair<uint32_t, int32_t>& p : m_parameters)
    {
        if (m_parametersVisibility.at(p.first))
        {
            QString value;
            for (const Rangers::QM::Parameter::Range & r : m_quest.parameters.at(p.first).ranges)
            {
                if ((p.second >= r.from) && (p.second <= r.to))
                {
                    value = QString::fromStdWString(r.text).replace("<>", QString::number(p.second));
                    break;
                }
            }
            params.push_back(substituteValues(value));
        }
    }
    return params;
}

void QuestPlayer::applyModifier(const QM::Modifier& m)
{
    switch (m.visibility)
    {
    case QM::Modifier::VISIBILITY_HIDE:
        m_parametersVisibility[m.param] = false;
        break;
    case QM::Modifier::VISIBILITY_SHOW:
        m_parametersVisibility[m.param] = true;
        break;
    case QM::Modifier::VISIBILITY_NO_CHANGE:
        break;
    }

    switch (m.operation)
    {
    case QM::Modifier::OPERATION_ASSIGN:
        m_parameters[m.param] = m.value;
        break;
    case QM::Modifier::OPERATION_CHANGE:
        m_parameters[m.param] += m.value;
        break;
    case QM::Modifier::OPERATION_PERCENT:
        m_parameters[m.param] += (m.value * m_parameters[m.param]) / 100;
        break;
    case QM::Modifier::OPERATION_EXPRESSION:
        //TODO: Expressions
        break;
    }
}
}
}