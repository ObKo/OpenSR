/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2017 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_H
#define OPENSR_H

#include <QtGlobal>

#ifdef OPENSR_ENGINE_BUILD
# ifdef Q_CC_MSVC
#  define ENGINE_API __declspec(dllexport)
# else
#  define ENGINE_API
# endif
#else
# ifdef Q_CC_MSVC
#  define ENGINE_API __declspec(dllimport)
# else
#  define ENGINE_API
# endif
#endif

#define OPENSR_DECLARE_PRIVATE(Class) \
    protected: \
        class Class##Private; \
    private: \
        inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_osr_ptr)); } \
        inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(qGetPtrHelper(d_osr_ptr)); } \
        friend class Class##Private;

#define OPENSR_DECLARE_DPOINTER(Class) \
    QScopedPointer<Class##Private> d_osr_ptr;


#endif // OPENSR_H
