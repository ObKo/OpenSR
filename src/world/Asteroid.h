/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_ASTEROID_H
#define WORLD_ASTEROID_H

#include "SystemObject.h"

namespace Rangers
{
namespace World
{
class RANGERS_WORLD_API Asteroid: public SystemObject
{
public:
    Asteroid(uint64_t id = 0);

    //! Semi-major axis
    float a() const;
    //! Semi-minor axis
    float b() const;
    float period() const;
    //! Orbit angle, in degrees.
    float angle() const;
    float time() const;
    float mineral() const;
    uint32_t style() const;
    //! Current approximate asteroid speed
    float speed() const;

    void setSemiAxis(float a, float b);
    void setPeriod(float T);
    void setAngle(float angle);
    void setTime(float time);
    void setMineral(float mineral);
    void setStyle(uint32_t style);
    void setStyle(const std::string& style);

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);

    virtual void calcTurn();
    virtual void turn(float progress);
    virtual void finishTurn();
protected:
    void calcEccentricity();
    void calcPosition();
    void calcSpeed();

    //! Semi-axis of the ellipse, a >= b
    float m_a, m_b;
    //! Orbit period, in days.
    float m_period;
    //! Orbit angle, in degrees.
    float m_angle;
    float m_t;
    float m_mineral;
    uint32_t m_style;

    float m_e;
    float m_prevT;
    float m_speed;
};
}
}

#endif