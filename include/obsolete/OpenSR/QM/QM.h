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

#ifndef RANGERS_QM_H
#define RANGERS_QM_H

#include <stdint.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#ifdef QM_LIBRARY
#ifdef WIN32
#define QM_API __declspec(dllexport)
#else
#define QM_API
#endif
#else
#ifdef WIN32
#define QM_API
#else
#define QM_API
#endif
#endif

namespace Rangers
{
namespace QM
{
struct QM_API Parameter
{
    enum Type {PARAMETER_NORMAL = 0, PARAMETER_FAIL = 1, PARAMETER_SUCCESS = 2, PARAMETER_DEATH = 3};
    struct Range
    {
        int32_t from;
        int32_t to;
        std::string text;
    };

    uint32_t id;

    int32_t min;
    int32_t max;
    int32_t mid;
    Type type;

    bool showOnZero;
    bool minCritical;
    bool active;
    bool money;

    std::string name;
    std::string critText;

    std::vector<Range> ranges;

    std::string start;
};

struct QM_API Modifier
{
    enum Operation {OPERATION_ASSIGN, OPERATION_CHANGE, OPERATION_PERCENT, OPERATION_EXPRESSION};
    enum Visibility {VISIBILITY_NO_CHANGE, VISIBILITY_HIDE, VISIBILITY_SHOW};

    Operation operation;
    Visibility visibility;

    uint32_t param;
    int32_t value;
    std::string expression;
};

struct QM_API Transition
{
    struct Condition
    {
        uint32_t param;

        int32_t rangeFrom;
        int32_t rangeTo;

        bool includeValues;
        bool includeMultiples;
        std::vector<int32_t> values;
        std::vector<uint32_t> multiples;
    };

    uint32_t id;

    uint32_t from;
    uint32_t to;

    double priority;
    bool day;

    //Color in TGE?
    bool alwaysVisible;
    uint32_t passCount;
    uint32_t position;

    std::vector<Modifier> modifiers;
    std::vector<Condition> conditions;

    std::string globalCondition;
    std::string title;
    std::string description;
};

struct QM_API Location
{
    enum Type {LOCATION_NORMAL, LOCATION_START, LOCATION_FAIL, LOCATION_SUCCESS};

    Type type;

    uint32_t id;

    int32_t x;
    int32_t y;

    bool day;
    bool death;
    bool empty;

    //! Description selected by expression
    bool descriptionExpression;
    std::string expression;

    std::vector<Modifier> modifiers;
    std::vector<std::string> descriptions;

    std::vector<Transition> transitions;
};

struct QM_API QuestInfo
{
    uint8_t races;
    bool doneImmediately;
    uint8_t planetRaces;
    uint8_t playerTypes;
    uint8_t playerRaces;
    int32_t relation;
    uint32_t difficulty;

    std::string winnerText;
    std::string descriptionText;
};

struct QM_API Quest
{
    enum Race {RACE_MALOQ = 0x01, RACE_PELENG = 0x02, RACE_HUMAN = 0x04, RACE_FAEYAN = 0x08, RACE_GAAL = 0x10, RACE_NONE = 0x40, RACE_ANY = 0xFF};
    enum PlayerType {PLAYER_TRADER = 0x01, PLAYER_PIRATE = 0x02, PLAYER_WARRIOR = 0x04, PLAYER_ANY = 0xFF};

    std::map<uint32_t, Parameter> parameters;
    std::map<uint32_t, Location> locations;

    uint32_t startLocation;

    QuestInfo info;

    std::string toStar;
    std::string toPlanet;
    std::string date;
    std::string money;
    std::string fromPlanet;
    std::string fromStar;
    std::string ranger;
};

//! Load whole quest.
QM_API Quest readQuest(std::istream& s);

//! Load only information about quest
QM_API QuestInfo readQuestInfo(std::istream& s);
}
}

#endif
