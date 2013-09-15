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
}

void Asteroid::setPeriod(float T)
{
    m_period = T;
    calcPosition();
    calcSpeed();
}

void Asteroid::setAngle(float angle)
{
    m_angle = angle;
}

void Asteroid::setTime(float time)
{
    m_t = time;

    if (fabs(m_t) > m_period)
        m_t = fmod(m_t, m_period);

    if (m_t < 0)
        m_t = 2.0f * M_PI + m_t;

    calcPosition();
}

void Asteroid::calcEccentricity()
{
    m_e = sqrt(1 - (m_b * m_b) / (m_a * m_a));
}

void Asteroid::calcPosition()
{
    // http://en.wikipedia.org/wiki/Kepler's_equation
    float M = 2.0f * M_PI / m_period * m_t;
    float E = M;
    for (int j = 0; j < ITERATION_COUNTS; j++)
    {
        E = m_e * sin(E) + M;
    }
    float alpha = 2.0f * atan(sqrt((1.0f + m_e) / (1.0f - m_e)) * tan(E / 2.0f));
    float r = m_a * (1.0f - m_e * cos(E));
    float x = r * cos(alpha), y = r * sin(alpha);
    float angle = m_angle / 180.0f * M_PI;
    m_position.x = x * cos(angle) - y * sin(angle);
    m_position.y = x * sin(angle) + y * cos(angle);
}

void Asteroid::calcSpeed()
{
    float M = 2.0f * M_PI / m_period * (m_t + 1.0f);
    float E = M;
    for (int j = 0; j < ITERATION_COUNTS; j++)
    {
        E = m_e * sin(E) + M;
    }
    float alpha = 2.0f * atan(sqrt((1.0f + m_e) / (1.0f - m_e)) * tan(E / 2.0f));
    float r = m_a * (1.0f - m_e * cos(E));
    float x = r * cos(alpha), y = r * sin(alpha);
    float angle = m_angle / 180.0f * M_PI;
    float dx = x * cos(angle) - y * sin(angle) - m_position.x;
    float dy = x * sin(angle) + y * cos(angle) - m_position.y;
    m_speed = sqrt(dx * dx + dy * dy);
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
