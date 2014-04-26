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

#include <OpenSR/QM/Parser.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <OpenSR/libRanger.h>

#include <boost/algorithm/string.hpp>

#include <fstream>

namespace Rangers
{
namespace World
{

QuestPlayer::QuestPlayer()
{
}

QuestPlayer::~QuestPlayer()
{

}

std::string QuestPlayer::substituteValues(const std::string &str) const
{
    std::string result = str;

    //Okay, <regex> working bad on GCC, work manually
    int pos = 0;
    while ((pos = result.find("{", pos)) != std::string::npos)
    {
        int endPos = result.find("}", pos);
        if (endPos == std::string::npos)
            endPos = result.length() - 1;

        std::string exp = result.substr(pos + 1, endPos - pos - 1);
        std::string expResult = "\\cS" + std::to_string(int(QM::eval(exp, m_parameters))) + "\\cR";
        result.replace(pos, endPos - pos + 1, expResult);
        pos += expResult.length();
    }

    pos = 0;
    while ((pos = result.find("[p", pos)) != std::string::npos)
    {
        int endPos = result.find("]", pos);
        if (endPos == std::string::npos)
            endPos = result.length() - 1;
        try
        {
            uint32_t param = std::stoul(result.substr(pos + 2, endPos - pos));

            std::string paramValue;
            auto i = m_parameters.find(param);
            if (i != m_parameters.end())
                paramValue = "\\cS" + std::to_string(int((*i).second)) + "\\cR";
            result.replace(pos, endPos - pos + 1, paramValue);
            pos += paramValue.length();
        }
        catch (std::logic_error &e)
        {
            pos = endPos;
        }
    }

    /*QRegularExpression valueExp("\\[p(\\d)+\\]");
    QRegularExpression expExp("\\{(.+?)\\}");
    int deltaPos = 0;

    QRegularExpressionMatchIterator i = expExp.globalMatch(str);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = "<font color=\"blue\">" + QString::number((int32_t)round(eval(match.captured(1).toStdString(), m_parameters))) + "</font>";
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }
    deltaPos = 0;
    QString str2 = result;
    i = valueExp.globalMatch(str2);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        QString value = "<font color=\"blue\">" + QString::number((int32_t)round(eval(match.captured(0).toStdString(), m_parameters))) + "</font>";
        result.replace(match.capturedStart() + deltaPos, match.capturedLength(), value);
        deltaPos += value.length() - match.capturedLength();
    }*/

    boost::algorithm::replace_all(result, "<ToStar>", "\\cS" + m_quest.toStar + "\\cR");
    boost::algorithm::replace_all(result, "<ToPlanet>", "\\cS" + m_quest.toPlanet + "\\cR");
    boost::algorithm::replace_all(result, "<FromStar>", "\\cS" + m_quest.fromStar + "\\cR");
    boost::algorithm::replace_all(result, "<FromPlanet>", "\\cS" + m_quest.fromPlanet + "\\cR");
    boost::algorithm::replace_all(result, "<Date>", "\\cS" + m_quest.date + "\\cR");
    boost::algorithm::replace_all(result, "<Money>", "\\cS" + m_quest.money + "\\cR");
    boost::algorithm::replace_all(result, "<Ranger>", "\\cS" + m_quest.ranger + "\\cR");

    boost::algorithm::replace_all(result, "<clr>", "\\cS");
    boost::algorithm::replace_all(result, "<clrEnd>", "\\cR");

    return result;
}

void QuestPlayer::loadQuest(const std::string& file)
{
    boost::shared_ptr<std::istream> s = ResourceManager::instance().getFileStream(file);

    if (!s)
        return;

    m_quest = Rangers::QM::readQuest(*s);

    resetQuest();
}

void QuestPlayer::resetQuest()
{
    m_parametersVisibility.clear();
    m_parameters.clear();
    m_transitionCounts.clear();
    m_locationDescriptionsCount.clear();

    m_transition = false;
    m_completed = false;
    m_failed = false;
    m_death = false;

    m_currentImage = 0;

    for (const std::pair<uint32_t, QM::Parameter>& p : m_quest.parameters)
    {
        float value = QM::eval(p.second.start, std::map<uint32_t, float>());
        m_parameters[p.first] = value;
        m_parametersVisibility[p.first] = true;
    }

    setLocation(m_quest.startLocation);
}

void QuestPlayer::setLocation(uint32_t location)
{
    if (m_quest.locations.find(location) == m_quest.locations.end())
    {
        Log::error() << "[Quest] Invalid location: " << location;
        return;
    }
    m_currentLocation = m_quest.locations.at(location);

    m_oldParameters = m_parameters;

    for (const QM::Modifier & m : m_currentLocation.modifiers)
    {
        applyModifier(m);
    }

    checkTransitions();
    reduceTransitions();

    if (!m_currentLocation.empty || m_currentTransition.description.empty())
    {
        if (m_currentLocation.descriptionExpression && !m_currentLocation.expression.empty())
        {
            int32_t t = (int32_t)QM::eval(m_currentLocation.expression, m_parameters);
            //FIXME: Bug or feature?
            if (t == 0)
                t = 1;
            if ((t < 1) || (t > 10) || (m_currentLocation.descriptions.at(t - 1).empty()))
            {
                Log::error() << "[Quest] Invalid location description selection in location " << m_currentLocation.id;
                m_currentText = std::string();
            }
            else
                m_currentText = substituteValues(m_currentLocation.descriptions.at(t - 1));
        }
        else
        {
            uint32_t value;
            if (m_locationDescriptionsCount.find(m_currentLocation.id) == m_locationDescriptionsCount.end())
            {
                m_locationDescriptionsCount[m_currentLocation.id] = 0;
                value = 0;
            }
            else
            {
                value = m_locationDescriptionsCount[m_currentLocation.id];
                value = (value + 1) % 10;
            }

            for (int i = 0; (i < 10) && (m_currentLocation.descriptions.at(value).empty()); i++)
                value = (value + 1) % 10;

            m_locationDescriptionsCount[m_currentLocation.id] = value;

            m_currentText = substituteValues(m_currentLocation.descriptions.at(value));
        }
    }

    Log::debug() << "[Quest] L" << m_currentLocation.id;
    auto img = m_locationImages.find(m_currentLocation.id);
    if (img != m_locationImages.end())
        m_currentImage = (*img).second;

    if ((m_possibleTransitions.size() == 1) && m_currentLocation.transitions[m_possibleTransitions.front()].title.empty() &&
            m_currentTransition.description.empty())
        transit(m_possibleTransitions.front());
    else if (m_currentLocation.type == QM::Location::LOCATION_SUCCESS)
        m_completed = true;
    else if (m_currentLocation.type == QM::Location::LOCATION_FAIL)
    {
        m_failed = true;
        m_death = m_currentLocation.death;
    }
    else if (checkCriticalParameters())
        m_transition = false;
}

bool QuestPlayer::checkCriticalParameters()
{
    for (const std::pair<uint32_t, int32_t>& p : m_parameters)
    {
        const QM::Parameter &pr = m_quest.parameters.at(p.first);

        if (pr.type == QM::Parameter::PARAMETER_NORMAL)
            continue;

        bool crit = false;

        if (pr.minCritical && p.second <= pr.min)
            crit = true;
        else if (!pr.minCritical && p.second >= pr.max)
            crit = true;

        if (crit)
        {
            m_currentText = pr.critText;
            switch (pr.type)
            {
            case QM::Parameter::PARAMETER_DEATH:
                m_death = true;
            case QM::Parameter::PARAMETER_FAIL:
                m_failed = true;
                break;
            case QM::Parameter::PARAMETER_SUCCESS:
                m_completed = true;
                break;
            }
            return false;
        }
    }
    return true;
}

std::string QuestPlayer::currentText() const
{
    return m_currentText;
}

std::vector<std::string> QuestPlayer::visibleParameters() const
{
    std::vector<std::string> params;
    for (const std::pair<uint32_t, float>& p : m_parameters)
    {
        if ((p.second != 0 || m_quest.parameters.at(p.first).showOnZero) && m_parametersVisibility.at(p.first))
        {
            std::string value;
            for (const Rangers::QM::Parameter::Range & r : m_quest.parameters.at(p.first).ranges)
            {
                int32_t v = (int32_t)round(p.second);
                if ((v >= r.from) && (v <= r.to))
                {
                    value = boost::algorithm::replace_all_copy(r.text, "<>", std::to_string(v));
                    break;
                }
            }
            if (value.empty())
                continue;
            params.push_back(substituteValues(value));
        }
    }
    return params;
}

bool QuestPlayer::checkCondition(const QM::Transition::Condition& c) const
{
    float param = m_parameters.at(c.param);

    if ((param < c.rangeFrom) || (param > c.rangeTo))
        return false;

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
            if ((fmod(param, m)) != 0)
                return false;
        }
    }
    else
    {
        for (int32_t m : c.multiples)
        {
            if ((fmod(param, m)) == 0)
                return false;
        }
    }
    return true;
}

void QuestPlayer::checkTransitions()
{
    m_possibleTransitions.clear();
    m_alwaysVisibleTransitions.clear();

    int i = 0;
    for (const QM::Transition & t : m_currentLocation.transitions)
    {
        bool cond = true;
        for (const QM::Transition::Condition & c : t.conditions)
        {
            cond = cond && checkCondition(c);
        }
        bool passed = (t.passCount == 0) || !((m_transitionCounts.find(t.id) != m_transitionCounts.end()) && (m_transitionCounts.at(t.id) >= t.passCount));
        if (passed && cond && (t.globalCondition.empty() || QM::eval(t.globalCondition, m_parameters)))
        {
            m_possibleTransitions.push_back(i);
        }
        else if (t.alwaysVisible)
            m_alwaysVisibleTransitions.push_back(i);
        i++;
    }
}

std::vector<std::tuple<uint32_t, std::string, bool> > QuestPlayer::visibleTransitions() const
{
    std::vector<std::tuple<uint32_t, std::string, bool > > r;

    if (!m_transition)
    {
        for (uint32_t t : m_possibleTransitions)
        {
            const QM::Transition &trans = m_currentLocation.transitions.at(t);

            if (!trans.title.empty())
                r.push_back(std::tuple<uint32_t, std::string, bool>(t, substituteValues(trans.title), true));
            else
                r.push_back(std::tuple<uint32_t, std::string, bool>(t, "Next", true));
        }

        for (uint32_t t : m_alwaysVisibleTransitions)
        {
            r.push_back(std::tuple<uint32_t, std::string, bool>(t, substituteValues(m_currentLocation.transitions.at(t).title), false));
        }

        std::sort(r.begin(), r.end(),
                  [&](const std::tuple<uint32_t, std::string, bool>& a, const std::tuple<uint32_t, std::string, bool>& b) -> bool
        {
            return m_currentLocation.transitions.at(std::get<0>(a)).position < m_currentLocation.transitions.at(std::get<0>(b)).position;
        });
    }
    else
    {
        r.push_back(std::tuple<uint32_t, std::string, bool>(0, "Next", true));
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

    float value;

    switch (m.operation)
    {
    case QM::Modifier::OPERATION_ASSIGN:
        value = m.value;
        break;
    case QM::Modifier::OPERATION_CHANGE:
        value = m_oldParameters[m.param] + m.value;
        break;
    case QM::Modifier::OPERATION_PERCENT:
        value = m_oldParameters[m.param] + (m.value * m_oldParameters[m.param]) / 100.0f;
        break;
    case QM::Modifier::OPERATION_EXPRESSION:
        if (!m.expression.empty())
            value = QM::eval(m.expression, m_oldParameters);
        else
            value = m_oldParameters[m.param];
        break;
    }

    m_parameters[m.param] = std::min(value, (float)m_quest.parameters.at(m.param).max);
    m_parameters[m.param] = std::max(m_parameters[m.param], (float)m_quest.parameters.at(m.param).min);
}

void QuestPlayer::transit(uint32_t num)
{
    if (m_transition)
    {
        finishTransition();
    }
    else
    {
        if (m_currentLocation.transitions.size() <= num)
            return;

        m_currentTransition = m_currentLocation.transitions[num];

        m_oldParameters = m_parameters;

        for (const QM::Modifier & m : m_currentTransition.modifiers)
        {
            applyModifier(m);
        }

        Log::debug() << "[Quest] P" << m_currentTransition.id;
        auto img = m_transitionImages.find(m_currentTransition.id);
        if (img != m_transitionImages.end())
            m_currentImage = (*img).second;

        m_currentText = substituteValues(m_currentTransition.description);

        if (m_quest.locations.at(m_currentTransition.to).empty)
        {
            finishTransition();
        }
        else if (m_currentTransition.description.empty())
        {
            finishTransition();
        }
        else
        {
            m_transition = true;
        }
    }
}

void QuestPlayer::finishTransition()
{
    if (m_transitionCounts.find(m_currentTransition.id) == m_transitionCounts.end())
    {
        m_transitionCounts[m_currentTransition.id] = 1;
    }
    else
    {
        m_transitionCounts[m_currentTransition.id]++;
    }
    setLocation(m_currentTransition.to);
}

//FIXME: Optimize
void QuestPlayer::reduceTransitions()
{
    std::map<std::string, std::list<uint32_t> > transitions;
    std::map<std::string, uint32_t> alwaysVisibleTransitions;

    for (uint32_t t : m_possibleTransitions)
    {
        const QM::Transition& tr = m_currentLocation.transitions.at(t);
        transitions[tr.title].push_back(t);
    }

    for (uint32_t t : m_alwaysVisibleTransitions)
    {
        const QM::Transition& tr = m_currentLocation.transitions.at(t);
        if (transitions.find(tr.title) == transitions.end())
            alwaysVisibleTransitions[tr.title] = t;
    }

    m_possibleTransitions.clear();
    m_alwaysVisibleTransitions.clear();

    for (std::pair<std::string, uint32_t> p : alwaysVisibleTransitions)
    {
        m_alwaysVisibleTransitions.push_back(p.second);
    }

    for (std::pair<std::string, std::list<uint32_t> > p : transitions)
    {
        if (p.second.size() == 1)
        {
            double pr = m_currentLocation.transitions.at(p.second.front()).priority;
            if (pr < 1.0)
            {
                if ((rand() % 1000) < pr * 1000)
                    m_possibleTransitions.push_back(p.second.front());
            }
            else
                m_possibleTransitions.push_back(p.second.front());
            continue;
        }
        double range = 0;

        for (uint32_t t : p.second)
        {
            range += m_currentLocation.transitions.at(t).priority;
        }

        uint32_t randRange = range * 1000;
        uint32_t r = rand() % randRange;
        uint32_t counter = 0;
        uint32_t selected = 0;

        for (uint32_t t : p.second)
        {
            counter += m_currentLocation.transitions.at(t).priority * 1000;
            selected = t;
            if (counter >= r)
                break;
        }
        m_possibleTransitions.push_back(selected);
    }
}

bool QuestPlayer::death() const
{
    return m_death;
}

bool QuestPlayer::questCompleted() const
{
    return m_completed;
}

bool QuestPlayer::questFailed() const
{
    return m_failed;
}

uint32_t QuestPlayer::currentLocation() const
{
    return m_currentLocation.id;
}

uint32_t QuestPlayer::currentTransition() const
{
    return m_currentTransition.id;
}

bool QuestPlayer::isTransition() const
{
    return m_transition;
}

void QuestPlayer::setImages(std::map<uint32_t, uint32_t> locationImages, std::map<uint32_t, uint32_t> transitionImages)
{
    m_transitionImages = transitionImages;
    m_locationImages = locationImages;
}

uint32_t QuestPlayer::currentImage() const
{
    return m_currentImage;
}
}
}