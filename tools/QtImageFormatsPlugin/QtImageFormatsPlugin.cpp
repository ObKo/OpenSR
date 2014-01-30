#include "QtImageFormatsPlugin.h"

#include <stdint.h>

#include "GIImageIO.h"
#include "GAIAnimationIO.h"
#include "HAIAnimationIO.h"

namespace Rangers
{
QtImageFormatsPlugin::QtImageFormatsPlugin(QObject *parent) :
    QImageIOPlugin(parent)
{
}

QImageIOPlugin::Capabilities QtImageFormatsPlugin::capabilities(QIODevice * device, const QByteArray & format) const
{
    uint32_t sig;
    if (format == "gi" || format == "gai" || format == "hai")
    {
        return QImageIOPlugin::CanRead;
    }
    if (device && device->peek((char*)&sig, 4) == 4)
    {
        if (sig == 0x00006967 || sig == 0x00696167 || sig == 0x04210420)
            return QImageIOPlugin::CanRead;
    }
    return 0;
}

QImageIOHandler* QtImageFormatsPlugin::create(QIODevice * device, const QByteArray & format) const
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

QStringList QtImageFormatsPlugin::keys() const
{
    return QStringList() << "gi" << "gai" << "hai";
}
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qopensr, Rangers::QtImageFormatsPlugin)
#endif // QT_VERSION < 0x050000
