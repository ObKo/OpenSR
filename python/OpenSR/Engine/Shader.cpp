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

#include <OpenSR/Shader.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportShader()
{
    using namespace boost::python;
    enum_<Shader::ShaderType>("ShaderType")
    .value("NONE", Shader::NONE)
    .value("VERTEX_SHADER", Shader::VERTEX_SHADER)
    .value("FRAGMENT_SHADER", Shader::FRAGMENT_SHADER);

    class_<Shader, boost::shared_ptr<Shader>, boost::noncopyable>("Shader", init<>())
    .def(init<Shader::ShaderType>())
    .def(init<Shader::ShaderType, const std::string&>())
    .add_property("source", &Shader::source, &Shader::setSource)
    .add_property("compiled", &Shader::isCompiled)
    .add_property("invalid", &Shader::isInvalid)
    .add_property("type", &Shader::type)
    .add_property("handle", &Shader::handle);
}
}
}
