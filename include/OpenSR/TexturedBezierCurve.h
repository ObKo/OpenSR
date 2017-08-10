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

#ifndef OPENSR_TEXTUREDBEZIERCURVE_H
#define OPENSR_TEXTUREDBEZIERCURVE_H

#include <OpenSR/OpenSR.h>
#include <OpenSR/Types.h>
#include <OpenSR/TexturedPolyline.h>

namespace OpenSR
{
class ENGINE_API TexturedBezierCurve: public TexturedPolyline
{
    Q_OBJECT
    OPENSR_DECLARE_PRIVATE(TexturedBezierCurve)

    Q_PROPERTY(OpenSR::BezierCurve curve READ curve WRITE setCurve NOTIFY curveChanged)
    Q_PROPERTY(int minStep READ minStep WRITE setMinStep NOTIFY minStepChanged)

public:
    TexturedBezierCurve(QQuickItem * parent = 0);
    virtual ~TexturedBezierCurve();

    BezierCurve curve() const;
    int minStep() const;

    void setCurve(const BezierCurve& curve);
    void setMinStep(int minStep);

Q_SIGNALS:
    void curveChanged();
    void minStepChanged();

protected:
    OPENSR_DECLARE_DPOINTER(TexturedBezierCurve)
};
}

#endif // OPENSR_TEXTUREDBEZIERCURVE_H
