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

#include "OpenSR/TexturedPolyline.h"

#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <QImageReader>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

#include <cmath>

namespace OpenSR
{
namespace
{
class TexturedPolylineNode : public QSGGeometryNode
{
public:
    QSGTextureMaterial material;
    QSGTexture *texture;

    TexturedPolylineNode(): texture(0)
    {
        setFlag(OwnsGeometry);
        material.setHorizontalWrapMode(QSGTexture::Repeat);
        material.setFiltering(QSGTexture::Linear);
        setMaterial(&material);
    }

    virtual ~TexturedPolylineNode()
    {
        if (texture)
            delete texture;
    }

    void setPolyline(const QList<QPointF>& points)
    {
        if (points.count() < 2)
        {
            setGeometry(new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0));
            return;
        }

        float width = 1.0f, stride = 1.0f;

        if (material.texture())
        {
            width = material.texture()->textureSize().height();
            stride = material.texture()->textureSize().width();
        }

        if (width < 1.0f)
            width = 1.0f;

        if (stride < 1.0f)
            stride = 1.0f;

        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), (points.count() - 1) * 4, (points.count() - 1) * 6);
        geometry->setDrawingMode(GL_TRIANGLES);

        QSGGeometry::TexturedPoint2D *p = geometry->vertexDataAsTexturedPoint2D();
        quint16 *idx = geometry->indexDataAsUShort();

        quint16 currentIndex = 0;
        double tdelta = 0.0;

        QPointF prev = points.first();

        auto end = points.end();
        for (auto i = points.begin() + 1; i != end; ++i)
        {
            bool last = (i + 1) == end;
            bool first = i == (points.begin() + 1);
            QVector2D d(i->x() - prev.x(), i->y() - prev.y());
            QVector2D n = QVector2D(-d.y(), d.x()).normalized();

            if (first)
            {
                p[0].set(prev.x() - n.x() * width / 2.0f, prev.y() - n.y() * width / 2.0f, 0.0, 0.0);
                p[1].set(prev.x() + n.x() * width / 2.0f, prev.y() + n.y() * width / 2.0f, 0.0, 1.0);
                p += 2;
            }

            tdelta += d.length() / stride;

            if (last)
            {
                p[0].set(i->x() - n.x() * width / 2.0f, i->y() - n.y() * width / 2.0f, tdelta, 0.0);
                p[1].set(i->x() + n.x() * width / 2.0f, i->y() + n.y() * width / 2.0f, tdelta, 1.0);
            }
            else
            {
                QPointF next = *(i + 1);

                QVector2D d1 = QVector2D(next - *i).normalized();
                QVector2D d2 = QVector2D(*i - prev).normalized();
                bool greaterPi = (d1.x() * d2.y() - d2.x() * d1.y()) < 0;
                QVector2D tangent = (d1 + d2).normalized();
                QVector2D miter = QVector2D(-tangent.y(), tangent.x());

                double cosa = QVector2D::dotProduct(miter, n);
                double tdx = tan(acos(cosa)) / 2.0 * width / stride;
                miter *= width / 2.0f / cosa;

                if (greaterPi)
                    tdx = -tdx;

                p[0].set(i->x() - miter.x(), i->y() - miter.y(), tdelta - tdx, 0.0);
                p[1].set(i->x() + miter.x(), i->y() + miter.y(), tdelta + tdx, 1.0);

                // Start of next segment
                p[2].set(i->x() - miter.x(), i->y() - miter.y(), tdelta + tdx, 0.0);
                p[3].set(i->x() + miter.x(), i->y() + miter.y(), tdelta - tdx, 1.0);
                p += 4;
            }
            *(idx++) = currentIndex;
            *(idx++) = currentIndex + 1;
            *(idx++) = currentIndex + 2;
            *(idx++) = currentIndex + 1;
            *(idx++) = currentIndex + 3;
            *(idx++) = currentIndex + 2;
            currentIndex += 4;

            prev = *i;
        }
        setGeometry(geometry);
    }

    void setTexture(QSGTexture *tex)
    {
        if (tex == texture)
            return;

        if (texture)
            delete texture;
        texture = tex;
        material.setTexture(texture);
    }
};
}

class TexturedPolyline::TexturedPolylinePrivate
{
    TexturedPolyline *q_ptr;
    Q_DECLARE_PUBLIC(TexturedPolyline)

    TexturedPolylinePrivate(TexturedPolyline *q);

    QList<QPointF> points;
    bool pointsChanged;
    QUrl source;
    bool sourceChanged;
};

TexturedPolyline::TexturedPolylinePrivate::TexturedPolylinePrivate(TexturedPolyline *q)
{
    q_ptr = q;
}

TexturedPolyline::TexturedPolyline(QQuickItem* parent): QQuickItem(parent),
    d_osr_ptr(new TexturedPolylinePrivate(this))
{
    Q_D(TexturedPolyline);
    setFlag(QQuickItem::ItemHasContents);
    d->pointsChanged = true;
    d->sourceChanged = true;
}

TexturedPolyline::~TexturedPolyline()
{
    Q_D(TexturedPolyline);
}

QSGNode* TexturedPolyline::updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* updatePaintNodeData)
{
    Q_D(TexturedPolyline);

    TexturedPolylineNode *n = dynamic_cast<TexturedPolylineNode*>(oldNode);

    if (!window())
        return oldNode;

    if (d->sourceChanged)
    {
        d->sourceChanged = false;

        QIODevice *dev = qobject_cast<Engine*>(qApp)->resources()->getIODevice(d->source);

        if (!dev)
        {
            if (oldNode)
                delete oldNode;
            return 0;
        }

        QImageReader r(dev);
        QImage img = r.read();
        dev->close();
        delete dev;

        if (img.isNull())
        {
            if (oldNode)
                delete oldNode;
            return 0;
        }

        QSGTexture *tex = window()->createTextureFromImage(img, QQuickWindow::CreateTextureOptions(QQuickWindow::TextureHasAlphaChannel) |
                          QQuickWindow::CreateTextureOptions(QQuickWindow::TextureOwnsGLTexture));

        if (!tex)
        {
            if (oldNode)
                delete oldNode;
            return 0;
        }

        if (!n)
            n = new TexturedPolylineNode();

        n->setTexture(tex);
        n->setPolyline(d->points);

        d->pointsChanged = false;
        n->markDirty(QSGNode::DirtyMaterial | QSGNode::DirtyGeometry);
    }

    if (!n)
        return n;

    if (d->pointsChanged)
    {
        d->pointsChanged = false;
        n->setPolyline(d->points);
        n->markDirty(QSGNode::DirtyGeometry);
    }

    return n;
}

QVariantList TexturedPolyline::points() const
{
    Q_D(const TexturedPolyline);
    QVariantList res;
    for (const QPointF& p : d->points)
        res.append(p);
    return res;
}

void TexturedPolyline::setPoints(const QVariantList& points)
{
    Q_D(TexturedPolyline);

    d->points.clear();
    for (const QVariant& v : points)
        d->points.append(v.toPointF());

    emit(pointsChanged());
    d->pointsChanged = true;
    update();
}

void TexturedPolyline::setSource(const QUrl& source)
{
    Q_D(TexturedPolyline);
    d->source = source;
    d->sourceChanged = true;
    emit(sourceChanged());
}

QUrl TexturedPolyline::source() const
{
    Q_D(const TexturedPolyline);
    return d->source;
}
}
