/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 - 2017 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_TEXTUREDPOLYLINE_H
#define OPENSR_TEXTUREDPOLYLINE_H

#include <OpenSR/OpenSR.h>
#include <QQuickItem>
#include <QList>
#include <QVariantList>

namespace OpenSR
{
class ENGINE_API TexturedPolyline: public QQuickItem
{
    Q_OBJECT
    OPENSR_DECLARE_PRIVATE(TexturedPolyline)

    Q_PROPERTY(QVariantList points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    TexturedPolyline(QQuickItem * parent = 0);
    virtual ~TexturedPolyline();

    QVariantList points() const;
    void setPoints(const QVariantList& points);

    QUrl source() const;
    void setSource(const QUrl& source);

Q_SIGNALS:
    void pointsChanged();
    void sourceChanged();

protected:
    OPENSR_DECLARE_DPOINTER(TexturedPolyline)
    virtual QSGNode *updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData *updatePaintNodeData);
};
}

#endif // OPENSR_TEXTUREDPOLYLINE_H
