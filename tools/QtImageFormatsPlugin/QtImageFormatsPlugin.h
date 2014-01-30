#ifndef RANGERS_QT_IMAGE_FORMATS_PLUGIN_H
#define RANGERS_QT_IMAGE_FORMATS_PLUGIN_H

#include <QImageIOPlugin>

namespace Rangers
{
class QtImageFormatsPlugin : public QImageIOPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "qopensr.json")
#endif // QT_VERSION >= 0x050000

public:
    QtImageFormatsPlugin(QObject *parent = 0);

    virtual Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    virtual QImageIOHandler* create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    virtual QStringList keys() const;
};
}

#endif // RANGERS_QT_IMAGE_FORMATS_PLUGIN_H
