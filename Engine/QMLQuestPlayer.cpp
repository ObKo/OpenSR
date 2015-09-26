/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/QMLQuestPlayer.h"

#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <QIODevice>

namespace OpenSR
{
QMLQuestPlayer::QMLQuestPlayer(QObject *parent): QObject(parent)
{
    connect(&m_player, SIGNAL(locationChanged()), this, SLOT(updateLocation()));
    connect(&m_player, SIGNAL(transitionText(QString)), this, SLOT(showTransitionText(QString)));
    connect(&m_player, SIGNAL(questCompleted(QString)), this, SLOT(showQuestCompleted(QString)));
    connect(&m_player, SIGNAL(questFailed(QString, bool)), this, SLOT(showQuestFailed(QString, bool)));
}

QString QMLQuestPlayer::questID() const
{
    return m_questID;
}

QString QMLQuestPlayer::currentText() const
{
    return m_currentText;
}

QStringList QMLQuestPlayer::parameters() const
{
    return m_player.visibleParameters();
}

QVariantList QMLQuestPlayer::transitions() const
{
    QVariantList result;
    QList<QM::QuestPlayer::TransitionItem> trans = m_player.visibleTransitions();
    for (const QM::QuestPlayer::TransitionItem& t : trans)
    {
        QVariantMap tres;
        tres["id"] = t.id;
        tres["title"] = t.title;
        tres["enabled"] = t.enabled;
        result.append(tres);
    }
    return result;
}

QUrl QMLQuestPlayer::image() const
{
    return m_image;
}

void QMLQuestPlayer::setQuestID(const QString& id)
{
    m_questID = id;
    emit(questIDChanged());
    resetQuest();
}

void QMLQuestPlayer::resetQuest()
{
    m_player.resetQuest();
    m_locationsImages.clear();
    m_transitionsImages.clear();

    Engine *engine = ((Engine*)qApp);

    //m_currentText = QString();
    //emit(currentTextChanged());
    QString path = engine->datValue(QString("PlanetQuest.PlanetQuest.%1").arg(m_questID)).toString().replace("\\", "/");
    QIODevice *dev = engine->resources()->getIODevice(path);

    if (!dev)
        return;

    if (!dev->isOpen())
    {
        delete dev;
        return;
    }

    QVariantMap images = engine->datValue("Data.PQI").toMap();
    auto end = images.end();
    for (auto i = images.begin(); i != end; ++i)
    {
        QStringList img = i.key().split(',');
        QString id = img.takeAt(0);
        if (id != m_questID)
            continue;
        QString type = img.takeAt(0);
        QString path = i.value().toString();
        if (type == "L")
        {
            for (const QString& lid : img)
                m_locationsImages[lid.toUInt()] = path;
        }
        else if (type == "P")
        {
            for (const QString& pid : img)
                m_transitionsImages[pid.toUInt()] = path;
        }
    }

    m_player.loadQuest(dev);
}

void QMLQuestPlayer::startTransition(uint32_t id)
{
    m_player.startTransition(id);
}

void QMLQuestPlayer::finishTransition()
{
    m_player.finishTransition();
}

void QMLQuestPlayer::updateLocation()
{
    m_currentText = m_player.locationText();
    emit(currentTextChanged());
    emit(parametersChanged());
    emit(transitionsChanged());

    auto it = m_locationsImages.find(m_player.currentLocation().id);
    if (it != m_locationsImages.end())
    {
        QString path = it.value();
        path = "res:/DATA/" + path.replace("Bm.", "").replace(".", "/") + ".jpg";
        m_image = QUrl(path);
        emit(imageChanged());
    }

    emit(questLocation());
}

void QMLQuestPlayer::showTransitionText(const QString &text)
{
    m_currentText = text;
    emit(currentTextChanged());
    emit(parametersChanged());

    auto it = m_transitionsImages.find(m_player.currentTransition().id);
    if (it != m_transitionsImages.end())
    {
        QString path = it.value();
        path = "res:/DATA/" + path.replace("Bm.", "").replace(".", "/") + ".jpg";
        m_image = QUrl(path);
        emit(imageChanged());
    }

    emit(questTransition());
}


void QMLQuestPlayer::showQuestCompleted(const QString &text)
{
    m_currentText = text;
    emit(currentTextChanged());
    emit(parametersChanged());
    emit(questCompleted());
}

void QMLQuestPlayer::showQuestFailed(const QString &text, bool death)
{
    m_currentText = text;
    emit(currentTextChanged());
    emit(parametersChanged());
    emit(questFailed(death));
}
}

