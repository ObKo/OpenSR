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

#include "Object.h"
#include "Engine.h"
#include <GL/glu.h>
#include <stack>
#include <algorithm>
#include <boost/math/constants/constants.hpp>
#define DEG2RAD(x) (x * boost::math::constants::pi<double>() / 180.0)

#include "private/Object_p.h"

namespace Rangers
{
ObjectPrivate::~ObjectPrivate()
{
}

ObjectPrivate::ObjectPrivate()
{
    m_rotation = 0;
    m_color = 0xffffffff;
    m_needUpdate = false;
    m_parent = 0;
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_layer = 0;
}

Object::Object(Object *parent): m_d(new ObjectPrivate())
{
    RANGERS_D(Object);

    if (parent)
        parent->addChild(this);
    d->m_q = this;
}

Object::Object(const Vector& pos, float rot, int layer, Object *parent): m_d(new ObjectPrivate())
{
    RANGERS_D(Object);
    d->m_layer = layer;
    d->m_position = pos;
    d->m_rotation = rot;

    if (parent)
        parent->addChild(this);
    d->m_q = this;
}

Object::Object(const Rangers::Object& other): m_d(new ObjectPrivate())
{
    RANGERS_D(Object);
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        if ((*i)->parent() == this)
            (*i)->setParent(0);

    d->m_children = other.m_d->m_children;
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        (*i)->setParent(this);

    d->m_color = other.m_d->m_color;
    d->m_needUpdate = false;

    d->m_parent = other.parent();

    if (d->m_parent)
        d->m_parent->addChild(this);

    //for(std::list<Object*>::const_iterator it = other.objectChilds.begin(); it < other.objectChilds.end(); it++)
    //	objectChilds.push_back(new Object(*(*it)));

    d->m_position = other.m_d->m_position;
    d->m_rotation = other.m_d->m_rotation;
    markToUpdate();
    d->m_q = this;
}

Object::Object(ObjectPrivate &p, const Object& other): m_d(&p)
{
    RANGERS_D(Object);
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        if ((*i)->parent() == this)
            (*i)->setParent(0);

    d->m_children = other.m_d->m_children;
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        (*i)->setParent(this);

    d->m_color = other.m_d->m_color;
    d->m_needUpdate = false;

    d->m_parent = other.parent();

    if (d->m_parent)
        d->m_parent->addChild(this);

    //for(std::list<Object*>::const_iterator it = other.objectChilds.begin(); it < other.objectChilds.end(); it++)
    //	objectChilds.push_back(new Object(*(*it)));

    d->m_position = other.m_d->m_position;
    d->m_rotation = other.m_d->m_rotation;
    markToUpdate();
    d->m_q = this;
}

Object::Object(ObjectPrivate &dd, Object* parent): m_d(&dd)
{
    RANGERS_D(Object);

    if (parent)
        parent->addChild(this);
    d->m_q = this;
}

Object::~Object()
{
    RANGERS_D(Object);
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        if ((*i)->parent() == this)
            (*i)->setParent(0);

    if (d->m_parent)
        d->m_parent->removeChild(this);

    if (Engine::instance())
        Engine::instance()->unmarkToUpdate(this);
    delete m_d;
}

Object& Object::operator=(const Rangers::Object& other)
{
    RANGERS_D(Object);
    if (this == &other)
        return *this;

    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        if ((*i)->parent() == this)
            (*i)->setParent(0);

    d->m_children = other.m_d->m_children;
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
        (*i)->setParent(this);

    d->m_color = other.m_d->m_color;
    d->m_needUpdate = false;

    d->m_parent = other.m_d->m_parent;

    if (d->m_parent)
        d->m_parent->addChild(this);

    d->m_position = other.m_d->m_position;
    d->m_rotation = other.m_d->m_rotation;
    markToUpdate();
    d->m_q = this;
    return *this;
}


bool Object::prepareDraw() const
{
    lock();
    RANGERS_D(const Object);
    glPushMatrix();
    glTranslatef(d->m_position.x, d->m_position.y, 0);
    glRotatef(d->m_rotation, 0, 0, -1);
    glColor4ub((d->m_color >> 24) & 0xff, (d->m_color >> 16) & 0xff, (d->m_color >> 8) & 0xff, d->m_color & 0xff);
    return true;
}

void Object::processMain()
{
    lock();
    RANGERS_D(Object);
    d->m_needUpdate = false;
    unlock();
}

Vector Object::mapFromParent(const Vector& v) const
{
    RANGERS_D(const Object);
    float c = cos(DEG2RAD(d->m_rotation));
    float s = -sin(DEG2RAD(d->m_rotation));
    Vector result;
    result.x = c * (v.x - d->m_position.x) + s * (v.y - d->m_position.y);
    result.y = -s * (v.x - d->m_position.x) + c * (v.y - d->m_position.y);
    return result;
}

//FIXME: Adding locks causing deadlocks.
Vector Object::mapToParent(const Vector& v) const
{
    RANGERS_D(const Object);
    float c = cos(DEG2RAD(d->m_rotation));
    float s = -sin(DEG2RAD(d->m_rotation));
    Vector result;
    result.x = c * v.x - s * v.y + d->m_position.x;
    result.y = s * v.x + c * v.y + d->m_position.y;
    return result;
}

Vector Object::mapFromGlobal(const Vector& v) const
{
    RANGERS_D(const Object);
    std::stack<const Object*> objects;
    objects.push(this);
    Object *currentParent = d->m_parent;
    while (currentParent != 0)
    {
        objects.push(currentParent);
        currentParent = currentParent->parent();
    }
    int size = objects.size();
    Vector result = v;
    for (; !objects.empty(); objects.pop())
    {
        result = objects.top()->mapFromParent(result);
    }
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
    Object *currentParent = d->m_parent;
    while (currentParent != 0)
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

void Object::setColor(float r, float g, float b, float a)
{
    lock();
    int color = 0;
    color |= (int(r * 255.0) & 0xff) << 24;
    color |= (int(g * 255.0) & 0xff) << 16;
    color |= (int(b * 255.0) & 0xff) << 8;
    color |= int(a * 255.0) & 0xff;
    setColor(color);
    unlock();
}

void Object::setColor(int color)
{
    lock();
    RANGERS_D(Object);
    d->m_color = color;
    unlock();
}

void Object::setPosition(float x, float y)
{
    lock();
    RANGERS_D(Object);
    d->m_position.x = x;
    d->m_position.y = y;
    unlock();
}

void Object::setPosition(const Vector& pos)
{
    lock();
    RANGERS_D(Object);
    d->m_position = pos;
    unlock();
}

void Object::setRotation(float angle)
{
    lock();
    RANGERS_D(Object);
    d->m_rotation = angle;
    unlock();
}

const Vector& Object::position() const
{
    RANGERS_D(const Object);
    return d->m_position;
}

float Object::rotation() const
{
    RANGERS_D(const Object);
    return d->m_rotation;
}

int Object::layer() const
{
    RANGERS_D(const Object);
    return d->m_layer;
}

Object* Object::parent() const
{
    RANGERS_D(const Object);
    return d->m_parent;
}

bool Object::needUpdate() const
{
    RANGERS_D(const Object);
    return d->m_needUpdate;
}

int Object::color() const
{
    RANGERS_D(const Object);
    return d->m_color;
}

void Object::addChild(Object* object)
{
    lock();
    RANGERS_D(Object);
    object->setParent(this);
    for (std::list<Object*>::iterator i = d->m_children.begin(); i != d->m_children.end(); i++)
    {
        if ((*i)->layer() > object->layer())
        {
            d->m_children.insert(i, object);
            unlock();
            return;
        }
    }
    d->m_children.push_back(object);
    unlock();
}

void Object::setParent(Object *parent)
{
    lock();
    RANGERS_D(Object);
    d->m_parent = parent;
    unlock();
}

void Object::removeChild(Object* object)
{
    lock();
    RANGERS_D(Object);
    object->setParent(0);
    d->m_children.remove(object);
    unlock();
}

void Object::setLayer(int layer)
{
    lock();
    RANGERS_D(Object);
    d->m_layer = layer;
    if (Object *parent = d->m_parent)
    {
        parent->removeChild(this);
        parent->addChild(this);
    }
    unlock();
}


void Object::markToUpdate()
{
    RANGERS_D(Object);
    if (!d->m_needUpdate)
    {
        d->m_needUpdate = true;
        Engine::instance()->markToUpdate(this);
    }
}

void Object::lock() const
{
    RANGERS_D(const Object);
    d->m_mutex.lock();
}

void Object::unlock() const
{
    RANGERS_D(const Object);
    d->m_mutex.unlock();
}
}
