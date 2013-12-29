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

#ifndef WORLD_QUESTPLAYER_H
#define WORLD_QUESTPLAYER_H

#include <OpenSR/QM/QM.h>

#include <list>
#include <map>
#include <tuple>

namespace Rangers
{
namespace World
{

class QuestPlayer
{
public:
    QuestPlayer();
    ~QuestPlayer();

    std::wstring currentText() const;
    std::vector<std::wstring> visibleParameters() const;
    std::vector<std::tuple<uint32_t, std::wstring, bool> > visibleTransitions() const;

    void transit(uint32_t num);

    void loadQuest(const std::wstring& quest);
    void resetQuest();

    bool questCompleted() const;
    bool questFailed() const;
    bool death() const;

private:
    void applyModifier(const QM::Modifier &m);
    bool checkCondition(const QM::Transition::Condition &c) const;
    void setLocation(uint32_t location);
    void checkTransitions();
    void reduceTransitions();
    bool checkCriticalParameters();
    void finishTransition();

    std::wstring substituteValues(const std::wstring &str) const;

    Rangers::QM::Quest m_quest;

    QM::Location m_currentLocation;
    std::wstring m_currentText;
    QM::Transition m_currentTransition;

    bool m_transition;
    bool m_completed;
    bool m_failed;
    bool m_death;

    std::map<uint32_t, int32_t> m_parametersVisibility;
    std::map<uint32_t, float> m_parameters;
    std::map<uint32_t, float> m_oldParameters;
    std::map<uint32_t, int32_t> m_transitionCounts;
    std::map<uint32_t, int32_t> m_locationDescriptionsCount;
    std::list<uint32_t> m_possibleTransitions;
    std::list<uint32_t> m_alwaysVisibleTransitions;
};

}
}

#endif // RANGERS_QUESTPLAYER_H
