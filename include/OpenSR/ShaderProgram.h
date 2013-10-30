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

#ifndef RANGERS_SHADERPROGRAM_H
#define RANGERS_SHADERPROGRAM_H

#include "OpenSR/Types.h"

#include <map>

namespace Rangers
{
class Shader;
class RANGERS_ENGINE_API ShaderProgram
{
public:
    ShaderProgram();
    virtual ~ShaderProgram();

    //! Add shader to shader program
    void addShader(boost::shared_ptr<Shader> shader);
    //! Link shader program
    bool link();

    GLint getUniformLocation(const std::string &name) const;

    bool isLinked() const;
    bool isInvalid() const;

    //! OpenGL shader program handle
    GLuint handle() const;

private:
    ShaderProgram(const ShaderProgram& other);
    ShaderProgram& operator=(const ShaderProgram& other);

    bool m_linked;
    bool m_invalid;
    GLuint m_handle;
};
}

#endif // RANGERS_SHADERPROGRAM_H
