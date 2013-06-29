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

#ifndef RANGERS_PYTHON_WIDGETWRAP_H
#define RANGERS_PYTHON_WIDGETWRAP_H

#include "ObjectWrap.h"

#define RANGERS_PYTHON_WRAP_WIDGET(class) \
    RANGERS_PYTHON_WRAP_OBJECT(class) \
    virtual int minWidth() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, minWidth); \
    } \
    int minWidth_() const \
    { \
        return class::minWidth(); \
    } \
    virtual int minHeight() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, minHeight); \
    } \
    int minHeight_() const \
    { \
        return class::minHeight(); \
    } \
    virtual int maxWidth() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, maxWidth); \
    } \
    int maxWidth_() const \
    { \
        return class::maxWidth(); \
    } \
    virtual int maxHeight() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, maxHeight); \
    } \
    int maxHeight_() const \
    { \
        return class::maxHeight(); \
    } \
    virtual int preferredWidth() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, preferredWidth); \
    } \
    int preferredWidth_() const \
    { \
        return class::preferredWidth(); \
    } \
    virtual int preferredHeight() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, preferredHeight); \
    } \
    int preferredHeight_() const \
    { \
        return class::preferredHeight(); \
    } \
    virtual Rect getBoundingRect() const \
    { \
        RANGERS_PYTHON_WRAP_FUNC(class, getBoundingRect); \
    } \
    Rect getBoundingRect_() const \
    { \
        return class::getBoundingRect(); \
    } \
    virtual void mouseMove(const Vector &p) \
    { \
        RANGERS_PYTHON_WRAP_FUNC_VOID(class, mouseMove, p); \
    } \
    void mouseMove_(const Vector &p) \
    { \
        class::mouseMove(p); \
    } \
    virtual void focus() \
    { \
        RANGERS_PYTHON_WRAP_FUNC_VOID(class, focus); \
    } \
    void focus_() \
    { \
        class::focus(); \
    } \
    virtual void unFocus() \
    { \
        RANGERS_PYTHON_WRAP_FUNC_VOID(class, unFocus); \
    } \
    void unFocus_() \
    { \
        class::unFocus(); \
    }

#define RANGERS_PYTHON_WRAP_WIDGET_DEF(class, wrap, object) \
    RANGERS_PYTHON_WRAP_OBJECT_DEF(class, wrap, object); \
    object \
    .def("minHeight", &class::minHeight, &wrap::minHeight_) \
    .def("minWidth", &class::minWidth, &wrap::minWidth_) \
    .def("maxHeight", &class::maxHeight, &wrap::maxHeight_) \
    .def("maxWidth", &class::maxWidth, &wrap::maxWidth_) \
    .def("preferredHeight", &class::preferredHeight, &wrap::preferredHeight_) \
    .def("preferredWidth", &class::preferredWidth, &wrap::preferredWidth_) \
    .def("focus", &class::focus, &wrap::focus_) \
    .def("unFocus", &class::unFocus, &wrap::unFocus_) \
    .def("getBoundingRect", &class::getBoundingRect, &wrap::getBoundingRect_) \
    .def("mouseMove", (void (class::*)(const Vector&))&class::mouseMove, &wrap::mouseMove_);

#endif
