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

#ifndef RANGERS_QUESTPLAYER_H
#define RANGERS_QUESTPLAYER_H

#include <QtCore/QObject>

#include <OpenSR/QM.h>

class QString;
namespace Rangers
{
namespace QuestPlayer
{

class QuestPlayer: public QObject
{
    Q_OBJECT
public:
    QuestPlayer(QObject *parent = 0);
    ~QuestPlayer();

    Rangers::QM::Location currentLocation() const;

    QString locationText() const;
    QStringList visibleParameters() const;
    std::map<uint32_t, QString> visibleTransitions();

    void startTransition(uint32_t num);
    void finishTransition();

public Q_SLOTS:
    void loadQuest(const QString& file);

Q_SIGNALS:
    void locationChanged();
    void transitionText(const QString &text);

private:
    void applyModifier(const QM::Modifier &m);
    bool checkCondition(const QM::Transition::Condition &c) const;
    void setLocation(uint32_t location);
    void checkTransitions();
    void reduceTransitions();

    QString substituteValues(const QString &str) const;

    Rangers::QM::Quest m_quest;

    QM::Location m_currentLocation;
    QString m_locationText;
    QM::Transition m_currentTransition;

    std::map<uint32_t, int32_t> m_parametersVisibility;
    std::map<uint32_t, int32_t> m_parameters;
    std::map<uint32_t, int32_t> m_oldParameters;
    std::map<uint32_t, int32_t> m_transitionCounts;
    std::map<uint32_t, int32_t> m_locationDescriptionsCount;
    std::list<uint32_t> m_possibleTransitions;
};

}
}

#endif // RANGERS_QUESTPLAYER_H
