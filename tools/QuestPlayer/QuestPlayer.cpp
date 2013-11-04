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

#include "Parser.h"

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
    QRegularExpression expExp("\\{(.+?)\\}");
    int deltaPos = 0;

    QRegularExpressionMatchIterator i = valueExp.globalMatch(str);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = QString::number(eval(match.captured(0), m_parameters));
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }
    deltaPos = 0;
    QString str2 = result;
    i = expExp.globalMatch(str2);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = QString::number(eval(match.captured(1), m_parameters));
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
        int32_t value = eval(start, std::map<uint32_t, int32_t>());
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

    m_oldParameters = m_parameters;

    for (const QM::Modifier & m : l.modifiers)
    {
        applyModifier(m);
    }

    checkTransitions();
    reduceTransitions();

    if (!l.empty)
    {
        if (l.descriptionExpression && !l.expression.empty())
        {
            int32_t t = eval(QString::fromStdWString(l.expression), m_parameters);
            if ((t > 10) || (l.descriptions.at(t - 1).empty()))
            {
                qCritical() << "Invalid location description selection in location " << m_currentLocation;
                m_locationText = "";
            }
            else
                m_locationText = substituteValues(QString::fromStdWString(l.descriptions.at(t - 1)));
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
    }

    if (l.empty && (m_possibleTransitions.size() == 1))
        startTransition(m_possibleTransitions.front());
    else
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

bool QuestPlayer::checkCondition(const QM::Transition::Condition& c) const
{
    int32_t param = m_parameters.at(c.param);

    if ((param < c.rangeFrom) || (param > c.rangeTo))
        return false;

    if (c.param == 2)
    {
        param = param;
    }

    if (c.includeValues)
    {
        if ((c.values.size()) && (std::find(c.values.begin(), c.values.end(), param) == c.values.end()))
            return false;
    }
    else
    {
        if ((c.values.size()) && (std::find(c.values.begin(), c.values.end(), param) != c.values.end()))
            return false;
    }

    if (c.includeMultiples)
    {
        for (int32_t m : c.multiples)
        {
            if ((param % m) != 0)
                return false;
        }
    }
    else
    {
        for (int32_t m : c.multiples)
        {
            if ((param % m) == 0)
                return false;
        }
    }
    return true;
}

void QuestPlayer::checkTransitions()
{
    m_possibleTransitions.clear();
    const QM::Location &l = m_quest.locations.at(m_currentLocation);

    int i = 0;
    for (const QM::Transition & t : l.transitions)
    {
        bool cond = true;
        for (const QM::Transition::Condition & c : t.conditions)
        {
            cond = cond && checkCondition(c);
        }
        bool passed = (t.passCount == 0) || !((m_transitionCounts.find(t.id) != m_transitionCounts.end()) && (m_transitionCounts.at(t.id) >= t.passCount));
        if (passed && cond && (t.globalCondition.empty() || eval(QString::fromStdWString(t.globalCondition), m_parameters)))
        {
            m_possibleTransitions.push_back(i);
        }
        i++;
    }
}

std::map<uint32_t, QString> QuestPlayer::visibleTransitions()
{
    std::map<uint32_t, QString> r;
    for (uint32_t t : m_possibleTransitions)
    {
        //TODO: Empty transitions
        r[t] = QString::fromStdWString(m_quest.locations.at(m_currentLocation).transitions.at(t).title);
    }
    return r;
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

    int32_t value;

    switch (m.operation)
    {
    case QM::Modifier::OPERATION_ASSIGN:
        value = m.value;
        break;
    case QM::Modifier::OPERATION_CHANGE:
        value = m_oldParameters[m.param] + m.value;
        break;
    case QM::Modifier::OPERATION_PERCENT:
        value = m_oldParameters[m.param] + (m.value * m_oldParameters[m.param]) / 100;
        break;
    case QM::Modifier::OPERATION_EXPRESSION:
        if (!m.expression.empty())
            value = eval(QString::fromStdWString(m.expression), m_oldParameters);
        else
            value = m_oldParameters[m.param];
        break;
    }

    m_parameters[m.param] = std::min(value, m_quest.parameters.at(m.param).max);
    m_parameters[m.param] = std::max(m_parameters[m.param], m_quest.parameters.at(m.param).min);
}

void QuestPlayer::startTransition(uint32_t num)
{
    const QM::Location &l = m_quest.locations.at(m_currentLocation);
    if (l.transitions.size() <= num)
        return;

    m_currentTransition = num;
    const QM::Transition &t = l.transitions.at(num);

    m_oldParameters = m_parameters;

    for (const QM::Modifier & m : t.modifiers)
    {
        applyModifier(m);
    }

    m_locationText = QString::fromStdWString(t.description);

    if (!m_quest.locations.at(t.from).empty && m_quest.locations.at(t.to).empty)
    {
        finishTransition();
    }
    else if (t.description.empty())
    {
        finishTransition();
    }
    else
    {
        emit(transitionText(QString::fromStdWString(t.description)));
    }
}

void QuestPlayer::finishTransition()
{
    const QM::Location &l = m_quest.locations.at(m_currentLocation);
    if (l.transitions.size() <= m_currentTransition)
        return;

    const QM::Transition &t = l.transitions.at(m_currentTransition);

    setLocation(t.to);
}

//FIXME: Optimize
void QuestPlayer::reduceTransitions()
{
    std::map<std::wstring, std::list<uint32_t> > transitions;
    const QM::Location &l = m_quest.locations.at(m_currentLocation);

    for (uint32_t t : m_possibleTransitions)
    {
        const QM::Transition& tr = l.transitions.at(t);
        transitions[tr.title].push_back(t);
    }

    m_possibleTransitions.clear();

    for (std::pair<std::wstring, std::list<uint32_t> > p : transitions)
    {
        if (p.second.size() == 1)
        {
            m_possibleTransitions.push_back(p.second.front());
            continue;
        }
        double range = 0;

        for (uint32_t t : p.second)
        {
            range += l.transitions.at(t).priority;
        }

        uint32_t randRange = range * 1000;
        uint32_t r = rand() % randRange;
        uint32_t counter;
        uint32_t selected;

        for (uint32_t t : p.second)
        {
            counter += l.transitions.at(t).priority * 1000;
            if (counter >= r)
                selected = t;
        }
        m_possibleTransitions.push_back(selected);
    }
}
}
}