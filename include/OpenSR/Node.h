/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_NODE_H
#define RANGERS_NODE_H

#include "OpenSR/Object.h"

namespace Rangers
{
class NodePrivate;
class RANGERS_ENGINE_API Node: public Object
{
    RANGERS_DECLARE_PRIVATE(Node)
public:
    Node(Object *parent = 0);
    virtual void draw() const;
    virtual void processLogic(int dt);

protected:
    Node(NodePrivate &p, Object *parent = 0);

    RANGERS_DISABLE_COPY(Node)
};
}
#endif
