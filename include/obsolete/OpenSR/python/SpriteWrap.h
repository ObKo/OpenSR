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

#ifndef RANGERS_PYTHON_SPRITEWRAP_H
#define RANGERS_PYTHON_SPRITEWRAP_H

#include "ObjectWrap.h"

#define RANGERS_PYTHON_WRAP_SPRITE(class) \
    RANGERS_PYTHON_WRAP_OBJECT(class) \
    virtual float normalHeight() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, normalHeight); \
    } \
    float normalHeight_() const \
    { \
        return class::normalHeight(); \
    } \
    virtual float normalWidth() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, normalWidth); \
    } \
    float normalWidth_() const \
    { \
        return class::normalWidth(); \
    } \
     
#define RANGERS_PYTHON_WRAP_SPRITE_DEF(class, wrap, object) \
    RANGERS_PYTHON_WRAP_OBJECT_DEF(class, wrap, object); \
    object \
    .def("normalHeight", &class::normalHeight, &wrap::normalHeight_) \
    .def("normalWidth", &class::normalWidth, &wrap::normalWidth_);

#endif
