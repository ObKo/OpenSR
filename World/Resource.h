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

#ifndef OPENSR_WORLD_RESOURCE_H
#define OPENSR_WORLD_RESOURCE_H

#include "World.h"

#include <QObject>
#include <QExplicitlySharedDataPointer>
#include <QDebug>

namespace OpenSR
{
namespace World
{
struct OPENSR_WORLD_API ResourceData: public QSharedData
{
public:
    quint32 id;
    QVariant data;
};

/*!
 * \brief Class used to handle non-object world data (e.g. styles)
 * All Resource subclasses should use Q_GADGET and Q_PROPERTY to be accessible in JS/QML
 */
class OPENSR_WORLD_API Resource
{
    Q_GADGET

    Q_PROPERTY(quint32 id READ id)

public:
    Resource();

    quint32 id() const;

    //! Makes resource known by ResourceManager.
    void registerResource();

    template<class T>
    T getData() const
    {
        auto d = d_func_const();
        return qvariant_cast<T>(d->data);
    }

    template<class T>
    void setData(const T& data)
    {
        auto d = d_func();
        d->data = QVariant::fromValue(data);
    }

    template<class T>
    static void replaceData(T& target, const Resource& source)
    {
        target.m_d = source.m_d;
    }

protected:
    //! Detaches private pointer, updates id and returns private pointer.
    QExplicitlySharedDataPointer<ResourceData> d_func();
    //! Returns private pointer.
    const QExplicitlySharedDataPointer<ResourceData> d_func_const() const;

private:
    QExplicitlySharedDataPointer<ResourceData> m_d;
    friend QDataStream& operator<<(QDataStream & stream, const Resource& res);
    friend QDataStream& operator>>(QDataStream & stream, Resource& res);
};

QDataStream& operator<<(QDataStream & stream, const Resource& res);
QDataStream& operator>>(QDataStream & stream, Resource& res);
}
}

Q_DECLARE_METATYPE(OpenSR::World::Resource)

#endif // OPENSR_WORLD_RESOURCE_H
