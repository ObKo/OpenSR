/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2017 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_PLANETDRAWER_H
#define OPENSR_PLANETDRAWER_H

#include <OpenSR/OpenSR.h>
#include <QQuickFramebufferObject>
#include <QList>
#include <QVariantList>

namespace OpenSR
{
class ENGINE_API PlanetDrawer: public QQuickFramebufferObject
{
    Q_OBJECT
    OPENSR_DECLARE_PRIVATE(PlanetDrawer)

    Q_PROPERTY(QUrl surface READ surface WRITE setSurface NOTIFY surfaceChanged)
    Q_PROPERTY(float phase READ phase WRITE setPhase NOTIFY phaseChanged)
    Q_PROPERTY(QColor atmosphere READ atmosphere WRITE setAtmosphere NOTIFY atmosphereChanged)
    Q_PROPERTY(QUrl cloud0 READ cloud0 WRITE setCloud0 NOTIFY cloud0Changed)
    Q_PROPERTY(QUrl cloud1 READ cloud1 WRITE setCloud1 NOTIFY cloud1Changed)
    Q_PROPERTY(float cloud0Phase READ cloud0Phase WRITE setCloud0Phase NOTIFY cloud0PhaseChanged)
    Q_PROPERTY(float cloud1Phase READ cloud1Phase WRITE setCloud1Phase NOTIFY cloud1PhaseChanged)
    Q_PROPERTY(float lightAngle READ lightAngle WRITE setLightAngle NOTIFY lightAngleChanged)

public:
    PlanetDrawer(QQuickItem * parent = 0);
    virtual ~PlanetDrawer();

    virtual Renderer *createRenderer() const;

    QUrl surface() const;
    void setSurface(const QUrl& surface);

    QColor atmosphere() const;
    void setAtmosphere(const QColor& atmosphere);

    float phase() const;
    void setPhase(float phase);

    QUrl cloud0() const;
    void setCloud0(const QUrl& clouds);

    QUrl cloud1() const;
    void setCloud1(const QUrl& clouds);

    float cloud0Phase() const;
    void setCloud0Phase(float phase);

    float cloud1Phase() const;
    void setCloud1Phase(float phase);

    float lightAngle() const;
    void setLightAngle(float angle);

Q_SIGNALS:
    void surfaceChanged();
    void phaseChanged();
    void atmosphereChanged();
    void cloud0Changed();
    void cloud1Changed();
    void cloud0PhaseChanged();
    void cloud1PhaseChanged();
    void lightAngleChanged();

protected:
    OPENSR_DECLARE_DPOINTER(PlanetDrawer)
};
}

#endif // OPENSR_PLANETDRAWER_H
