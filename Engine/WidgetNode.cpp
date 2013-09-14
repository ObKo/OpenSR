/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/WidgetNode.h"
#include "OpenSR/private/WidgetNode_p.h"

namespace Rangers
{
WidgetNode::WidgetNode(): Widget(*(new WidgetNodePrivate()))
{
}

WidgetNode::WidgetNode(WidgetNodePrivate &p): Widget(p)
{
}

void WidgetNode::draw() const
{
    RANGERS_D(const WidgetNode);
    if (!prepareDraw())
        return;

    std::list<boost::shared_ptr<Object> > children = d->children;
    for (std::list<boost::shared_ptr<Object> >::const_iterator i = children.begin(); i != children.end(); ++i)
        (*i)->draw();

    endDraw();
}

void WidgetNode::processLogic(int dt)
{
    lock();
    RANGERS_D(WidgetNode);
    std::list<boost::shared_ptr<Object> > children = d->children;
    unlock();
    for (std::list<boost::shared_ptr<Object> >::const_iterator i = children.begin(); i != children.end(); ++i)
        (*i)->processLogic(dt);
}
}
