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

#ifndef OPENSR_WORLD_WORLDOBJECT_H
#define OPENSR_WORLD_WORLDOBJECT_H

#include "World.h"

#include <QObject>

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API WorldObject: public QObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(uint32_t id READ id)
    Q_PROPERTY(uint32_t typeId READ typeId)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString namePrefix READ namePrefix)

public:
    Q_INVOKABLE WorldObject(uint32_t id = 0, WorldObject *parent = 0);
    virtual ~WorldObject();

    uint32_t id() const;
    virtual uint32_t typeId() const;
    QString name() const;
    virtual QString namePrefix() const;

    void setName(const QString& name);

    bool save(QDataStream &stream) const;
    bool load(QDataStream &stream);

Q_SIGNALS:
    void nameChanged();

private:
    uint32_t m_id;
    QString m_name;
};
}
}

#endif // OPENSR_WORLD_WORLDOBJECT_H
