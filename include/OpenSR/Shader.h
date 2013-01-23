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

#ifndef RANGERS_SHADER_H
#define RANGERS_SHADER_H

#include "Types.h"

namespace Rangers
{
class Shader
{
public:
    enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER};

    Shader(ShaderType type);
    //! Create shader from source
    Shader(ShaderType type, const std::string& shaderSource);
    virtual ~Shader();

    //! Set shader source code
    void setSource(const std::string& shaderSource);
    //! Try to compile shader
    bool compile();

    //! Returns true if shader was compiled.
    bool isCompiled() const;
    //! Returns true if shader cannot be compiled.
    bool isInvalid() const;
    //! Shader type
    ShaderType type() const;
    //! OpenGL shader handle
    GLuint handle() const;

private:
    GLuint m_handle;
    std::string m_source;
    bool m_invalid;
    bool m_compiled;
    ShaderType m_type;
};
}

#endif // RANGERS_SHADER_H
