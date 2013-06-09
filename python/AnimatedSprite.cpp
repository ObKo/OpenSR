/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include <boost/python.hpp>
#include <OpenSR/AnimatedSprite.h>

#include "OpenSR/python/SpriteWrap.h"

namespace Rangers
{
namespace Python
{
struct AnimatedSpriteWrap : AnimatedSprite, boost::python::wrapper<AnimatedSprite>
{
    AnimatedSpriteWrap(Object *parent = 0): AnimatedSprite(parent)
    {
    }

    AnimatedSpriteWrap(boost::shared_ptr<AnimatedTexture> texture, Object *parent = 0)
        : AnimatedSprite(texture, parent)
    {
    }
    AnimatedSpriteWrap(const std::wstring& animation, Object *parent = 0)
        : AnimatedSprite(animation, parent)
    {
    }
    RANGERS_PYTHON_WRAP_SPRITE(AnimatedSprite)
};

void exportAnimatedSprite()
{
    using namespace boost::python;

    class_<AnimatedSpriteWrap, bases<Sprite>, boost::noncopyable> c("AnimatedSprite", init<const std::wstring&, Object*>());
    c.def(init<const std::wstring&>())
    .def(init<boost::shared_ptr<AnimatedTexture>, Object*>())
    .def(init<boost::shared_ptr<AnimatedTexture> >())
    .def(init<const std::wstring&, Object*>())
    .def(init<const std::wstring&>())
    .def(init<Object*>())
    .def(init<>())
    .def("setSingleShot", &AnimatedSprite::setSingleShot)
    .def("start", &AnimatedSprite::start)
    .def("stop", &AnimatedSprite::stop)
    .def("reset", &AnimatedSprite::reset)
    .def("isStarted", &AnimatedSprite::isStarted)
    .def("isSingleShot", &AnimatedSprite::isSingleShot)
    .def("currentFrame", &AnimatedSprite::currentFrame)
    .def("frameRate", &AnimatedSprite::frameRate)
    .def("setFrame", &AnimatedSprite::setFrame)
    .def("setFrameRate", &AnimatedSprite::setFrameRate);
    RANGERS_PYTHON_WRAP_SPRITE_DEF(AnimatedSprite, AnimatedSpriteWrap, c);
}
}
}