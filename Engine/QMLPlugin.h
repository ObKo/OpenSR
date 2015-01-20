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

#ifndef OPENSR_QMLPLUGIN_H
#define OPENSR_QMLPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QVariant>
#include <OpenSR/QM/QM.h>

class OpenSRPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

//TODO: Separate this
namespace OpenSR
{
namespace QML
{
class QMLQMExporter: public QObject
{
    Q_OBJECT
public:
    QMLQMExporter(QObject *parent = 0);

    Q_INVOKABLE QVariant loadQuestInfo(const QUrl& url);

private:
    QVariant convertQuestInfoToJS(const QM::QuestInfo& info);
};
}
}

#endif // OPENSR_QMLPLUGIN_H