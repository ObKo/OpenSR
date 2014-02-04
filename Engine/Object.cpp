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

#include "OpenSR/Object.h"
#include "OpenSR/private/Object_p.h"

#include <stack>
#include <algorithm>
#include <boost/math/constants/constants.hpp>

#include "OpenSR/Log.h"
#include "OpenSR/Engine.h"

#define DEG2RAD(x) (x * boost::math::constants::pi<double>() / 180.0)

namespace Rangers
{
ObjectPrivate::~ObjectPrivate()
{
}

ObjectPrivate::ObjectPrivate()
{
    rotation = 0;
    needUpdate = false;
    position.x = 0.0f;
    position.y = 0.0f;
    layer = 0;
    parent = 0;
}

Object::Object(): m_d(new ObjectPrivate())
{
    RANGERS_D(Object);
    d->m_q = this;
}

Object::Object(ObjectPrivate &dd): m_d(&dd)
{
    RANGERS_D(Object);
    d->m_q = this;
}

Object::~Object()
{
    RANGERS_D(Object);

    Engine::instance().unmarkToUpdate(this);

    for (std::list<boost::shared_ptr<Object> >::iterator i = d->children.begin(); i != d->children.end(); ++i)
    {
        if ((*i)->parent() == this)
            (*i)->setParent(0);
    }

    delete d;
}

bool Object::prepareDraw() const
{
    lock();
    RANGERS_D(const Object);
    //! FIXME: VERY, VERY, VERY DIRTY HACK!!!
    if (d->needUpdate)
        ((Object*)this)->processMain();
    glPushMatrix();
    glTranslatef(d->position.x, d->position.y, 0);
    glRotatef(d->rotation, 0, 0, -1);
    glColor4f(d->color.r, d->color.g, d->color.b, d->color.a);
    return true;
}

void Object::processMain()
{
    lock();
    RANGERS_D(Object);
    d->needUpdate = false;
    unlock();
}

void Object::draw() const
{
}

Vector Object::mapFromParent(const Vector& v) const
{
    RANGERS_D(const Object);
    float c = cos(DEG2RAD(d->rotation));
    float s = -sin(DEG2RAD(d->rotation));
    Vector result;
    result.x = c * (v.x - d->position.x) + s * (v.y - d->position.y);
    result.y = -s * (v.x - d->position.x) + c * (v.y - d->position.y);
    return result;
}

//FIXME: Adding locks causing deadlocks.
Vector Object::mapToParent(const Vector& v) const
{
    RANGERS_D(const Object);
    float c = cos(DEG2RAD(d->rotation));
    float s = -sin(DEG2RAD(d->rotation));
    Vector result;
    result.x = c * v.x - s * v.y + d->position.x;
    result.y = s * v.x + c * v.y + d->position.y;
    return result;
}

Vector Object::mapFromGlobal(const Vector& v) const
{
    RANGERS_D(const Object);
    std::stack<Object *> objects;
    //objects.push(this);
    Object *currentParent = d->parent;
    while (currentParent)
    {
        objects.push(currentParent);
        currentParent = currentParent->parent();
    }
    Vector result = v;
    for (; !objects.empty(); objects.pop())
    {
        result = objects.top()->mapFromParent(result);
    }
    result = mapFromParent(result);
    return result;
}

Rect Object::mapToParent(const Rect& r) const
{
    Vector a(r.x, r.y), b(r.x + r.width, r.y), c(r.x + r.width, r.y + r.height), d(r.x, r.y + r.height);
    a = mapToParent(a);
    b = mapToParent(b);
    c = mapToParent(c);
    d = mapToParent(d);
    Rect result;
    result.x = std::min(std::min(a.x, b.x), std::min(c.x, d.x));
    result.y = std::min(std::min(a.y, b.y), std::min(c.y, d.y));
    result.width = std::max(std::max(a.x, b.x), std::max(c.x, d.x)) - result.x;
    result.height = std::max(std::max(a.y, b.y), std::max(c.y, d.y)) - result.y;
    return result;
}

//FIXME: Locking parent obvious causing deadlock (logic thread and nodes).
Vector Object::mapToGlobal(const Vector& v) const
{
    RANGERS_D(const Object);
    Vector result = mapToParent(v);
    Object *currentParent = d->parent;
    while (currentParent)
    {
        result = currentParent->mapToParent(result);
        currentParent = currentParent->parent();
    }
    return result;
}

Rect Object::mapToGlobal(const Rect& r) const
{
    Vector a(r.x, r.y), b(r.x + r.width, r.y), c(r.x + r.width, r.y + r.height), d(r.x, r.y + r.height);
    a = mapToGlobal(a);
    b = mapToGlobal(b);
    c = mapToGlobal(c);
    d = mapToGlobal(d);
    Rect result;
    result.x = std::min(std::min(a.x, b.x), std::min(c.x, d.x));
    result.y = std::min(std::min(a.y, b.y), std::min(c.y, d.y));
    result.width = std::max(std::max(a.x, b.x), std::max(c.x, d.x)) - result.x;
    result.height = std::max(std::max(a.y, b.y), std::max(c.y, d.y)) - result.y;
    return result;
}

Vector Object::mapFromScreen(const Vector& v) const
{
    GLdouble wx, wy, wz;
    int viewport[4];
    GLdouble p[16];
    GLdouble w[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluUnProject(v.x, v.y, 0, w, p, viewport, &wx, &wy, &wz);

    Vector result = mapFromGlobal(Vector(wx, viewport[3] - wy));

    return result;
}

Vector Object::mapToScreen(const Vector& v) const
{
    Vector global = mapToGlobal(v);
    GLdouble x = global.x, y = global.y;
    GLdouble wx, wy, wz;
    int viewport[4];
    GLdouble p[16];
    GLdouble w[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluProject(x, y, 0, w, p, viewport, &wx, &wy, &wz);
    Vector result(wx, viewport[3] - wy);

    return result;
}

Rect Object::mapToScreen(const Rect& r) const
{
    Rect global = mapToGlobal(r);
    GLdouble wx1, wy1, wx2, wy2, wx3, wy3, wx4, wy4, wz;
    int viewport[4];
    GLdouble p[16];
    GLdouble w[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    glGetDoublev(GL_PROJECTION_MATRIX, p);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluProject(global.x, global.y, 0, w, p, viewport, &wx1, &wy1, &wz);
    gluProject(global.x + global.width, global.y, 0, w, p, viewport, &wx2, &wy2, &wz);
    gluProject(global.x + global.width, global.y + global.height, 0, w, p, viewport, &wx3, &wy3, &wz);
    gluProject(global.x, global.y + global.height, 0, w, p, viewport, &wx4, &wy4, &wz);

    Rect result;
    result.x = std::min(std::min(wx1, wx2), std::min(wx3, wx4));
    result.y = std::min(std::min(wy1, wy2), std::min(wy3, wy4));
    result.width = std::max(std::max(wx1, wx2), std::max(wx3, wx4)) - result.x;
    result.height = std::max(std::max(wy1, wy2), std::max(wy3, wy4)) - result.y;

    return result;
}

void Object::processLogic(int dt)
{
}

void Object::endDraw() const
{
    glPopMatrix();
    unlock();
}

void Object::setColor(const Color& color)
{
    lock();
    RANGERS_D(Object);
    d->color = color;
    unlock();
}

void Object::setPosition(float x, float y)
{
    lock();
    RANGERS_D(Object);
    d->position.x = x;
    d->position.y = y;
    unlock();
}

void Object::setPosition(const Vector& pos)
{
    lock();
    RANGERS_D(Object);
    d->position = pos;
    unlock();
}

void Object::setRotation(float angle)
{
    lock();
    RANGERS_D(Object);
    d->rotation = angle;
    unlock();
}

Vector Object::position() const
{
    RANGERS_D(const Object);
    return d->position;
}

float Object::rotation() const
{
    RANGERS_D(const Object);
    return d->rotation;
}

int Object::layer() const
{
    RANGERS_D(const Object);
    return d->layer;
}

Object* Object::parent() const
{
    RANGERS_D(const Object);
    return d->parent;
}

bool Object::needUpdate() const
{
    RANGERS_D(const Object);
    return d->needUpdate;
}

Color Object::color() const
{
    RANGERS_D(const Object);
    return d->color;
}

void Object::addChild(boost::shared_ptr<Object> object)
{
    lock();
    RANGERS_D(Object);

    if (!object)
        return;

    //FIXME: Remove child from old parent instead of error?
    if (object->parent() && object->parent() != this)
    {
        Log::debug() << "Object::addChild(): object already has a parent!";
        return;
    }

    //TODO: Check dublicates

    object->setParent(this);
    for (std::list<boost::shared_ptr<Object> >::iterator i = d->children.begin(); i != d->children.end(); ++i)
    {
        if ((*i)->layer() > object->layer())
        {
            d->children.insert(i, object);
            unlock();
            return;
        }
    }
    d->children.push_back(object);
    unlock();
}


std::list<boost::shared_ptr<Object> > Object::children() const
{
    RANGERS_D(const Object);
    return d->children;
}

void Object::setParent(Object *parent)
{
    lock();
    RANGERS_D(Object);
    d->parent = parent;
    unlock();
}

void Object::removeChild(boost::shared_ptr<Object> object)
{
    lock();
    RANGERS_D(Object);

    if (!object)
        return;

    //FIXME: Debug
    if (object->parent() && (object->parent() != this))
    {
        Log::debug() << "Object::removeChild(): parent mismatch!";
        d->children.remove(object);
        return;
    }

    object->setParent(0);
    d->children.remove(object);
    unlock();
}

void Object::setLayer(int layer)
{
    lock();
    RANGERS_D(Object);
    d->layer = layer;
    if (d->parent)
    {
        boost::shared_ptr<Object> self = d->parent->getChild(this);
        if (!self)
        {
            Log::debug() << "Object::setLayer(): Invalid parent!";
            unlock();
            return;
        }
        d->parent->removeChild(self);
        d->parent->addChild(self);
    }
    unlock();
}

boost::shared_ptr<Object> Object::getChild(Object *ptr) const
{
    RANGERS_D(const Object);

    if (!ptr)
        return boost::shared_ptr<Object>();

    lock();
    std::list<boost::shared_ptr<Object> > children = d->children;
    for (std::list<boost::shared_ptr<Object> >::const_iterator i = children.begin(); i != children.end(); ++i)
    {
        if ((*i).get() == ptr)
        {
            unlock();
            return *i;
        }
    }
    unlock();
    return boost::shared_ptr<Object>();
}

void Object::markToUpdate()
{
    RANGERS_D(Object);
    if (!d->needUpdate)
    {
        d->needUpdate = true;
        Engine::instance().markToUpdate(this);
    }
}

void Object::lock() const
{
    RANGERS_D(const Object);
    d->mutex.lock();
}

void Object::unlock() const
{
    RANGERS_D(const Object);
    d->mutex.unlock();
}
}
