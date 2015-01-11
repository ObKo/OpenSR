/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_DAT_CRYPT_H
#define OPENSR_DAT_CRYPT_H

#include <cstdint>
#include <QByteArray>

namespace OpenSR
{
namespace DAT
{
int32_t genKey();
void decrypt(QByteArray& data, int32_t key, bool isCache = false);
void encrypt(QByteArray& data, int32_t key, bool isCache = false);
quint32 crc32(const QByteArray &data);
}
}

#endif
