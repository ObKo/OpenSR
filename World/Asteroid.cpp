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

#include "Asteroid.h"
#include "WorldHelper.h"
#include <OpenSR/libRanger.h>
#include <OpenSR/Log.h>
#include <cmath>

namespace
{
const int ITERATION_COUNTS = 10;
}

namespace Rangers
{
namespace World
{
Asteroid::Asteroid(uint64_t id): SystemObject(id),
    m_a(0), m_b(0), m_period(0), m_angle(0), m_t(0), m_style(0), m_mineral(0),
    m_speed(0)
{
}

bool Asteroid::deserialize(std::istream& stream)
{
    if (!SystemObject::deserialize(stream))
        return false;

    stream.read((char *)&m_style, sizeof(uint32_t));
    stream.read((char *)&m_a, sizeof(float));
    stream.read((char *)&m_b, sizeof(float));
    stream.read((char *)&m_period, sizeof(float));
    stream.read((char *)&m_angle, sizeof(float));
    stream.read((char *)&m_t, sizeof(float));
    stream.read((char *)&m_mineral, sizeof(float));

    if (!stream.good())
        return false;

    calcEccentricity();
    calcPosition();

    return true;
}

float Asteroid::mineral() const
{
    return m_mineral;
}

bool Asteroid::serialize(std::ostream& stream) const
{
    if (!SystemObject::serialize(stream))
        return false;

    stream.write((char *)&m_style, sizeof(uint32_t));
    stream.write((char *)&m_a, sizeof(float));
    stream.write((char *)&m_b, sizeof(float));
    stream.write((char *)&m_period, sizeof(float));
    stream.write((char *)&m_angle, sizeof(float));
    stream.write((char *)&m_t, sizeof(float));
    stream.write((char *)&m_mineral, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}

void Asteroid::setMineral(float mineral)
{
    m_mineral = mineral;
}

float Asteroid::a() const
{
    return m_a;
}

float Asteroid::b() const
{
    return m_b;
}

float Asteroid::period() const
{
    return m_period;
}

float Asteroid::angle() const
{
    return m_angle;
}

float Asteroid::time() const
{
    return m_t;
}

void Asteroid::setSemiAxis(float a, float b)
{
    if (a < b)
    {
        Log::warning() << "Asteroid: semi-major axis is less than semi-minor, axis will be swapped.";
        m_a = b;
        m_b = a;
    }
    else
    {
        m_a = a;
        m_b = b;
    }
    calcEccentricity();
    calcPosition();
    calcSpeed();
    calcTrajectory();
}

void Asteroid::setPeriod(float T)
{
    m_period = T;
    calcPosition();
    calcSpeed();
    calcTrajectory();
}

void Asteroid::setAngle(float angle)
{
    m_angle = angle;
    calcPosition();
    calcSpeed();
    calcTrajectory();
}

void Asteroid::setTime(float time)
{
    m_t = time;

    if (fabs(m_t) > m_period)
        m_t = fmod(m_t, m_period);

    if (m_t < 0)
        m_t = m_period + m_t;

    calcPosition();
}

void Asteroid::calcEccentricity()
{
    m_e = sqrt(1 - (m_b * m_b) / (m_a * m_a));
}

void Asteroid::calcPosition()
{
    m_position = E(solveKepler(m_t));
}

void Asteroid::calcSpeed()
{
    Point nextPos = E(solveKepler(m_t + 1));
    float dx = nextPos.x - m_position.x;
    float dy = nextPos.y - m_position.y;
    m_speed = sqrt(dx * dx + dy * dy);
}

float Asteroid::solveKepler(float t)
{
    // http://en.wikipedia.org/wiki/Kepler's_equation
    float M = 2.0f * M_PI / m_period * t;
    float E = M;
    for (int j = 0; j < ITERATION_COUNTS; j++)
    {
        E = m_e * sin(E) + M;
    }
    return E;
}

Point Asteroid::E(float eta)
{
    Point p;
    float angle = m_angle / 180.0f * M_PI;
    p.x = - (m_a * m_e) * cos(angle) + m_a * cos(angle) * cos(eta) - m_b * sin(angle) * sin(eta);
    p.y = - (m_a * m_e) * sin(angle) + m_a * sin(angle) * cos(eta) + m_b * cos(angle) * sin(eta);
    return p;
}

Point Asteroid::Ederiv(float eta)
{
    Point p;
    float angle = m_angle / 180.0f * M_PI;
    p.x = - m_a * cos(angle) * sin(eta) - m_b * sin(angle) * cos(eta);
    p.y = - m_a * sin(angle) * sin(eta) + m_b * cos(angle) * cos(eta);
    return p;
}

void Asteroid::calcTrajectory()
{
    m_trajectory.nextTurns.clear();

    float prev = solveKepler(m_t);

    for (int i = 1; i < int(round(m_period)); i++)
    {
        float t = m_t + i;
        if (fabs(t) > m_period)
            t = fmod(t, m_period);

        if (t < 0)
            t = m_period + t;

        float eta = solveKepler(t);

        Vector p1, p2, p3, p4;
        Point p;

        // http://www.spaceroots.org/documents/ellipse/elliptical-arc.pdf
        p = E(prev);
        p1 = Vector(p.x, p.y);
        p = E(eta);
        p4 = Vector(p.x, p.y);

        float tangent = tan((eta - prev) / 2.0f);
        float k = sin(eta - prev) * (sqrt(4.0f + 3.0f * tangent * tangent) - 1.0f) / 3.0f;

        p = Ederiv(prev);
        p2 = p1 + k * Vector(p.x, p.y);
        p = Ederiv(eta);
        p3 = p4 - k * Vector(p.x, p.y);

        BeizerCurve c = BeizerCurve(p1, p2, p3, p4);

        m_trajectory.nextTurns.push_back(c);

        prev = eta;
    }
}

float Asteroid::speed() const
{
    return m_speed;
}

void Asteroid::calcTurn()
{
    m_prevT = m_t;
}

void Asteroid::finishTurn()
{
    setTime(m_prevT + 1.0f);
    float E = solveKepler(m_t);
    Log::debug() << "eta: " << (float)(E * 180 / M_PI);
    calcTrajectory();
    calcSpeed();
}

void Asteroid::turn(float progress)
{
    setTime(m_prevT + progress);
}

uint32_t Asteroid::style() const
{
    return m_style;
}

void Asteroid::setStyle(uint32_t style)
{
    m_style = style;
}

void Asteroid::setStyle(const std::string& style)
{
    setStyle(textHash32(style));
}

uint32_t Asteroid::type() const
{
    return WorldHelper::TYPE_ASTEROID;
}
}
}
