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

#include <OpenSR/libRangerQt.h>

#include <QByteArray>
#include <QtEndian>
#include <cstdlib>

namespace OpenSR
{
/*!
 * \param src zlib compressed data
 *   \note: This function has to convert second DWORD to big endian
 * \return unpacked data
 */
QByteArray unpackZL(QByteArray &src)
{
    uint32_t sig = ((uint32_t *)src.constData())[0];
    uint32_t outsize = ((uint32_t *)src.constData())[1];

    if (sig != ZL02_SIGNATURE && sig != ZL01_SIGNATURE)
        return QByteArray();

    ((uint32_t *)src.data())[1] = qToBigEndian(outsize);

    return qUncompress((const uchar*)(src.constData() + 4), src.size() - 4);
}

QByteArray packZL01(const QByteArray &src)
{
    //TODO: Optimize
    QByteArray compressed = qCompress(src);
    QByteArray sigArr(4, 0);

    *((uint32_t *)sigArr.data()) = ZL01_SIGNATURE;
    *((uint32_t *)compressed.data()) = qToLittleEndian((uint32_t)src.size());
    compressed.prepend(sigArr);
    return compressed;
}

QByteArray packZL02(const QByteArray &src)
{
    //TODO: Optimize
    QByteArray compressed = qCompress(src);
    QByteArray sigArr(4, 0);

    *((uint32_t *)sigArr.data()) = ZL02_SIGNATURE;
    *((uint32_t *)compressed.data()) = qToLittleEndian((uint32_t)src.size());
    compressed.prepend(sigArr);
    return compressed;
}
}
