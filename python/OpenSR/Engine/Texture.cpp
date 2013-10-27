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

#include <OpenSR/Texture.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportTexture()
{
    using namespace boost::python;
    enum_<TextureType>("TextureType")
    .value("TEXTURE_A8", TEXTURE_A8)
    .value("TEXTURE_R8G8B8A8", TEXTURE_R8G8B8A8)
    .value("TEXTURE_B8G8R8A8", TEXTURE_B8G8R8A8)
    .value("TEXTURE_DXT1", TEXTURE_DXT1)
    .value("TEXTURE_DXT3", TEXTURE_DXT3)
    .value("TEXTURE_DXT5", TEXTURE_DXT5)
    .value("TEXTURE_R5G6B5", TEXTURE_R5G6B5)
    .value("TEXTURE_R8G8B8", TEXTURE_R8G8B8);

    class_<Texture, boost::shared_ptr<Texture>, boost::noncopyable>("Texture", init<>())
    .def(init<int, int>())
    .def(init<int, int, TextureType, unsigned char*, int>())
    .def(init<int, int, TextureType, unsigned char*>())
    .add_property("width", &Texture::width)
    .add_property("height", &Texture::height)
    .add_property("openGLTexture", &Texture::openGLTexture)
    .def("setRawData", &Texture::setRawData);
}
}
}
