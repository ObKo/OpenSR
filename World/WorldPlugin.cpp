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

#include "WorldPlugin.h"

#include <QQmlEngine>
#include <QtQml>

#include "WorldObject.h"
#include "WorldManager.h"

namespace OpenSR
{
namespace World
{
static QJSValue managerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    QJSValue manager = scriptEngine->newQObject(WorldManager::instance());
    return manager;
}

bool WorldPlugin::initPlugin(Engine* engine)
{
    WorldManager *manager = new WorldManager(engine);

    qmlRegisterSingletonType("OpenSR.World", 1, 0, "WorldManager", managerSingletonProvider);
    return true;
}
}
}
