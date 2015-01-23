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

#include "WorldManager.h"
#include "WorldBindings.h"

namespace OpenSR
{
namespace World
{
bool WorldPlugin::initPlugin(OpenSR::Engine* engine)
{
    WorldManager *manager = new WorldManager(engine);

    bindWorldTypes(engine->scriptEngine(), engine->qmlEngine());

    return true;
}
}
}
