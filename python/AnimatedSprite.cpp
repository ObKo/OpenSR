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
    AnimatedSpriteWrap(): AnimatedSprite()
    {
    }

    AnimatedSpriteWrap(boost::shared_ptr<AnimatedTexture> texture)
        : AnimatedSprite(texture)
    {
    }
    AnimatedSpriteWrap(const std::wstring& animation)
        : AnimatedSprite(animation)
    {
    }
    RANGERS_PYTHON_WRAP_SPRITE(AnimatedSprite)
};

void exportAnimatedSprite()
{
    using namespace boost::python;

    class_<AnimatedSpriteWrap, bases<Sprite>, boost::shared_ptr<AnimatedSpriteWrap>, boost::noncopyable> c("AnimatedSprite", init<const std::wstring&>());
    c.def(init<boost::shared_ptr<AnimatedTexture> >())
    .def(init<const std::wstring&>())
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
    register_ptr_to_python<boost::shared_ptr<AnimatedSprite> >();
}
}
}