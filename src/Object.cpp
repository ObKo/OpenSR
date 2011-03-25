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

using namespace Rangers;

Object::Object(Object *parent): objectLayer(0)
{
    objRotation = 0;
    objectMutex = SDL_CreateMutex();
    red = 1.0f;
    green = 1.0f;
    blue = 1.0f;
    alpha = 1.0f;
    marked = false;
    objectParent = parent;
    objPosition.x = 0;
    objPosition.y = 0;

    if (parent)
        parent->addChild(this);
}

Object::Object(const Vector& pos, float rot, int layer, Object *parent): objectLayer(layer)
{
    objPosition = pos;
    objRotation = rot;
    objectMutex = SDL_CreateMutex();
    red = 1.0f;
    green = 1.0f;
    blue = 1.0f;
    alpha = 1.0f;
    marked = false;

    if (parent)
        parent->addChild(this);
}

Object::Object(const Rangers::Object& other)
{
    objectMutex = SDL_CreateMutex();
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
    marked = false;
    
    objectParent = other.parent();
    
    if (objectParent)
        objectParent->addChild(this);
    
    //for(std::list<Object*>::const_iterator it = other.objectChilds.begin(); it < other.objectChilds.end(); it++)
    //	objectChilds.push_back(new Object(*(*it)));
	
    objPosition = other.objPosition;
    objRotation = other.objRotation;
}


Object::~Object()
{
    lock();
    for (std::list<Object*>::iterator i = objectChilds.begin(); i != objectChilds.end(); i++)
        (*i)->setParent(0);

    if (objectParent)
        objectParent->removeChild(this);
    
    if(Engine::instance())    
	Engine::instance()->unmarkToUpdate(this);

    unlock();
    SDL_DestroyMutex(objectMutex);
}

Object& Object::operator=(const Rangers::Object& other)
{
    if (this == &other)
        return *this;
    
    //objectMutex = SDL_CreateMutex();
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
    marked = false;
    
    objectParent = other.parent();
    
    if (objectParent)
        objectParent->addChild(this);
    
    //for(std::list<Object*>::const_iterator it = other.objectChilds.begin(); it < other.objectChilds.end(); it++)
    //	objectChilds.push_back(new Object(*(*it)));
	
    objPosition = other.objPosition;
    objRotation = other.objRotation;
    return *this;
}


void Object::prepareDraw() const
{
    glPushMatrix();
    glTranslatef(objPosition.x, objPosition.y, 0);
    glRotatef(objRotation, 0, 0, -1);
    glColor4f(red, green, blue, alpha);
}

void Object::processMain()
{
    marked = false;
}

void Object::processLogic(int dt)
{
}

void Object::endDraw() const
{
    glPopMatrix();
}

void Object::draw() const
{

}

void Object::setColor(float r, float g, float b, float a)
{
    lock();
    red = r;
    green = g;
    blue = b;
    alpha = a;
    unlock();
}

void Object::setPosition(float x, float y)
{
    lock();
    objPosition.x = x;
    objPosition.y = y;
    unlock();
}

void Object::setPosition(const Vector& pos)
{
    lock();
    objPosition = pos;
    unlock();
}

void Object::setRotation(float angle)
{
    lock();
    objRotation = angle;
    unlock();
}

const Vector& Object::position() const
{
    return objPosition;
}

float Object::rotation() const
{
    return objRotation;
}

int Object::layer() const
{
    return objectLayer;
}

SDL_mutex* Object::mutex() const
{
    return objectMutex;
}

Object* Object::parent() const
{
    return objectParent;
}

void Object::addChild(Object* object)
{
    lock();
    object->setParent(this);
    for (std::list<Object*>::iterator i = objectChilds.begin(); i != objectChilds.end(); i++)
    {
        if ((*i)->layer() > object->layer())
        {
            objectChilds.insert(i, object);
            SDL_mutexV(objectMutex);
            return;
        }
    }
    objectChilds.push_back(object);
    unlock();
}

void Object::setParent(Object *parent)
{
    objectParent = parent;
}

void Object::removeChild(Object* object)
{
    lock();
    object->setParent(0);
    objectChilds.remove(object);
    unlock();
}

void Object::setLayer(int layer)
{
    lock();
    objectLayer = layer;
    if (objectParent)
    {
        objectParent->removeChild(this);
        objectParent->addChild(this);
    }
    unlock();
}


void Object::markToUpdate()
{
    if (!marked)
    {
        marked = true;
        Engine::instance()->markToUpdate(this);
    }
}

void Object::lock() const
{
    SDL_mutexP(objectMutex);
}

void Object::unlock() const
{
    SDL_mutexV(objectMutex);
}

