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

#include "Shader.h"
#include "Log.h"
#include <string>

namespace Rangers
{
Shader::Shader(ShaderType type, const std::string& shaderSource): m_handle(0), m_compiled(false), m_type(type)
{
    switch (type)
    {
    case VERTEX_SHADER:
        m_handle = glCreateShader(GL_VERTEX_SHADER);
        break;

    case FRAGMENT_SHADER:
        m_handle = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    setSource(shaderSource);
}

bool Shader::compile()
{
    if (m_compiled)
        return true;

    glCompileShader(m_handle);
    GLint compiled;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &compiled);
    //TODO: Error log
    if (!compiled)
    {
        Log::debug() << "Cannot compile shader";
        return false;
    }

    m_compiled = true;
    return true;
}

void Shader::setSource(const std::string& shaderSource)
{
    const char *str = shaderSource.c_str();
    glShaderSource(m_handle, 1, &str, 0);
    m_compiled = false;
}

bool Shader::isCompiled() const
{
    return m_compiled;
}

Shader::ShaderType Shader::type() const
{
    return m_type;
}

GLuint Shader::handle() const
{
    return m_handle;
}
}