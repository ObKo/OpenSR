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
Shader::Shader(): m_handle(0), m_compiled(false), m_type(NONE), m_invalid(true)
{
}

Shader::Shader(ShaderType type): m_handle(0), m_compiled(false), m_type(type), m_invalid(true)
{
}

Shader::Shader(ShaderType type, const std::string& shaderSource): m_handle(0), m_compiled(false),
    m_type(type), m_invalid(false), m_source(shaderSource)
{
}

Shader::~Shader()
{
    if (m_handle != 0)
        glDeleteShader(m_handle);
}

bool Shader::compile()
{
    if (m_type == NONE)
        return false;
    if (m_invalid)
        return false;
    if (m_compiled)
        return true;

    if (m_handle == 0)
    {
        switch (m_type)
        {
        case VERTEX_SHADER:
            m_handle = glCreateShader(GL_VERTEX_SHADER);
            break;

        case FRAGMENT_SHADER:
            m_handle = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
        if (m_handle == 0)
        {
            Log::error() << "Cannot create OpenGL shader";
            m_invalid = true;
            return false;
        }
    }
    const char *str = m_source.c_str();
    glShaderSource(m_handle, 1, &str, 0);

    glCompileShader(m_handle);
    GLint compiled;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &compiled);
    //TODO: Error log
    if (compiled == GL_FALSE)
    {
        int logLength;
        char *log;
        glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        glGetShaderInfoLog(m_handle, logLength, 0, log);
        Log::error() << "Cannot compile shader:\n" << log;
        delete[] log;

        m_invalid = true;
        return false;
    }

    m_compiled = true;
    return true;
}

void Shader::setSource(const std::string& shaderSource)
{
    if (m_type == NONE)
        return;
    const char *str = shaderSource.c_str();
    glShaderSource(m_handle, 1, &str, 0);
    m_compiled = false;
    m_invalid = false;
}

bool Shader::isCompiled() const
{
    return m_compiled;
}

bool Shader::isInvalid() const
{
    return m_invalid;
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