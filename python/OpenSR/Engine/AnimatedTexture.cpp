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

#include <OpenSR/AnimatedTexture.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportAnimatedTexture()
{
    using namespace boost::python;

    //TODO: Do we need constructors?
    class_<AnimatedTexture, bases<Texture>, boost::shared_ptr<AnimatedTexture>, boost::noncopyable>("AnimatedTexture", init<>())
    .add_property("waitSeek", &AnimatedTexture::waitSeek)
    .add_property("waitSize", &AnimatedTexture::waitSize)
    .add_property("frameCount", &AnimatedTexture::frameCount)
    .def("openGLTexture", &AnimatedTexture::openGLTexture);
}
}
}
