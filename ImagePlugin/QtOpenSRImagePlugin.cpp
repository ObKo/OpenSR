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

#include "QtOpenSRImagePlugin.h"

#include <stdint.h>
#include <OpenSR/libRangerQt.h>

#include "GIImageIO.h"
#include "GAIAnimationIO.h"
#include "HAIAnimationIO.h"

namespace OpenSR
{
QtOpenSRImagePlugin::QtOpenSRImagePlugin(QObject *parent) :
    QImageIOPlugin(parent)
{
}

QImageIOPlugin::Capabilities QtOpenSRImagePlugin::capabilities(QIODevice * device, const QByteArray & format) const
{
    uint32_t sig;
    if (format == "gi" || format == "gai" || format == "hai")
    {
        return QImageIOPlugin::CanRead;
    }
    if (device && device->peek((char*)&sig, 4) == 4)
    {
        if (sig == GI_FRAME_SIGNATURE || sig == GAI_SIGNATURE || sig == HAI_SIGNATURE)
            return QImageIOPlugin::CanRead;
    }
    return 0;
}

QImageIOHandler* QtOpenSRImagePlugin::create(QIODevice * device, const QByteArray & format) const
{
    QImageIOHandler* result = 0;
    if (format == "gi")
        result = new GIImageIO();
    else if (format == "gai")
        result = new GAIAnimationIO();
    else if (format == "hai")
        result = new HAIAnimationIO();

    if (result)
    {
        result->setDevice(device);
        result->setFormat(format);
    }

    return result;
}

QStringList QtOpenSRImagePlugin::keys() const
{
    return QStringList() << "gi" << "gai" << "hai";
}
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(QtOpenSRImagePlugin, OpenSR::QtOpenSRImagePlugin)
#endif // QT_VERSION < 0x050000
