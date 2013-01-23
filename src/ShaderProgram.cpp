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

#include "ShaderProgram.h"
#include "Shader.h"
#include "Log.h"

namespace Rangers
{
ShaderProgram::ShaderProgram(): m_linked(false), m_handle(0), m_invalid(false)
{
}

ShaderProgram::~ShaderProgram()
{
    if (m_handle != 0)
        glDeleteProgram(m_handle);
}

void ShaderProgram::addShader(Shader& shader)
{
    if (m_handle == 0)
    {
        m_handle = glCreateProgram();
        if (m_handle == 0)
        {
            Log::error() << "Cannot create OpenGL shader program";
            m_invalid = true;
            return;
        }
    }
    if (!shader.isCompiled())
    {
        if (!shader.compile())
            return;
    }

    m_linked = false;
    m_invalid = false;

    glAttachShader(m_handle, shader.handle());
}

bool ShaderProgram::link()
{
    if (m_invalid)
        return false;
    if (m_linked)
        return true;

    if (m_handle == 0)
    {
        m_handle = glCreateProgram();
        if (m_handle == 0)
        {
            Log::error() << "Cannot create OpenGL shader program";
            m_invalid = true;
            return false;
        }
    }

    GLint linked;
    glLinkProgram(m_handle);
    glGetProgramiv(m_handle, GL_LINK_STATUS, &linked);
    //TODO: Error log
    if (linked == GL_FALSE)
    {
        int logLength;
        char *log;
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        glGetProgramInfoLog(m_handle, logLength, 0, log);
        Log::error() << "Cannot link shader program:\n" << log;
        delete[] log;

        m_invalid = true;
        return false;
    }

    m_linked = true;
    return true;
}

GLint ShaderProgram::getUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(m_handle, name.c_str());
}

bool ShaderProgram::isInvalid() const
{
    return m_invalid;
}

bool ShaderProgram::isLinked() const
{
    return m_linked;
}

GLuint ShaderProgram::handle() const
{
    return m_handle;
}
}
