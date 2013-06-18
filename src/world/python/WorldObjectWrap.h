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

#ifndef WORLD_PYTHON_OBJECTWRAP_H
#define WORLD_PYTHON_OBJECTWRAP_H

#include <OpenSR/python/Wrappers.h>

#define WORLD_PYTHON_WRAP_WORLD_OBJECT(class) \
    uint32_t type() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, type); \
    } \
    uint32_t type_() const \
    { \
        return class::type(); \
    } \
    std::list<uint64_t> dependencies() \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, dependencies); \
    } \
    std::list<uint64_t> dependencies_() \
    { \
        return class::dependencies(); \
    }

#define WORLD_PYTHON_WRAP_WORLD_OBJECT_DEF(class, wrap, object) \
    object \
    .def("type", &class::type, &wrap::type_) \
    .def("dependencies", &class::dependencies, &wrap::dependencies_);

#endif
