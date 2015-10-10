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

#include "WorldManager.h"

#include <QHash>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QMetaProperty>
#include <QUrl>

#include <OpenSR/Engine.h>

#include "WorldObject.h"
#include "WorldContext.h"

namespace OpenSR
{
namespace World
{
class Race;
class Item;
class Goods;
class Equipment;
class Micromodulus;
class Artefact;
class Hull;
class Engine;
class Tank;
class Droid;
class CargoHook;
class DefenceGenerator;
class Radar;
class Scanner;
class Weapon;
class Sector;
class SpaceObject;
class Container;
class Asteroid;
class Planet;
class MannedObject;
class InhabitedPlanet;
class DesertPlanet;
class Ship;
class SpaceStation;

namespace
{

static QMap<quint32, const QMetaObject*> metaMap;
const quint32 SAVE_FILE_SIGNATURE = 0x5352534F;
const quint32 OBJECT_SIGNATURE = 0x4F575253;

struct ObjectHeader
{
    quint32 type;
    quint32 id;
    quint32 parentId;
};

QDataStream& operator<<(QDataStream& stream, const ObjectHeader& h)
{
    return stream << h.type << h.id << h.parentId;
}

QDataStream& operator>>(QDataStream& stream, ObjectHeader& h)
{
    return stream >> h.type >> h.id >> h.parentId;
}

WorldObject* createObject(QMap<quint32, WorldObject*>& objects, QMap<quint32, ObjectHeader>& headers, const ObjectHeader& header)
{
    WorldObject *parent = 0;
    headers.remove(header.id);
    if (header.parentId)
    {
        auto p = objects.find(header.parentId);
        if (p == objects.end())
        {
            auto ph = headers.find(header.parentId);
            if (ph == headers.end())
            {
                qWarning() << "Inconsistent save file.";
                return 0;
            }
            parent = createObject(objects, headers, *ph);
        }
        else
            parent = *p;
    }
    auto metai = metaMap.find(header.type);
    if (metai == metaMap.end())
    {
        qWarning() << "Unknow object in save file";
        return 0;
    }
    const QMetaObject *meta = *metai;
    WorldObject *obj = qobject_cast<WorldObject*>(meta->newInstance(Q_ARG(WorldObject*, parent), Q_ARG(quint32, header.id)));
    if (obj)
        objects.insert(obj->id(), obj);
    return obj;
}

bool writeObject(const WorldObject* object, QDataStream& stream)
{
    const QMetaObject *meta = object->metaObject();
    for (int i = 0; i < meta->propertyCount(); ++i)
    {
        QMetaProperty p = meta->property(i);
        //FIXME: Save property id?
        if (p.isReadable() && p.isStored())
            stream << p.read(object);

        if (stream.status() != QDataStream::Ok)
            return false;
    }
    return object->save(stream);
}

bool readObject(WorldObject* object, QDataStream& stream, const QMap<quint32, WorldObject*>& objects)
{
    const QMetaObject *meta = object->metaObject();
    for (int i = 0; i < meta->propertyCount(); ++i)
    {
        QMetaProperty p = meta->property(i);
        if (p.isWritable() && p.isStored())
        {
            QVariant value;
            stream >> value;
            p.write(object, value);
        }

        if (stream.status() != QDataStream::Ok)
            return false;
    }
    return object->load(stream, objects);
}

void countObjects(QList<WorldObject*>& objects, WorldObject* current)
{
    if (!current)
        return;

    objects.push_back(current);
    for (QObject* c : current->children())
    {
        WorldObject *o = qobject_cast<WorldObject*>(c);
        if (!o)
            continue;
        countObjects(objects, o);
    }
}
}

WorldManager* WorldManager::m_staticInstance = 0;
quint32 WorldManager::m_idPool = 0;

WorldManager::WorldManager(QObject *parent): QObject(parent),
    m_context(0)
{
    if (WorldManager::m_staticInstance)
        throw std::runtime_error("WorldManager constructed twice");

    WorldManager::m_staticInstance = this;

    metaMap.insert(WorldObject::staticTypeId<WorldObject>(), WorldObject::staticTypeMeta<WorldObject>());
    metaMap.insert(WorldObject::staticTypeId<WorldContext>(), WorldObject::staticTypeMeta<WorldContext>());
    metaMap.insert(WorldObject::staticTypeId<Race>(), WorldObject::staticTypeMeta<Race>());
    metaMap.insert(WorldObject::staticTypeId<Item>(), WorldObject::staticTypeMeta<Item>());
    metaMap.insert(WorldObject::staticTypeId<Goods>(), WorldObject::staticTypeMeta<Goods>());
    metaMap.insert(WorldObject::staticTypeId<Equipment>(), WorldObject::staticTypeMeta<Equipment>());
    metaMap.insert(WorldObject::staticTypeId<Micromodulus>(), WorldObject::staticTypeMeta<Micromodulus>());
    metaMap.insert(WorldObject::staticTypeId<Artefact>(), WorldObject::staticTypeMeta<Artefact>());
    metaMap.insert(WorldObject::staticTypeId<Hull>(), WorldObject::staticTypeMeta<Hull>());
    metaMap.insert(WorldObject::staticTypeId<World::Engine>(), WorldObject::staticTypeMeta<World::Engine>());
    metaMap.insert(WorldObject::staticTypeId<Tank>(), WorldObject::staticTypeMeta<Tank>());
    metaMap.insert(WorldObject::staticTypeId<Droid>(), WorldObject::staticTypeMeta<Droid>());
    metaMap.insert(WorldObject::staticTypeId<CargoHook>(), WorldObject::staticTypeMeta<CargoHook>());
    metaMap.insert(WorldObject::staticTypeId<DefenceGenerator>(), WorldObject::staticTypeMeta<DefenceGenerator>());
    metaMap.insert(WorldObject::staticTypeId<Radar>(), WorldObject::staticTypeMeta<Radar>());
    metaMap.insert(WorldObject::staticTypeId<Scanner>(), WorldObject::staticTypeMeta<Scanner>());
    metaMap.insert(WorldObject::staticTypeId<Weapon>(), WorldObject::staticTypeMeta<Weapon>());
    metaMap.insert(WorldObject::staticTypeId<Sector>(), WorldObject::staticTypeMeta<Sector>());
    metaMap.insert(WorldObject::staticTypeId<PlanetarySystem>(), WorldObject::staticTypeMeta<PlanetarySystem>());
    metaMap.insert(WorldObject::staticTypeId<SpaceObject>(), WorldObject::staticTypeMeta<SpaceObject>());
    metaMap.insert(WorldObject::staticTypeId<Container>(), WorldObject::staticTypeMeta<Container>());
    metaMap.insert(WorldObject::staticTypeId<Asteroid>(), WorldObject::staticTypeMeta<Asteroid>());
    metaMap.insert(WorldObject::staticTypeId<Planet>(), WorldObject::staticTypeMeta<Planet>());
    metaMap.insert(WorldObject::staticTypeId<MannedObject>(), WorldObject::staticTypeMeta<MannedObject>());
    metaMap.insert(WorldObject::staticTypeId<InhabitedPlanet>(), WorldObject::staticTypeMeta<InhabitedPlanet>());
    metaMap.insert(WorldObject::staticTypeId<DesertPlanet>(), WorldObject::staticTypeMeta<DesertPlanet>());
    metaMap.insert(WorldObject::staticTypeId<Ship>(), WorldObject::staticTypeMeta<Ship>());
    metaMap.insert(WorldObject::staticTypeId<SpaceStation>(), WorldObject::staticTypeMeta<SpaceStation>());
    metaMap.insert(WorldObject::staticTypeId<ResourceManager>(), WorldObject::staticTypeMeta<ResourceManager>());
}

QString WorldManager::typeName(quint32 type) const
{
    auto metai = metaMap.find(type);

    if (metai == metaMap.end())
        return QString();

    return metai.value()->className();
}

WorldManager::~WorldManager()
{
    if (m_context)
        delete m_context;
    WorldManager::m_staticInstance = 0;
}

WorldContext* WorldManager::context() const
{
    return m_context;
}

WorldManager* WorldManager::instance()
{
    return WorldManager::m_staticInstance;
}

quint32 WorldManager::getNextId() const
{
    return ++WorldManager::m_idPool;
}

bool WorldManager::loadWorld(const QString& path)
{
    QFile f(path);
    f.open(QIODevice::ReadOnly);

    if (!f.isOpen())
    {
        qWarning().noquote() << QString("Cannot load save file \"%1\"").arg(path);
        return false;
    }

    QDataStream stream(&f);

    quint32 sig;
    stream >> sig;

    if (sig != SAVE_FILE_SIGNATURE)
    {
        qWarning().noquote() << QString("Cannot load save file \"%1\": not a save file.").arg(path);
        return false;
    }

    QMap<quint32, WorldObject*> objects;
    quint32 objectCount;
    QMap<quint32, ObjectHeader> headersMap;

    stream >> objectCount;

    if (!objectCount)
    {
        qWarning() << "Invalid save file";
        f.close();
        return false;
    }

    if (m_context)
        delete m_context;

    ObjectHeader h;

    // Context is always first
    stream >> h;
    m_context = qobject_cast<WorldContext*>(createObject(objects, headersMap, h));
    m_context->setParent(this);
    emit(contextChanged());

    if (!m_context)
    {
        qWarning() << "Invalid save file";
        f.close();
        return false;
    }

    for (int i = 0; i < objectCount - 1; i++)
    {
        stream >> h;
        headersMap[h.id] = h;
    }

    while (!headersMap.isEmpty())
    {
        ObjectHeader h = *(headersMap.begin());
        WorldObject *o = createObject(objects, headersMap, h);
        if (!o)
        {
            qWarning() << "Invalid save file";
            f.close();
            return false;
        }
    }

    for (int i = 0; i < objectCount; i++)
    {
        quint32 sig, id;
        stream >> sig;
        if (sig != OBJECT_SIGNATURE)
        {
            qWarning() << "Invalid object in save file.";
            f.close();
            return false;
        }
        stream >> id;
        auto oi = objects.find(id);
        if (oi == objects.end() || !readObject(oi.value(), stream, objects))
        {
            qWarning() << "Invalid object in save file.";
            f.close();
            return false;
        }
    }

    f.close();
    return true;
}

bool WorldManager::saveWorld(const QString& path)
{
    QFile f(path);
    f.open(QIODevice::WriteOnly);

    if (!f.isOpen())
    {
        qWarning().noquote() << QString("Cannot open save file \"%1\": %2").arg(path, f.errorString());
        return false;
    }

    QDataStream stream(&f);

    QList<WorldObject*> objects;
    countObjects(objects, m_context);

    stream << SAVE_FILE_SIGNATURE;
    stream << objects.count();

    for (WorldObject* o : objects)
    {
        o->prepareSave();

        ObjectHeader h;
        h.id = o->id();
        WorldObject *p = qobject_cast<WorldObject*>(o->parent());
        if (p)
            h.parentId = p->id();
        else
            h.parentId = 0;
        h.type = o->typeId();

        stream << h;
    }

    for (WorldObject* o : objects)
    {
        stream << OBJECT_SIGNATURE;
        stream << o->id();
        if (!writeObject(o, stream))
        {
            f.close();
            return false;
        }
    }

    f.close();
    return true;
}


void WorldManager::generateWorld(const QString& genScriptUrl)
{
    if (m_context)
    {
        delete m_context;
        m_context = 0;
    }

    m_context = new WorldContext();
    m_context->setParent(this);
    qobject_cast<OpenSR::Engine*>(qApp)->execScript(genScriptUrl);
    emit(contextChanged());
}
}
}
