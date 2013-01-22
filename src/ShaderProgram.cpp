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
ShaderProgram::ShaderProgram(): m_linked(false), m_handle(0)
{
    m_handle = glCreateProgram();
}

void ShaderProgram::addShader(Shader& shader)
{
    if (shader.isCompiled())
    {
        if (!shader.compile())
            return;
    }

    m_linked = false;

    glAttachShader(m_handle, shader.handle());
}

bool ShaderProgram::link()
{
    if (m_linked)
        return true;
    GLint linked;
    glLinkProgram(m_handle);
    glGetProgramiv(m_handle, GL_LINK_STATUS, &linked);
    //TODO: Error log
    if (!linked)
    {
        Log::debug() << "Cannot link shader program";
        return false;
    }

    m_linked = true;
    return true;
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
