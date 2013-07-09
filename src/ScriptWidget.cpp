/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/ScriptWidget.h"
#include "OpenSR/private/ScriptWidget_p.h"

namespace Rangers
{

ScriptWidget::ScriptWidget(): Widget()
{
}

void ScriptWidget::draw() const
{
    RANGERS_D(const ScriptWidget);
    if (!prepareDraw())
        return;

    std::list<boost::shared_ptr<Object> > children = d->children;

    for (std::list<boost::shared_ptr<Object> >::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->draw();

    endDraw();
}

void ScriptWidget::processLogic(int dt)
{
    RANGERS_D(const ScriptWidget);

    std::list<boost::shared_ptr<Object> > children = d->children;

    for (std::list<boost::shared_ptr<Object> >::const_iterator i = children.begin(); i != children.end(); i++)
        (*i)->processLogic(dt);
}
}
