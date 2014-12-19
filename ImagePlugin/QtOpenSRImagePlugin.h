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

#ifndef OPENSR_QT_OPENSR_IMAGE_PLUGIN_H
#define OPENSR_QT_OPENSR_IMAGE_PLUGIN_H

#include <QImageIOPlugin>

namespace OpenSR
{
class QtOpenSRImagePlugin : public QImageIOPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "QtOpenSRImagePlugin.json")
#endif // QT_VERSION >= 0x050000

public:
    QtOpenSRImagePlugin(QObject *parent = 0);

    virtual Capabilities capabilities(QIODevice * device, const QByteArray & format) const;
    virtual QImageIOHandler* create(QIODevice * device, const QByteArray & format = QByteArray()) const;
    virtual QStringList keys() const;
};
}

#endif // OPENSR_QT_OPENSR_IMAGE_PLUGIN_H
