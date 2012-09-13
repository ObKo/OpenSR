/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_OBJECT_P_H
#define RANGERS_OBJECT_P_H

#include "global.h"
#include "Types.h"
#include <list>
#include <boost/thread/recursive_mutex.hpp>

namespace Rangers
{
class Object;
class ObjectPrivate
{
    RANGERS_DECLARE_PUBLIC(Object)
public:
    virtual ~ObjectPrivate();
       
    int m_layer;
    mutable boost::recursive_mutex m_mutex;
    int m_color;
    bool m_needUpdate;
    Object *m_parent;
    std::list<Object*> m_children;
    Vector m_position;
    float m_rotation;
    
    Object *m_q;
};
}

#endif