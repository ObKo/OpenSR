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

#ifndef OPENSR_WORLD_WORLDMANAGER_H
#define OPENSR_WORLD_WORLDMANAGER_H

#include "World.h"

#include "WorldContext.h"

#include <QObject>

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API WorldManager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* context READ context NOTIFY contextChanged)
public:
    WorldManager(QObject *parent = 0);
    virtual ~WorldManager();

    quint32 getNextId() const;

    static WorldManager *instance();

    WorldContext* context() const;
    Q_INVOKABLE void generateWorld(const QString& genScriptUrl);
    Q_INVOKABLE bool saveWorld(const QString& path);
    Q_INVOKABLE bool loadWorld(const QString& path);

Q_SIGNALS:
    void contextChanged();

private:
    WorldContext* m_context;

    static WorldManager* m_staticInstance;
    static quint32 m_idPool;

    Q_DISABLE_COPY(WorldManager)
};
}
}

#endif // OPENSR_WORLD_WORLDMANAGER_H
