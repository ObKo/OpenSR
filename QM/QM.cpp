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

#include "OpenSR/QM.h"
#include <vector>

namespace Rangers
{
namespace QM
{
std::wstring readString(std::istream& s)
{
    uint16_t *str;
    uint32_t len;
    s.read((char*)&len, 4);
    str = new uint16_t[len];
    s.read((char*)str, len * 2);

    std::wstring result(len, 0);
    for (int i = 0; i < len; i++)
    {
        //FIXME: Assume that string is in UCS-2 (wchar_t on windows)
        result[i] = str[i];
    }
    delete str;

    return result;
}

void skipString(std::istream& s)
{
    uint32_t len;
    s.read((char*)&len, 4);
    s.seekg(len * 2, std::ios_base::cur);
}

uint8_t readByte(std::istream& s)
{
    uint8_t r;
    s.read((char*)&r, 1);
    return r;
}

uint32_t readUInt(std::istream& s)
{
    uint32_t r;
    s.read((char*)&r, 4);
    return r;
}

int32_t readInt(std::istream& s)
{
    int32_t r;
    s.read((char*)&r, 4);
    return r;
}

int32_t readDouble(std::istream& s)
{
    double r;
    s.read((char*)&r, 8);
    return r;
}

Parameter readParameter(std::istream& s)
{
    Parameter p;

    uint8_t rangesNum;

    p.min = readInt(s);
    p.max = readInt(s);
    p.mid = readInt(s);
    p.type = (Parameter::Type)readByte(s);
    s.seekg(4, std::ios_base::cur);
    p.showOnZero = readByte(s) == 1;
    p.minCritical = readByte(s) == 1;
    p.active = readByte(s) == 1;
    rangesNum = readByte(s);
    s.seekg(3, std::ios_base::cur);
    p.money = readByte(s) == 1;
    s.seekg(4, std::ios_base::cur);
    p.name = readString(s);

    for (uint8_t i = 0; i < rangesNum; i++)
    {
        Parameter::Range r;
        r.from = readInt(s);
        r.to = readInt(s);
        s.seekg(4, std::ios_base::cur);
        r.text = readString(s);
        p.ranges.push_back(r);
    }
    s.seekg(4, std::ios_base::cur);
    p.critText = readString(s);
    s.seekg(4, std::ios_base::cur);
    p.start = readString(s);

    return p;
}

void skipParameter(std::istream& s)
{
    uint8_t rangesNum;

    s.seekg(20, std::ios_base::cur);
    rangesNum = readByte(s);
    s.seekg(8, std::ios_base::cur);
    skipString(s);

    for (uint8_t i = 0; i < rangesNum; i++)
    {
        s.seekg(12, std::ios_base::cur);
        skipString(s);
    }

    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
}

Transition readTransition(std::istream& s, const Quest &q, uint32_t totalParamCount)
{
    Transition t;

    t.priority = readDouble(s);
    t.day = readUInt(s) == 1;
    t.id = readUInt(s);
    t.from = readUInt(s);
    t.to = readUInt(s);
    //Color?
    readByte(s);
    t.alwaysVisible = readByte(s) == 1;
    t.passCount = readUInt(s);
    t.position = readUInt(s);

    for (uint32_t i = 1; i < totalParamCount + 1; i++)
    {
        Modifier m;
        Transition::Condition c;
        m.param = i;
        c.param = i;

        s.seekg(4, std::ios_base::cur);
        c.rangeFrom = readInt(s);
        c.rangeTo = readInt(s);

        bool inRange = false;

        if (q.parameters.find(i) != q.parameters.end())
        {
            if ((c.rangeFrom > q.parameters.at(i).min) || (c.rangeTo < q.parameters.at(i).max))
                inRange = true;
        }

        m.value = readInt(s);

        uint8_t show = readUInt(s);
        if (show == 1)
            m.visibility = Modifier::VISIBILITY_SHOW;
        else if (show == 2)
            m.visibility = Modifier::VISIBILITY_HIDE;
        else
            m.visibility = Modifier::VISIBILITY_NO_CHANGE;

        readByte(s);

        bool percent, assign, expression;
        percent = readByte(s) == 1;
        assign = readByte(s) == 1;
        expression = readByte(s) == 1;

        if (percent)
            m.operation = Modifier::OPERATION_PERCENT;
        else if (assign)
            m.operation = Modifier::OPERATION_ASSIGN;
        else if (expression)
            m.operation = Modifier::OPERATION_EXPRESSION;
        else
            m.operation = Modifier::OPERATION_CHANGE;

        s.seekg(4, std::ios_base::cur);
        m.expression = readString(s);

        uint32_t count = readUInt(s);
        c.includeValues = readByte(s) == 1;
        for (uint32_t j = 0; j < count; j++)
            c.values.push_back(readInt(s));

        count = readUInt(s);
        c.includeMultiples = readByte(s) == 1;
        for (uint32_t j = 0; j < count; j++)
            c.multiples.push_back(readUInt(s));

        s.seekg(4, std::ios_base::cur);
        skipString(s);

        if ((q.parameters.find(i) != q.parameters.end()) && ((show != 0) || (m.value != 0) || (percent) || (assign) ||
                (expression && (m.expression != L"")) || c.values.size() || c.multiples.size() || inRange))
        {
            t.modifiers.push_back(m);
            t.conditions.push_back(c);
        }
    }
    s.seekg(4, std::ios_base::cur);
    t.globalCondition = readString(s);
    s.seekg(4, std::ios_base::cur);
    t.title = readString(s);
    s.seekg(4, std::ios_base::cur);
    t.description = readString(s);

    return t;
}

Location readLocation(std::istream& s, const Quest &q, uint32_t totalParamCount)
{
    Location l;

    l.day = readUInt(s) == 1;
    l.x = readInt(s);
    l.y = readInt(s);
    l.id = readUInt(s);

    bool start, success, fail;
    start = readByte(s) == 1;
    success = readByte(s) == 1;
    fail = readByte(s) == 1;

    if ((start && success) || (start && fail) || (success && fail))
        l.type = Location::LOCATION_NORMAL;
    else if (!start && !success && !fail)
        l.type = Location::LOCATION_NORMAL;
    else if (start)
        l.type = Location::LOCATION_START;
    else if (success)
        l.type = Location::LOCATION_SUCCESS;
    else if (fail)
        l.type = Location::LOCATION_FAIL;

    l.death = readByte(s) == 1;

    if (l.death)
        l.type = Location::LOCATION_FAIL;

    l.empty = readByte(s) == 1;

    for (uint32_t i = 1; i < totalParamCount + 1; i++)
    {
        Modifier m;

        m.param = i;

        s.seekg(12, std::ios_base::cur);
        m.value = readInt(s);

        uint8_t show = readByte(s);
        if (show == 1)
            m.visibility = Modifier::VISIBILITY_SHOW;
        else if (show == 2)
            m.visibility = Modifier::VISIBILITY_HIDE;
        else
            m.visibility = Modifier::VISIBILITY_NO_CHANGE;

        s.seekg(4, std::ios_base::cur);
        bool percent, assign, expression;
        percent = readByte(s) == 1;
        assign = readByte(s) == 1;
        expression = readByte(s) == 1;

        if (percent)
            m.operation = Modifier::OPERATION_PERCENT;
        else if (assign)
            m.operation = Modifier::OPERATION_ASSIGN;
        else if (expression)
            m.operation = Modifier::OPERATION_EXPRESSION;
        else
            m.operation = Modifier::OPERATION_CHANGE;

        s.seekg(4, std::ios_base::cur);
        m.expression = readString(s);

        s.seekg(14, std::ios_base::cur);
        skipString(s);

        if ((q.parameters.find(i) != q.parameters.end()) && ((show != 0) || (m.value != 0) || (percent) || (assign) ||
                (expression && (m.expression != L""))))
            l.modifiers.push_back(m);
    }

    for (int i = 0; i < 10; i++)
    {
        s.seekg(4, std::ios_base::cur);
        std::wstring str = readString(s);
        l.descriptions.push_back(str);
    }
    l.descriptionExpression = readByte(s) == 1;

    s.seekg(8, std::ios_base::cur);
    skipString(s);

    s.seekg(4, std::ios_base::cur);
    skipString(s);

    s.seekg(4, std::ios_base::cur);
    l.expression = readString(s);

    return l;
}

Quest readQuest(std::istream& s)
{
    uint32_t sig;
    Quest q;
    uint32_t temp;

    uint8_t paramsCount;
    uint32_t pathCount;

    s.read((char*)&sig, 4);
    if (sig == 0x423a35d4)
        paramsCount = 96;
    else if (sig == 0x423a35d3)
        paramsCount = 48;
    else if (sig == 0x423a35d2)
        paramsCount = 24;
    else
        return Quest();

    s.seekg(4, std::ios_base::cur);

    q.info.races = readByte(s);
    q.info.doneImmediately = readByte(s) == 1;
    q.info.planetRaces = readByte(s);

    s.seekg(4, std::ios_base::cur);
    q.info.playerTypes = readByte(s);

    s.seekg(4, std::ios_base::cur);
    q.info.playerRaces = readByte(s);

    s.seekg(4, std::ios_base::cur);
    q.info.relation = readInt(s);

    s.seekg(20, std::ios_base::cur);
    pathCount = readUInt(s);
    q.info.difficulty = readUInt(s);

    for (uint8_t i = 1; i < paramsCount + 1; i++)
    {
        Parameter p = readParameter(s);
        p.id = i;

        if (p.active)
            q.parameters[i] = p;
    }

    s.seekg(4, std::ios_base::cur);
    q.toStar = readString(s);

    //FIXME: Other substitutions?
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);

    s.seekg(4, std::ios_base::cur);
    q.toPlanet = readString(s);
    s.seekg(4, std::ios_base::cur);
    q.date = readString(s);
    s.seekg(4, std::ios_base::cur);
    q.money = readString(s);
    s.seekg(4, std::ios_base::cur);
    q.fromPlanet = readString(s);
    s.seekg(4, std::ios_base::cur);
    q.fromStar = readString(s);
    s.seekg(4, std::ios_base::cur);
    q.ranger = readString(s);

    uint32_t locationCount = readUInt(s);
    pathCount = readUInt(s);

    s.seekg(4, std::ios_base::cur);
    q.info.winnerText = readString(s);
    s.seekg(4, std::ios_base::cur);
    q.info.descriptionText = readString(s);
    s.seekg(8, std::ios_base::cur);

    for (uint32_t i = 0; i < locationCount; i++)
    {
        Location l = readLocation(s, q, paramsCount);
        q.locations[l.id] = l;
        if (l.type == Location::LOCATION_START)
            q.startLocation = l.id;
    }

    for (uint32_t i = 0; i < pathCount; i++)
    {
        Transition t = readTransition(s, q, paramsCount);
        q.locations[t.from].transitions.push_back(t);
    }

    return q;
}

QuestInfo readQuestInfo(std::istream& s)
{
    uint32_t sig;
    QuestInfo info;
    uint32_t temp;

    uint8_t paramsCount;
    uint32_t pathCount;

    s.read((char*)&sig, 4);
    if (sig == 0x423a35d4)
        paramsCount = 96;
    else if (sig == 0x423a35d3)
        paramsCount = 48;
    else if (sig == 0x423a35d2)
        paramsCount = 24;
    else
        return QuestInfo();

    s.seekg(4, std::ios_base::cur);

    info.races = readByte(s);
    info.doneImmediately = readByte(s) == 1;
    info.planetRaces = readByte(s);

    s.seekg(4, std::ios_base::cur);
    info.playerTypes = readByte(s);

    s.seekg(4, std::ios_base::cur);
    info.playerRaces = readByte(s);

    s.seekg(4, std::ios_base::cur);
    info.relation = readInt(s);

    s.seekg(20, std::ios_base::cur);
    pathCount = readUInt(s);
    info.difficulty = readUInt(s);

    for (uint8_t i = 0; i < paramsCount; i++)
        skipParameter(s);

    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);
    s.seekg(4, std::ios_base::cur);
    skipString(s);

    s.seekg(12, std::ios_base::cur);
    info.winnerText = readString(s);
    s.seekg(4, std::ios_base::cur);
    info.descriptionText = readString(s);

    return info;
}
}
}
