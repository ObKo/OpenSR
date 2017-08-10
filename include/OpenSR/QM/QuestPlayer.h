/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2017 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_QUESTPLAYER_H
#define OPENSR_QUESTPLAYER_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/QM/QM.h>
#include <QObject>
#include <QList>
#include <QPair>

class QIODevice;
namespace OpenSR
{
namespace QM
{
class QM_API QuestPlayer: public QObject
{
    Q_OBJECT
    OPENSR_DECLARE_PRIVATE(QuestPlayer)
public:
    struct TransitionItem
    {
        uint32_t id;
        QString title;
        bool enabled;
    };

    QuestPlayer(QObject *parent = 0);
    ~QuestPlayer();

    QM::Location currentLocation() const;
    QM::Transition currentTransition() const;

    QString locationText() const;
    QStringList visibleParameters() const;
    QList<TransitionItem> visibleTransitions() const;

public Q_SLOTS:
    void loadQuest(QIODevice *dev);
    void resetQuest();
    void startTransition(uint32_t num);
    void finishTransition();

Q_SIGNALS:
    void locationChanged();
    void transitionText(const QString &text);
    void questCompleted(const QString &text);
    void questFailed(const QString &text, bool death);

protected:
    OPENSR_DECLARE_DPOINTER(QuestPlayer)
};

}
}

#endif // OPENSR_QUESTPLAYER_H
