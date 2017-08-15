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
#include <QQmlEngine>

#include <OpenSR/Engine.h>

#include "WorldObject.h"
#include "WorldContext.h"

namespace OpenSR
{
namespace World
{
namespace
{
static QMap<quint32, const QMetaObject*> metaMap;
const quint32 SAVE_FILE_SIGNATURE = 0x5352534F;
const quint32 OBJECT_SIGNATURE = 0x4F575253;

struct ObjectHeader
{
    quint32 type;
    quint32 id;
    QString idName;
    quint32 parentId;
};

QDataStream& operator<<(QDataStream& stream, const ObjectHeader& h)
{
    return stream << h.type << h.id << h.idName << h.parentId;
}

QDataStream& operator>>(QDataStream& stream, ObjectHeader& h)
{
    return stream >> h.type >> h.id >> h.idName >> h.parentId;
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
    obj->setObjectName(header.idName);
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

TurnAnimation::TurnAnimation(QObject *parent): QAbstractAnimation(parent)
{
}

int	TurnAnimation::duration() const
{
    return 2000;
}

void TurnAnimation::updateCurrentTime(int currentTime)
{
    if (state() != TurnAnimation::Running)
        return;

    WorldContext *ctx = WorldManager::instance()->context();
    if (ctx && ctx->currentSystem())
        ctx->currentSystem()->processTurn((float)currentTime / (float)duration());
}

WorldManager* WorldManager::m_staticInstance = 0;
quint32 WorldManager::m_idPool = 0;

WorldManager::WorldManager(QObject *parent): QObject(parent),
    m_context(0)
{
    if (WorldManager::m_staticInstance)
        throw std::runtime_error("WorldManager constructed twice");

    WorldManager::m_staticInstance = this;

    metaMap.insert(WorldObject::staticTypeId<World::WorldObject>(), WorldObject::staticTypeMeta<World::WorldObject>());
    metaMap.insert(WorldObject::staticTypeId<World::WorldContext>(), WorldObject::staticTypeMeta<World::WorldContext>());
    metaMap.insert(WorldObject::staticTypeId<World::Race>(), WorldObject::staticTypeMeta<World::Race>());
    metaMap.insert(WorldObject::staticTypeId<World::Item>(), WorldObject::staticTypeMeta<World::Item>());
    metaMap.insert(WorldObject::staticTypeId<World::Goods>(), WorldObject::staticTypeMeta<World::Goods>());
    metaMap.insert(WorldObject::staticTypeId<World::Equipment>(), WorldObject::staticTypeMeta<World::Equipment>());
    metaMap.insert(WorldObject::staticTypeId<World::Micromodulus>(), WorldObject::staticTypeMeta<World::Micromodulus>());
    metaMap.insert(WorldObject::staticTypeId<World::Artefact>(), WorldObject::staticTypeMeta<World::Artefact>());
    metaMap.insert(WorldObject::staticTypeId<World::Hull>(), WorldObject::staticTypeMeta<World::Hull>());
    metaMap.insert(WorldObject::staticTypeId<World::Engine>(), WorldObject::staticTypeMeta<World::Engine>());
    metaMap.insert(WorldObject::staticTypeId<World::Tank>(), WorldObject::staticTypeMeta<World::Tank>());
    metaMap.insert(WorldObject::staticTypeId<World::Droid>(), WorldObject::staticTypeMeta<World::Droid>());
    metaMap.insert(WorldObject::staticTypeId<World::CargoHook>(), WorldObject::staticTypeMeta<World::CargoHook>());
    metaMap.insert(WorldObject::staticTypeId<World::DefenceGenerator>(), WorldObject::staticTypeMeta<World::DefenceGenerator>());
    metaMap.insert(WorldObject::staticTypeId<World::Radar>(), WorldObject::staticTypeMeta<World::Radar>());
    metaMap.insert(WorldObject::staticTypeId<World::Scanner>(), WorldObject::staticTypeMeta<World::Scanner>());
    metaMap.insert(WorldObject::staticTypeId<World::Weapon>(), WorldObject::staticTypeMeta<World::Weapon>());
    metaMap.insert(WorldObject::staticTypeId<World::Sector>(), WorldObject::staticTypeMeta<World::Sector>());
    metaMap.insert(WorldObject::staticTypeId<World::PlanetarySystem>(), WorldObject::staticTypeMeta<World::PlanetarySystem>());
    metaMap.insert(WorldObject::staticTypeId<World::SpaceObject>(), WorldObject::staticTypeMeta<World::SpaceObject>());
    metaMap.insert(WorldObject::staticTypeId<World::Container>(), WorldObject::staticTypeMeta<World::Container>());
    metaMap.insert(WorldObject::staticTypeId<World::Asteroid>(), WorldObject::staticTypeMeta<World::Asteroid>());
    metaMap.insert(WorldObject::staticTypeId<World::Planet>(), WorldObject::staticTypeMeta<World::Planet>());
    metaMap.insert(WorldObject::staticTypeId<World::MannedObject>(), WorldObject::staticTypeMeta<World::MannedObject>());
    metaMap.insert(WorldObject::staticTypeId<World::InhabitedPlanet>(), WorldObject::staticTypeMeta<World::InhabitedPlanet>());
    metaMap.insert(WorldObject::staticTypeId<World::DesertPlanet>(), WorldObject::staticTypeMeta<World::DesertPlanet>());
    metaMap.insert(WorldObject::staticTypeId<World::Ship>(), WorldObject::staticTypeMeta<World::Ship>());
    metaMap.insert(WorldObject::staticTypeId<World::SpaceStation>(), WorldObject::staticTypeMeta<World::SpaceStation>());
    metaMap.insert(WorldObject::staticTypeId<World::ResourceManager>(), WorldObject::staticTypeMeta<World::ResourceManager>());

    m_animation = new TurnAnimation(this);
    connect(m_animation, SIGNAL(finished()), this, SLOT(finishTurn()));
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
    delete m_animation;
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

void WorldManager::startTurn()
{
    if (!m_context)
        return;

    if (m_animation->state() == TurnAnimation::Running)
        return;

    m_context->startTurn();

    m_animation->setLoopCount(1);
    m_animation->setCurrentTime(0);
    m_animation->start();
}

void WorldManager::finishTurn()
{
    if (m_animation->state() == TurnAnimation::Running)
        m_animation->stop();

    m_context->finishTurn();
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
        h.idName = o->objectName();
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

WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldManager, RaceStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldManager, PlanetarySystemStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldManager, AsteroidStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldManager, PlanetStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldManager, StationStyle)

WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Race)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Item)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Goods)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Equipment)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Micromodulus)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Artefact)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Hull)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Engine)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Tank)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Droid)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, CargoHook)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, DefenceGenerator)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Radar)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Scanner)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Weapon)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Sector)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, PlanetarySystem)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, SpaceObject)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Container)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Asteroid)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Planet)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, MannedObject)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, InhabitedPlanet)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, DesertPlanet)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, Ship)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldManager, SpaceStation)

}
}
