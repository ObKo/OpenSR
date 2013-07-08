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

#ifndef RANGERS_TILED_LINE_H
#define RANGERS_TILED_LINE_H

#include "OpenSR/Object.h"

namespace Rangers
{
class Texture;

class TiledLinePrivate;
class RANGERS_ENGINE_API TiledLine: public Object
{
    RANGERS_DECLARE_PRIVATE(TiledLine)
public:
    TiledLine(Object *parent = 0);
    TiledLine(boost::shared_ptr<Texture> texture, Object *parent = 0);
    TiledLine(boost::shared_ptr<Texture> texture, const Vector& start, const Vector& end, Object *parent = 0);
    TiledLine(const std::wstring& texture, Object *parent = 0);
    TiledLine(const std::wstring& texture, const Vector& start, const Vector& end, Object *parent = 0);

    virtual ~TiledLine();

    virtual void draw() const;
    virtual void processMain();

    Vector start() const;
    Vector end() const;
    boost::shared_ptr<Texture> texture() const;

    void setLine(const Vector& start, const Vector& end);
    void setTexture(boost::shared_ptr<Texture> texture);

protected:
    TiledLine(TiledLinePrivate &p, Object *parent = 0);

    RANGERS_DISABLE_COPY(TiledLine)
};
}

#endif
