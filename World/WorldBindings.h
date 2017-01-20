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

#ifndef OPENSR_WORLD_WORLDBINDINGS_H
#define OPENSR_WORLD_WORLDBINDINGS_H

#include "WorldObject.h"

#include "PlanetarySystem.h"
#include "Race.h"
#include "Asteroid.h"
#include "Planet.h"
#include "SpaceStation.h"

class QJSEngine;
class QQmlEngine;

#define WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Class) Q_INVOKABLE OpenSR::World::Class* Class(WorldObject *parent = 0);
#define WORLD_DECLARE_JS_GADGET_CONSTRUCTOR(Class) Q_INVOKABLE OpenSR::World::Class Class();

#define WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(Factory, Class)\
    OpenSR::World::Class* Factory::Class(WorldObject *parent) {\
        OpenSR::World::Class *obj = WorldObject::createObject<World::Class>(parent);\
        QQmlEngine::setObjectOwnership((QObject*)obj, QQmlEngine::JavaScriptOwnership);\
        return obj;\
    }

#define WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(Factory, Class)\
    Class Factory::Class() {\
        return World::Class();\
    }

namespace OpenSR
{
namespace World
{
void bindWorldTypes(QJSEngine *script, QQmlEngine *qml);
}
}

#endif // OPENSR_WORLD_WORLDBINDINGS_H
