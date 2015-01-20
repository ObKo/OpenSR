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

#include "QMLPlugin.h"

#include <OpenSR/QM/QM.h>

#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Sound.h>
#include <OpenSR/GAIAnimatedImage.h>
#include <OpenSR/QMLQuestPlayer.h>
#include <OpenSR/Music.h>

#include <QtQml>
#include <QVariant>

static QJSValue engineSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    QJSValue eng = scriptEngine->newQObject(qobject_cast<OpenSR::Engine*>(qApp));
    return eng;
}

static QJSValue qmSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    QJSValue qm = scriptEngine->newQObject(new OpenSR::QML::QMLQMExporter(scriptEngine));
    return qm;
}

void OpenSRPlugin::registerTypes(const char* uri)
{
    using namespace OpenSR;
    qmlRegisterSingletonType(uri, 1, 0, "Engine", engineSingletonProvider);
    qmlRegisterSingletonType(uri, 1, 0, "QM", qmSingletonProvider);
    qmlRegisterType<Sound>(uri, 1, 0, "Sound");
    qmlRegisterType<GAIAnimatedImage>(uri, 1, 0, "GAIAnimatedImage");
    qmlRegisterType<QMLQuestPlayer>(uri, 1, 0, "QuestPlayer");
    qmlRegisterType<Music>(uri, 1, 0, "Music");
}

namespace OpenSR
{
namespace QML
{
QMLQMExporter::QMLQMExporter(QObject* parent): QObject(parent)
{

}

QVariant QMLQMExporter::loadQuestInfo(const QUrl& url)
{
    QIODevice *d = qobject_cast<Engine*>(qApp)->resources()->getIODevice(url);
    if (!d || !d->isOpen())
        return QVariant();

    QVariant result = convertQuestInfoToJS(QM::readQuestInfo(d));

    delete d;

    return result;
}

QVariant QMLQMExporter::convertQuestInfoToJS(const QM::QuestInfo& info)
{
    QVariantMap map;

    map["races"] = info.races;
    map["doneImmediately"] = info.doneImmediately;
    map["planetRaces"] = info.planetRaces;
    map["playerTypes"] = info.playerTypes;
    map["playerRaces"] = info.playerRaces;
    map["relation"] = info.relation;
    map["difficulty"] = info.difficulty;
    map["winnerText"] = info.winnerText;
    map["descriptionText"] = info.descriptionText;

    return map;
}
}
}

