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
#include <OpenSR/GAISprite.h>

#include "OpenSR/python/SpriteWrap.h"

namespace Rangers
{
namespace Python
{
struct GAISpriteWrap : GAISprite, boost::python::wrapper<GAISprite>
{
    GAISpriteWrap(): GAISprite()
    {
    }

    GAISpriteWrap(const std::wstring& name)
        : GAISprite(name)
    {
    }

    RANGERS_PYTHON_WRAP_SPRITE(GAISprite)
};

void exportGAISprite()
{
    using namespace boost::python;

    class_<GAISpriteWrap, bases<AnimatedSprite>, boost::shared_ptr<GAISpriteWrap>, boost::noncopyable> c("GAISprite", init<const std::wstring&>());
    c.def(init<>())
    .def("reset", &GAISprite::reset)
    .def("setFrame", &GAISprite::setFrame);
    RANGERS_PYTHON_WRAP_SPRITE_DEF(GAISprite, GAISpriteWrap, c);
    register_ptr_to_python<boost::shared_ptr<GAISprite> >();
}
}
}