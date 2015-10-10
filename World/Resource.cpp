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

#include "Resource.h"
#include "ResourceManager.h"

#include <QDataStream>

namespace OpenSR
{
namespace World
{
Resource::Resource(): m_d(new ResourceData())
{
    m_d->id = 0;
}

quint32 Resource::id() const
{
    return m_d->id;
}

void Resource::registerResource()
{
    ResourceManager *m = ResourceManager::instance();
    if(!m)
        return;

    quint32 id = m->registerResource(*this);
    m_d->id = id;
}

QExplicitlySharedDataPointer<ResourceData> Resource::d_func()
{
    if (m_d.data()->ref.load() > 1)
    {
        m_d.detach();
        m_d->id = 0;
    }
    return m_d;
}

const QExplicitlySharedDataPointer<ResourceData> Resource::d_func_const() const
{
    return m_d;
}


QDataStream& operator<<(QDataStream & stream, const Resource& res)
{
    auto d = res.d_func_const();
    return stream << d->id << d->data;
}

QDataStream& operator>>(QDataStream & stream, Resource& res)
{
    auto d = res.d_func();
    return stream >> d->id >> d->data;
}
}
}
