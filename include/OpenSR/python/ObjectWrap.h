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

#ifndef RANGERS_PYTHON_OBJECTWRAP_H
#define RANGERS_PYTHON_OBJECTWRAP_H

#include "Wrappers.h"

#define RANGERS_PYTHON_WRAP_OBJECT(class) \
    virtual void draw() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC_VOID(class, draw); \
    } \
    void draw_() const \
    { \
        class::draw(); \
    } \
    virtual void processMain() \
    { \
        RANGERS_PYTHON_WRAP_FUNC_VOID(class, processMain); \
    } \
    void processMain_() \
    { \
        class::processMain(); \
    } \
    virtual void processLogic(int dt) \
    { \
        RANGERS_PYTHON_WRAP_FUNC_VOID(class, processLogic, dt); \
    } \
    void processLogic_(int dt) \
    { \
        class::processLogic(dt); \
    }

#define RANGERS_PYTHON_WRAP_OBJECT_DEF(class, wrap, object) \
    object \
    .def("draw", &class::draw, &wrap::draw_) \
    .def("processMain", &class::processMain, &wrap::processMain_) \
    .def("processLogic", &class::processLogic, &wrap::processLogic_)

#endif
