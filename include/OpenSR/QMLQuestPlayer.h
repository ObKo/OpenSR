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

#ifndef OPENSR_QMLQUESTPLAYER_H
#define OPENSR_QMLQUESTPLAYER_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/QM/QuestPlayer.h>

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <QVariantList>

namespace OpenSR
{
class ENGINE_API QMLQuestPlayer: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString questID READ questID WRITE setQuestID NOTIFY questIDChanged)
    Q_PROPERTY(QString currentText READ currentText NOTIFY currentTextChanged)
    Q_PROPERTY(QStringList parameters READ parameters NOTIFY parametersChanged)
    Q_PROPERTY(QVariantList transitions READ transitions NOTIFY transitionsChanged)
    Q_PROPERTY(QUrl image READ image NOTIFY imageChanged)

public:
    QMLQuestPlayer(QObject *parent = 0);

    QString questID() const;
    QString currentText() const;
    QStringList parameters() const;
    QVariantList transitions() const;
    QUrl image() const;

    void setQuestID(const QString& id);

public Q_SLOTS:
    void resetQuest();
    void startTransition(quint32 id);
    void finishTransition();

Q_SIGNALS:
    void questIDChanged();
    void currentTextChanged();
    void parametersChanged();
    void transitionsChanged();
    void imageChanged();

    void questLocation();
    void questTransition();
    void questFailed(bool death);
    void questCompleted();

private Q_SLOTS:
    void updateLocation();
    void showTransitionText(const QString &text);
    void showQuestCompleted(const QString &text);
    void showQuestFailed(const QString &text, bool death);

private:
    QString m_questID;
    QM::QuestPlayer m_player;
    QString m_currentText;
    QMap<quint32, QString> m_locationsImages;
    QMap<quint32, QString> m_transitionsImages;
    QUrl m_image;
};
}

#endif // OPENSR_QMLQUESTPLAYER_H
