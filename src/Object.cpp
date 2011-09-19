/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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

namespace Rangers
{
Object::Object(Object *parent): m_layer(0)
{
    m_rotation = 0;
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;
    m_colorA = 1.0f;
    m_needUpdate = false;
    m_parent = parent;
    m_position.x = 0;
    m_position.y = 0;

    if (parent)
        parent->addChild(this);
}

Object::Object(const Vector& pos, float rot, int layer, Object *parent): m_layer(layer)
{
    m_position = pos;
    m_rotation = rot;
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;
    m_colorA = 1.0f;
    m_needUpdate = false;

    if (parent)
        parent->addChild(this);
}

Object::Object(const Rangers::Object& other)
{
    m_colorR = other.m_colorR;
    m_colorG = other.m_colorG;
    m_colorB = other.m_colorB;
    m_colorA = other.m_colorA;
    m_needUpdate = false;

    m_parent = other.parent();

    if (m_parent)
        m_parent->addChild(this);

    //for(std::list<Object*>::const_iterator it = other.objectChilds.begin(); it < other.objectChilds.end(); it++)
    //	objectChilds.push_back(new Object(*(*it)));

    m_position = other.m_position;
    m_rotation = other.m_rotation;
}


Object::~Object()
{
    lock();
    for (std::list<Object*>::iterator i = m_children.begin(); i != m_children.end(); i++)
        (*i)->setParent(0);

    if (m_parent)
        m_parent->removeChild(this);

    if (Engine::instance())
        Engine::instance()->unmarkToUpdate(this);

    unlock();
}

Object& Object::operator=(const Rangers::Object& other)
{
    if (this == &other)
        return *this;

    //objectMutex = SDL_CreateMutex();
    m_colorR = other.m_colorR;
    m_colorG = other.m_colorG;
    m_colorB = other.m_colorB;
    m_colorA = other.m_colorA;
    m_needUpdate = false;

    m_parent = other.parent();

    if (m_parent)
        m_parent->addChild(this);

    //for(std::list<Object*>::const_iterator it = other.objectChilds.begin(); it < other.objectChilds.end(); it++)
    //	objectChilds.push_back(new Object(*(*it)));

    m_position = other.m_position;
    m_rotation = other.m_rotation;
    return *this;
}


bool Object::prepareDraw()
{
    lock();
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, 0);
    glRotatef(m_rotation, 0, 0, -1);
    glColor4f(m_colorR, m_colorG, m_colorB, m_colorA);
    return true;
}

void Object::processMain()
{
    m_needUpdate = false;
}

void Object::processLogic(int dt)
{
}

void Object::endDraw()
{
    glPopMatrix();
    unlock();
}

void Object::draw()
{

}

void Object::setColor(float r, float g, float b, float a)
{
    lock();
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_colorA = a;
    unlock();
}

void Object::setPosition(float x, float y)
{
    lock();
    m_position.x = x;
    m_position.y = y;
    unlock();
}

void Object::setPosition(const Vector& pos)
{
    lock();
    m_position = pos;
    unlock();
}

void Object::setRotation(float angle)
{
    lock();
    m_rotation = angle;
    unlock();
}

const Vector& Object::position() const
{
    return m_position;
}

float Object::rotation() const
{
    return m_rotation;
}

int Object::layer() const
{
    return m_layer;
}

Object* Object::parent() const
{
    return m_parent;
}

void Object::addChild(Object* object)
{
    lock();
    object->setParent(this);
    for (std::list<Object*>::iterator i = m_children.begin(); i != m_children.end(); i++)
    {
        if ((*i)->layer() > object->layer())
        {
            m_children.insert(i, object);
            unlock();
            return;
        }
    }
    m_children.push_back(object);
    unlock();
}

void Object::setParent(Object *parent)
{
    lock();
    m_parent = parent;
    unlock();
}

void Object::removeChild(Object* object)
{
    lock();
    object->setParent(0);
    m_children.remove(object);
    unlock();
}

void Object::setLayer(int layer)
{
    lock();
    m_layer = layer;
    if (Object *parent = m_parent)
    {
        parent->removeChild(this);
        parent->addChild(this);
    }
    unlock();
}


void Object::markToUpdate()
{
    if (!m_needUpdate)
    {
        m_needUpdate = true;
        Engine::instance()->markToUpdate(this);
    }
}

void Object::lock()
{
    m_mutex.lock();
}

void Object::unlock()
{
    m_mutex.unlock();
}
}
