/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_OBJECT_H
#define RANGERS_OBJECT_H

#include "OpenSR/global.h"
#include "OpenSR/Types.h"
#include <list>

namespace Rangers
{
class ObjectPrivate;
class RANGERS_ENGINE_API Object
{
    RANGERS_DECLARE_PRIVATE(Object)
public:
    Object();

    virtual ~Object();

    virtual void draw() const;

    //Function to process all needed OpenGL functions
    virtual void processMain();
    //Function to process all logic operations
    virtual void processLogic(int dt);

    void setPosition(float x, float y);
    void setPosition(const Vector& pos);
    void setRotation(float angle);
    void setColor(const Color& color);

    void addChild(boost::shared_ptr<Object> object);
    void removeChild(boost::shared_ptr<Object> object);

    Vector position() const;
    float rotation() const;
    int layer() const;
    Object* parent() const;
    bool needUpdate() const;
    Color color() const;
    std::list<boost::shared_ptr<Object> > children() const;

    void setLayer(int layer);

    Vector mapFromParent(const Vector& v) const;
    Vector mapFromGlobal(const Vector& v) const;
    Vector mapFromScreen(const Vector& v) const;

    Vector mapToParent(const Vector& v) const;
    Vector mapToGlobal(const Vector& v) const;
    Vector mapToScreen(const Vector& v) const;

    Rect mapToParent(const Rect& r) const;
    Rect mapToGlobal(const Rect& r) const;
    Rect mapToScreen(const Rect& r) const;

    boost::shared_ptr<Object> getChild(Object *ptr) const;

    void markToUpdate();

protected:
    Object(ObjectPrivate &p);

    ObjectPrivate *m_d;

    virtual bool prepareDraw() const;
    virtual void endDraw() const;
    void lock() const;
    void unlock() const;

    void setParent(Object *parent);

    RANGERS_DISABLE_COPY(Object)
};
}

#endif
