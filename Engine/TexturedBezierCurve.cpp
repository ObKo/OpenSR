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

#include "OpenSR/TexturedBezierCurve.h"

#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/QMLHelper.h>
#include <QImageReader>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

namespace OpenSR
{
class TexturedBezierCurve::TexturedBezierCurvePrivate
{
    TexturedBezierCurve *q_ptr;
    Q_DECLARE_PUBLIC(TexturedBezierCurve)

    TexturedBezierCurvePrivate(TexturedBezierCurve *q);

    BezierCurve curve;
    int minStep;
};

TexturedBezierCurve::TexturedBezierCurvePrivate::TexturedBezierCurvePrivate(TexturedBezierCurve *q)
{
    q_ptr = q;

    curve.p0 = QPointF(0.0f, 0.0f);
    curve.p1 = QPointF(0.0f, 0.0f);
    curve.p2 = QPointF(0.0f, 0.0f);
    curve.p3 = QPointF(0.0f, 0.0f);

    minStep = 1;
}

TexturedBezierCurve::TexturedBezierCurve(QQuickItem* parent): TexturedPolyline(parent),
    d_osr_ptr(new TexturedBezierCurvePrivate(this))
{
    Q_D(TexturedBezierCurve);
}

TexturedBezierCurve::~TexturedBezierCurve()
{
    Q_D(TexturedBezierCurve);
}

BezierCurve TexturedBezierCurve::curve() const
{
    Q_D(const TexturedBezierCurve);
    return d->curve;
}

void TexturedBezierCurve::setCurve(const BezierCurve& curve)
{
    Q_D(TexturedBezierCurve);

    d->curve = curve;

    QList<QPointF> points = d->curve.calcPolyline(d->minStep);
    QVariantList vpoints;

    for (const QPointF& p : points)
        vpoints.append(QVariant(p));

    setPoints(vpoints);
    emit(curveChanged());
}

int TexturedBezierCurve::minStep() const
{
    Q_D(const TexturedBezierCurve);
    return d->minStep;
}

void TexturedBezierCurve::setMinStep(int minStep)
{
    Q_D(TexturedBezierCurve);
    d->minStep = minStep;
    emit(minStepChanged());

    setCurve(d->curve);
}
}
