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

#ifndef RANGERS_PLAYER_WINDOW_H
#define RANGERS_PLAYER_WINDOW_H

#include <QMainWindow>
#include <QMap>

#include <OpenSR/QM/QuestPlayer.h>

namespace Ui
{
class PlayerWindow;
}
class QLabel;
namespace OpenSR
{
namespace QuestPlayer
{
class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::PlayerWindow *m_ui;
    QM::QuestPlayer m_player;
    bool m_transition;

    QMap<QLabel*, uint32_t> m_transitionButtons;

private Q_SLOTS:
    void loadQuest();
    void updateQuest();
    void showTransition(const QString& text);
    void showQuestCompleted(const QString& text);
    void showQuestFailed(const QString& text, bool death);
};

}
}

#endif // RANGERS_PLAYER_WINDOW_H
