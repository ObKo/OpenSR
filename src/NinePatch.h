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

#ifndef RANGERS_NINEPATCH_H
#define RANGERS_NINEPATCH_H

#include "Sprite.h"
#include <vector>

namespace Rangers
{
class NinePatch: public Sprite
{
public:
    //TODO: Move this to global
    struct TextureRegion
    {
        float u1, v1;
        float u2, v2;
    };

    NinePatch(Object *parent = 0);
    NinePatch(const std::wstring& resourceName, Object *parent = 0);
    NinePatch(const std::wstring& resourceName, const std::vector<float> &rows, const std::vector<float> &columns, Object *parent = 0);

    virtual void processMain();
    virtual void draw() const;

private:
    std::vector<float> m_rows;
    std::vector<float> m_columns;
    bool m_embedded;
};
}

#endif /* RANGER_NINEPATCH_H */
