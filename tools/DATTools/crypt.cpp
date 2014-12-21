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

#include "crypt.h"
#include <stdint.h>
#include <ctime>
#include <cstdlib>

namespace OpenSR
{
namespace DAT
{
namespace
{
const uint32_t RANGERS_KEY_RELOAD_CACHE = 0xF2FDE658;
const uint32_t RANGERS_KEY_RELOAD = 0xBE970BF1;
}

int32_t genKey()
{
    srand(time(0));
    return rand();
}

uint32_t getNextKey(int32_t key)
{
    int32_t k = (key % 0x1F31D) * 0x41A7 - ((key / 0x1F31D) * 0xB14);
    if (k < 0)
        k = k + 0x7FFFFFFF;
    return k;
}

void decrypt(QByteArray& d, int32_t key, bool isCache)
{
    int32_t k;

    uint8_t *data = (uint8_t*)d.data();

    if (!isCache)
        k = key ^ RANGERS_KEY_RELOAD;
    else
        k = key ^ RANGERS_KEY_RELOAD_CACHE;

    for (uint32_t i = 0; i < d.size(); i++)
    {
        k = getNextKey(k);
        data[i] = data[i] ^ ((k - 1) & 0xFF);
    }
}

void encrypt(QByteArray& d, int32_t key, bool isCache)
{
    int32_t k;

    uint8_t *data = (uint8_t*)d.data();

    if (!isCache)
        k = key ^ RANGERS_KEY_RELOAD;
    else
        k = key ^ RANGERS_KEY_RELOAD_CACHE;

    for (uint32_t i = 0; i < d.size(); i++)
    {
        k = getNextKey(k);
        data[i] = data[i] ^ ((k - 1) & 0xFF);
    }
}
}
}

