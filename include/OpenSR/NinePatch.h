/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Sprite.h"

namespace Rangers
{
class NinePatchPrivate;
class RANGERS_ENGINE_API NinePatch: public Sprite
{
    RANGERS_DECLARE_PRIVATE(NinePatch)
public:
    NinePatch(Object *parent = 0);
    NinePatch(const NinePatchDescriptor &desc, Object *parent = 0);
    NinePatch(const std::wstring& name, Object *parent = 0);

    virtual void processMain();
    virtual void draw() const;

    virtual float normalWidth() const;
    virtual float normalHeight() const;

protected:
    NinePatch(NinePatchPrivate &p, Object *parent = 0);

    RANGERS_DISABLE_COPY(NinePatch)
};
}

#endif /* RANGER_NINEPATCH_H */
