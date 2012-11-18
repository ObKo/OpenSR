/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_GLOBAL_H
#define RANGERS_GLOBAL_H

#define RANGERS_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(m_d); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(m_d); } \
    friend class Class##Private;

#define RANGERS_D(Class) Class##Private * const d = d_func()

#define RANGERS_DECLARE_PUBLIC(Class)                  \
    inline Class* q_func() { return static_cast<Class *>(m_q); } \
    inline const Class* q_func() const { return static_cast<const Class *>(m_q); } \
    friend class Class;

#define RANGERS_Q(Class) Class * const q = q_func()

#define RANGERS_DISABLE_COPY(Class) \
    private: \
    Class(const Class& other); \
    Class &operator=(const Class& other);

#endif