/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2015 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/QM/QM.h"
#include <QIODevice>
#include <QString>

namespace OpenSR
{
namespace QM
{
QString readString(QIODevice *dev)
{
    uint32_t len;
    dev->read((char*)&len, 4);
    return QString::fromUtf16((ushort*)dev->read(len * 2).constData(), len);
}

void skipString(QIODevice *dev)
{
    uint32_t len;
    dev->read((char*)&len, 4);
    //FIXME: Dummy read?
    dev->seek(dev->pos() + len * 2);
}

uint8_t readByte(QIODevice *dev)
{
    uint8_t r;
    dev->read((char*)&r, 1);
    return r;
}

uint32_t readUInt(QIODevice *dev)
{
    uint32_t r;
    dev->read((char*)&r, 4);
    return r;
}

int32_t readInt(QIODevice *dev)
{
    int32_t r;
    dev->read((char*)&r, 4);
    return r;
}

double readDouble(QIODevice *dev)
{
    double r;
    dev->read((char*)&r, 8);
    return r;
}

Parameter readParameter(QIODevice *dev)
{
    Parameter p;

    uint8_t rangesNum;

    p.min = readInt(dev);
    p.max = readInt(dev);
    p.mid = readInt(dev);
    p.type = (Parameter::Type)readByte(dev);
    dev->seek(dev->pos() + 4);
    p.showOnZero = readByte(dev) == 1;
    p.minCritical = readByte(dev) == 1;
    p.active = readByte(dev) == 1;
    rangesNum = readByte(dev);
    dev->seek(dev->pos() + 3);
    p.money = readByte(dev) == 1;
    dev->seek(dev->pos() + 4);
    p.name = readString(dev);

    for (uint8_t i = 0; i < rangesNum; i++)
    {
        Parameter::Range r;
        r.from = readInt(dev);
        r.to = readInt(dev);
        dev->seek(dev->pos() + 4);
        r.text = readString(dev);
        p.ranges.append(r);
    }
    dev->seek(dev->pos() + 4);
    p.critText = readString(dev);
    dev->seek(dev->pos() + 4);
    p.start = readString(dev);

    return p;
}

void skipParameter(QIODevice *dev)
{
    uint8_t rangesNum;

    dev->seek(dev->pos() + 20);
    rangesNum = readByte(dev);
    dev->seek(dev->pos() + 8);
    skipString(dev);

    for (uint8_t i = 0; i < rangesNum; i++)
    {
        dev->seek(dev->pos() + 12);
        skipString(dev);
    }

    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
}

Transition readTransition(QIODevice *dev, const Quest &q, uint32_t totalParamCount)
{
    Transition t;

    t.priority = readDouble(dev);
    t.day = readUInt(dev) == 1;
    t.id = readUInt(dev);
    t.from = readUInt(dev);
    t.to = readUInt(dev);
    //Color?
    readByte(dev);
    t.alwaysVisible = readByte(dev) == 1;
    t.passCount = readUInt(dev);
    t.position = readUInt(dev);

    for (uint32_t i = 1; i < totalParamCount + 1; i++)
    {
        Modifier m;
        Transition::Condition c;
        m.param = i;
        c.param = i;

        dev->seek(dev->pos() + 4);
        c.rangeFrom = readInt(dev);
        c.rangeTo = readInt(dev);

        bool inRange = false;

        auto it = q.parameters.find(i);
        if (it != q.parameters.end())
        {
            if ((c.rangeFrom > it->min) || (c.rangeTo < it->max))
                inRange = true;
        }

        m.value = readInt(dev);

        uint8_t show = readUInt(dev);
        if (show == 1)
            m.visibility = Modifier::VISIBILITY_SHOW;
        else if (show == 2)
            m.visibility = Modifier::VISIBILITY_HIDE;
        else
            m.visibility = Modifier::VISIBILITY_NO_CHANGE;

        readByte(dev);

        bool percent, assign, expression;
        percent = readByte(dev) == 1;
        assign = readByte(dev) == 1;
        expression = readByte(dev) == 1;

        if (percent)
            m.operation = Modifier::OPERATION_PERCENT;
        else if (assign)
            m.operation = Modifier::OPERATION_ASSIGN;
        else if (expression)
            m.operation = Modifier::OPERATION_EXPRESSION;
        else
            m.operation = Modifier::OPERATION_CHANGE;

        dev->seek(dev->pos() + 4);
        m.expression = readString(dev);

        uint32_t count = readUInt(dev);
        c.includeValues = readByte(dev) == 1;
        for (uint32_t j = 0; j < count; j++)
            c.values.append(readInt(dev));

        count = readUInt(dev);
        c.includeMultiples = readByte(dev) == 1;
        for (uint32_t j = 0; j < count; j++)
            c.multiples.append(readUInt(dev));

        dev->seek(dev->pos() + 4);
        skipString(dev);

        if ((it != q.parameters.end()) && ((show != 0) || (m.value != 0) || (percent) || (assign) ||
                                           (expression && (m.expression != "")) || c.values.size() || c.multiples.size() || inRange))
        {
            t.modifiers.append(m);
            t.conditions.append(c);
        }
    }
    dev->seek(dev->pos() + 4);
    t.globalCondition = readString(dev);
    dev->seek(dev->pos() + 4);
    t.title = readString(dev);
    dev->seek(dev->pos() + 4);
    t.description = readString(dev);

    return t;
}

Location readLocation(QIODevice *dev, const Quest &q, uint32_t totalParamCount)
{
    Location l;

    l.day = readUInt(dev) == 1;
    l.x = readInt(dev);
    l.y = readInt(dev);
    l.id = readUInt(dev);

    bool start, success, fail;
    start = readByte(dev) == 1;
    success = readByte(dev) == 1;
    fail = readByte(dev) == 1;

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

    l.death = readByte(dev) == 1;

    if (l.death)
        l.type = Location::LOCATION_FAIL;

    l.empty = readByte(dev) == 1;

    for (uint32_t i = 1; i < totalParamCount + 1; i++)
    {
        Modifier m;

        m.param = i;

        dev->seek(dev->pos() + 12);
        m.value = readInt(dev);

        uint8_t show = readByte(dev);
        if (show == 1)
            m.visibility = Modifier::VISIBILITY_SHOW;
        else if (show == 2)
            m.visibility = Modifier::VISIBILITY_HIDE;
        else
            m.visibility = Modifier::VISIBILITY_NO_CHANGE;

        dev->seek(dev->pos() + 4);
        bool percent, assign, expression;
        percent = readByte(dev) == 1;
        assign = readByte(dev) == 1;
        expression = readByte(dev) == 1;

        if (percent)
            m.operation = Modifier::OPERATION_PERCENT;
        else if (assign)
            m.operation = Modifier::OPERATION_ASSIGN;
        else if (expression)
            m.operation = Modifier::OPERATION_EXPRESSION;
        else
            m.operation = Modifier::OPERATION_CHANGE;

        dev->seek(dev->pos() + 4);
        m.expression = readString(dev);

        dev->seek(dev->pos() + 14);
        skipString(dev);

        auto it = q.parameters.find(i);
        if ((it != q.parameters.end()) && ((show != 0) || (m.value != 0) || (percent) || (assign) ||
                                           (expression && (m.expression != ""))))
            l.modifiers.append(m);
    }

    for (int i = 0; i < 10; i++)
    {
        dev->seek(dev->pos() + 4);
        QString str = readString(dev);
        l.descriptions.append(str);
    }
    l.descriptionExpression = readByte(dev) == 1;

    dev->seek(dev->pos() + 8);
    skipString(dev);

    dev->seek(dev->pos() + 4);
    skipString(dev);

    dev->seek(dev->pos() + 4);
    l.expression = readString(dev);

    return l;
}

Quest readQuest(QIODevice *dev)
{
    uint32_t sig;
    Quest q;
    uint32_t temp;

    uint8_t paramsCount;
    uint32_t pathCount;

    dev->read((char*)&sig, 4);
    if (sig == 0x423a35d4)
        paramsCount = 96;
    else if (sig == 0x423a35d3)
        paramsCount = 48;
    else if (sig == 0x423a35d2)
        paramsCount = 24;
    else
        return Quest();

    dev->seek(dev->pos() + 4);

    q.info.races = readByte(dev);
    q.info.doneImmediately = readByte(dev) == 1;
    q.info.planetRaces = readByte(dev);

    dev->seek(dev->pos() + 4);
    q.info.playerTypes = readByte(dev);

    dev->seek(dev->pos() + 4);
    q.info.playerRaces = readByte(dev);

    dev->seek(dev->pos() + 4);
    q.info.relation = readInt(dev);

    dev->seek(dev->pos() + 20);
    pathCount = readUInt(dev);
    q.info.difficulty = readUInt(dev);

    for (uint8_t i = 1; i < paramsCount + 1; i++)
    {
        Parameter p = readParameter(dev);
        p.id = i;

        if (p.active)
            q.parameters[i] = p;
    }

    dev->seek(dev->pos() + 4);
    q.toStar = readString(dev);

    //FIXME: Other substitutions?
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);

    dev->seek(dev->pos() + 4);
    q.toPlanet = readString(dev);
    dev->seek(dev->pos() + 4);
    q.date = readString(dev);
    dev->seek(dev->pos() + 4);
    q.money = readString(dev);
    dev->seek(dev->pos() + 4);
    q.fromPlanet = readString(dev);
    dev->seek(dev->pos() + 4);
    q.fromStar = readString(dev);
    dev->seek(dev->pos() + 4);
    q.ranger = readString(dev);

    uint32_t locationCount = readUInt(dev);
    pathCount = readUInt(dev);

    dev->seek(dev->pos() + 4);
    q.info.winnerText = readString(dev);
    dev->seek(dev->pos() + 4);
    q.info.descriptionText = readString(dev);
    dev->seek(dev->pos() + 8);

    for (uint32_t i = 0; i < locationCount; i++)
    {
        Location l = readLocation(dev, q, paramsCount);
        q.locations[l.id] = l;
        if (l.type == Location::LOCATION_START)
            q.startLocation = l.id;
    }

    for (uint32_t i = 0; i < pathCount; i++)
    {
        Transition t = readTransition(dev, q, paramsCount);
        q.locations[t.from].transitions.append(t);
    }

    return q;
}

QuestInfo readQuestInfo(QIODevice *dev)
{
    uint32_t sig;
    QuestInfo info;
    uint32_t temp;

    uint8_t paramsCount;
    uint32_t pathCount;

    dev->read((char*)&sig, 4);
    if (sig == 0x423a35d4)
        paramsCount = 96;
    else if (sig == 0x423a35d3)
        paramsCount = 48;
    else if (sig == 0x423a35d2)
        paramsCount = 24;
    else
        return QuestInfo();

    dev->seek(dev->pos() + 4);

    info.races = readByte(dev);
    info.doneImmediately = readByte(dev) == 1;
    info.planetRaces = readByte(dev);

    dev->seek(dev->pos() + 4);
    info.playerTypes = readByte(dev);

    dev->seek(dev->pos() + 4);
    info.playerRaces = readByte(dev);

    dev->seek(dev->pos() + 4);
    info.relation = readInt(dev);

    dev->seek(dev->pos() + 20);
    pathCount = readUInt(dev);
    info.difficulty = readUInt(dev);

    for (uint8_t i = 0; i < paramsCount; i++)
        skipParameter(dev);

    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);
    dev->seek(dev->pos() + 4);
    skipString(dev);

    dev->seek(dev->pos() + 12);
    info.winnerText = readString(dev);
    dev->seek(dev->pos() + 4);
    info.descriptionText = readString(dev);

    return info;
}
}
}
