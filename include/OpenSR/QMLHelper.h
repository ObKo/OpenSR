/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 - 2017 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_QMLHELPER_H
#define OPENSR_QMLHELPER_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/Types.h>
#include <OpenSR/QM/QM.h>
#include <QObject>
#include <QVariant>

namespace OpenSR
{
namespace QML
{
class QMLHelper: public QObject
{
    Q_OBJECT
public:
    QMLHelper(QObject *parent = 0);

    Q_INVOKABLE QVariant questInfo(const QUrl& url);

    static QVariant convertQuestInfoToJS(const QM::QuestInfo& info);
    static void registerQMLTypes(const char* uri);
};
}
}

#endif // OPENSR_QMLHELPER_H
