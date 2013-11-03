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

#include "PlayerWindow.h"
#include "QuestPlayer.h"
#include "ui_PlayerWindow.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtCore/QDebug>

namespace Rangers
{
namespace QuestPlayer
{

PlayerWindow::PlayerWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::PlayerWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(loadQuest()));
    connect(&m_player, SIGNAL(locationChanged()), this, SLOT(updateQuest()));
}

PlayerWindow::~PlayerWindow()
{
    delete m_ui;
}

void PlayerWindow::loadQuest()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open quest"));
    if (!file.isEmpty())
        m_player.loadQuest(file);
}

void PlayerWindow::updateQuest()
{
    QStringList ps = m_player.visibleParameters();
    m_ui->variableLabel->setText(ps.join("<br>"));
    m_ui->textLabel->setText("<p>" + m_player.locationText().replace("\r\n", "</p><p>") + "</p>");
}

}
}

