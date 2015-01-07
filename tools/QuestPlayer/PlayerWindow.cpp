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

#include "PlayerWindow.h"
#include <OpenSR/QM/QuestPlayer.h>
#include "ui_PlayerWindow.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtGui/QCursor>
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtCore/QtGlobal>

namespace OpenSR
{
namespace QuestPlayer
{

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::PlayerWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(loadQuest()));
    connect(m_ui->actionRestart, SIGNAL(triggered(bool)), &m_player, SLOT(resetQuest()));
    connect(&m_player, SIGNAL(locationChanged()), this, SLOT(updateQuest()));
    connect(&m_player, SIGNAL(transitionText(QString)), this, SLOT(showTransition(QString)));
    connect(&m_player, SIGNAL(questFailed(QString, bool)), this, SLOT(showQuestFailed(QString, bool)));
    connect(&m_player, SIGNAL(questCompleted(QString)), this, SLOT(showQuestCompleted(QString)));
}

PlayerWindow::~PlayerWindow()
{
    delete m_ui;
}

void PlayerWindow::loadQuest()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open quest"));
    m_transition = false;
    if (!file.isEmpty())
    {
        QFile f(file);
        f.open(QIODevice::ReadOnly);
        m_player.loadQuest(&f);
        f.close();
    }
}

void PlayerWindow::updateQuest()
{
    for (QLabel * b : m_transitionButtons.keys())
    {
        m_ui->buttonLayout->removeWidget(b);
        delete b;
    }
    m_transitionButtons.clear();

    QStringList ps = m_player.visibleParameters();
    m_ui->variableLabel->setText(ps.join("<br>"));
    m_ui->textLabel->setText("<p>" + m_player.locationText().replace("\r\n", "</p><p>") + "</p>");

    QList<QM::QuestPlayer::TransitionItem> newButtons = m_player.visibleTransitions();
    for (QM::QuestPlayer::TransitionItem &p : newButtons)
    {
        QLabel *b;
        if (p.title.isEmpty())
            b = new QLabel(tr("Next"), this);
        else
            b = new QLabel(p.title, this);

        b->setWordWrap(true);
        if (p.enabled)
        {
            b->setCursor(QCursor(Qt::PointingHandCursor));
            b->setStyleSheet("QLabel {color: blue} QLabel:hover {color: darkblue}");
            b->installEventFilter(this);
        }
        else
        {
            b->setStyleSheet("QLabel {color: grey} QLabel:hover {color: darkgrey}");
        }

        m_ui->buttonLayout->addWidget(b);
        m_transitionButtons[b] = p.id;
    }
}

void PlayerWindow::showTransition(const QString& text)
{
    m_transition = true;
    for (QLabel * b : m_transitionButtons.keys())
    {
        m_ui->buttonLayout->removeWidget(b);
        delete b;
    }
    m_transitionButtons.clear();
    QString t = text;
    QStringList ps = m_player.visibleParameters();
    m_ui->variableLabel->setText(ps.join("<br>"));
    m_ui->textLabel->setText("<p>" + t.replace("\r\n", "</p><p>") + "</p>");

    QLabel *b = new QLabel(tr("Next"), this);
    b->setWordWrap(true);
    b->setStyleSheet("QLabel {color: blue} QLabel:hover {color: darkblue}");
    b->setCursor(QCursor(Qt::PointingHandCursor));
    b->installEventFilter(this);
    m_ui->buttonLayout->addWidget(b);
    m_transitionButtons[b] = 0;
}

void PlayerWindow::showQuestCompleted(const QString& text)
{
    for (QLabel * b : m_transitionButtons.keys())
    {
        m_ui->buttonLayout->removeWidget(b);
        delete b;
    }
    m_transitionButtons.clear();

    QString t = text;
    m_ui->textLabel->setText("<p>" + t.replace("\r\n", "</p><p>") + "</p><p><font color=\"green\">Quest completed!</font></p>");
}

void PlayerWindow::showQuestFailed(const QString& text, bool death)
{
    for (QLabel * b : m_transitionButtons.keys())
    {
        m_ui->buttonLayout->removeWidget(b);
        delete b;
    }
    m_transitionButtons.clear();

    QString t = text;
    m_ui->textLabel->setText("<p>" + t.replace("\r\n", "</p><p>") +
                             "</p><p><font color=\"red\">" + tr("Quest failed!") + "</font></p>" +
                             (death ? "<p><font color=\"red\">" + tr("You're dead!") + "</font></p>" : ""));

}

bool PlayerWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *m = dynamic_cast<QMouseEvent*>(event);
        if (m->button() == Qt::LeftButton)
        {
            if (m_transition)
            {
                m_player.finishTransition();
                m_transition = false;
            }
            else
            {
                auto t = m_transitionButtons.find(dynamic_cast<QLabel*>(obj));
                if (t != m_transitionButtons.end())
                {
                    m_player.startTransition(*t);
                }
            }
            return true;
        }
        return obj->event(event);
    }
    else
        return obj->event(event);
}

}
}

