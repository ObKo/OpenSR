/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
namespace
{
const int ITERATION_COUNTS = 10;
}

const quint32 Asteroid::m_staticTypeId = typeIdFromClassName(Asteroid::staticMetaObject.className());

template<>
void WorldObject::registerType<Asteroid>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<AsteroidStyle>();
    qRegisterMetaTypeStreamOperators<AsteroidStyle>();
    qRegisterMetaType<AsteroidStyle::Data>();
    qRegisterMetaTypeStreamOperators<AsteroidStyle::Data>();
    qmlRegisterType<Asteroid>("OpenSR.World", 1, 0, "Asteroid");
}

template<>
Asteroid* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Asteroid(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<Asteroid>()
{
    return Asteroid::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<Asteroid>()
{
    return &Asteroid::staticMetaObject;
}

QString AsteroidStyle::texture() const
{
    return getData<Data>().texture;
}

QColor AsteroidStyle::color() const
{
    return getData<Data>().color;
}

void AsteroidStyle::setTexture(const QString& texture)
{
    auto d = getData<Data>();
    d.texture = texture;
    setData(d);
}

void AsteroidStyle::setColor(const QColor& color)
{
    auto d = getData<Data>();
    d.color = color;
    setData(d);
}

QDataStream& operator<<(QDataStream & stream, const AsteroidStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, AsteroidStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const AsteroidStyle::Data& data)
{
    return stream << data.color << data.texture;
}

QDataStream& operator>>(QDataStream & stream, AsteroidStyle::Data& data)
{
    return stream >> data.color >> data.texture;
}

Asteroid::Asteroid(WorldObject *parent, quint32 id): SpaceObject(parent, id),
    m_angle(0), m_period(0), m_time(0), m_speed(0)
{
}

Asteroid::~Asteroid()
{
}

AsteroidStyle Asteroid::style() const
{
    return m_style;
}

QPointF Asteroid::semiAxis() const
{
    return m_semiAxis;
}

float Asteroid::angle() const
{
    return m_angle;
}

float Asteroid::period() const
{
    return m_period;
}

float Asteroid::time() const
{
    return m_time;
}

float Asteroid::speed() const
{
    return m_speed;
}

void Asteroid::setStyle(const AsteroidStyle& style)
{
    m_style = style;
    emit(styleChanged());
}

void Asteroid::setSemiAxis(const QPointF& axis)
{
    if (axis != m_semiAxis)
    {
        if (axis.x() < axis.y())
        {
            qWarning() << "Asteroid: semi-major axis is less than semi-minor, axis will be swapped";
            m_semiAxis.setX(axis.y());
            m_semiAxis.setY(axis.x());
        }
        else
            m_semiAxis = axis;

        emit(semiAxisChanged());
        calcEccentricity();
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

void Asteroid::setAngle(float angle)
{
    if (angle != m_angle)
    {
        m_angle = angle;
        emit(angleChanged());
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

void Asteroid::setPeriod(float period)
{
    if (period != m_period)
    {
        m_period = period;
        emit(periodChanged());
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

void Asteroid::setTime(float time)
{
    if (time != m_time)
    {
        m_time = time;
        emit(timeChanged());
        calcPosition();
        calcSpeed();
        updateTrajectory();
    }
}

quint32 Asteroid::typeId() const
{
    return Asteroid::m_staticTypeId;
}

QString Asteroid::namePrefix() const
{
    return tr("Asteroid");
}

void Asteroid::prepareSave()
{
    m_style.registerResource();
}

void Asteroid::calcEccentricity()
{
    m_e = sqrt(1 - (m_semiAxis.y() * m_semiAxis.y()) / (m_semiAxis.x() * m_semiAxis.x()));
}

void Asteroid::calcPosition(float dt)
{
    QPointF next = E(solveKepler(m_time + dt));
    setPosition(next);
}

void Asteroid::calcSpeed(float dt)
{
    QPointF nextPos = E(solveKepler(m_time + dt + 1.0f));
    float dx = nextPos.x() - position().x();
    float dy = nextPos.y() - position().y();
    m_speed = sqrt(dx * dx + dy * dy);
    emit(speedChanged());
}

float Asteroid::solveKepler(float t)
{
    // http://en.wikipedia.org/wiki/Kepler's_equation
    float M = 2.0f * M_PI / m_period * t;
    float E = M;
    for (int j = 0; j < ITERATION_COUNTS; j++)
        E = m_e * sin(E) + M;
    return E;
}

QPointF Asteroid::E(float eta)
{
    QPointF p;
    p.setX(-(m_semiAxis.x() * m_e) * cos(m_angle) + m_semiAxis.x() * cos(m_angle) * cos(eta) - m_semiAxis.y() * sin(m_angle) * sin(eta));
    p.setY(-(m_semiAxis.x() * m_e) * sin(m_angle) + m_semiAxis.x() * sin(m_angle) * cos(eta) + m_semiAxis.y() * cos(m_angle) * sin(eta));
    return p;
}

QPointF Asteroid::Ederiv(float eta)
{
    QPointF p;
    p.setX(- m_semiAxis.x() * cos(m_angle) * sin(eta) - m_semiAxis.y() * sin(m_angle) * cos(eta));
    p.setY(- m_semiAxis.x() * sin(m_angle) * sin(eta) + m_semiAxis.y() * cos(m_angle) * cos(eta));
    return p;
}

void Asteroid::updateTrajectory()
{
    QList<BezierCurve> trajectory;

    float prev = solveKepler(m_time);

    for (int i = 1; i < int(round(m_period)) + 1; i++)
    {
        float t = m_time + i;
        if (fabs(t) > m_period)
            t = fmod(t, m_period);

        if (t < 0)
            t = m_period + t;

        float eta = solveKepler(t);

        QPointF p;
        BezierCurve c;

        // http://www.spaceroots.org/documents/ellipse/elliptical-arc.pdf
        p = E(prev);
        c.p0 = QPointF(p.x(), p.y());
        p = E(eta);
        c.p3 = QPointF(p.x(), p.y());

        float tangent = tan((eta - prev) / 2.0f);
        float k = sin(eta - prev) * (sqrt(4.0f + 3.0f * tangent * tangent) - 1.0f) / 3.0f;

        p = Ederiv(prev);
        c.p1 = c.p0 + k * QPointF(p.x(), p.y());
        p = Ederiv(eta);
        c.p2 = c.p3 - k * QPointF(p.x(), p.y());

        trajectory.push_back(c);

        prev = eta;
    }
    setTrajectory(trajectory);
}

void Asteroid::startTurn()
{
    SpaceObject::startTurn();
}

void Asteroid::processTurn(float time)
{
    calcPosition(time);
    SpaceObject::processTurn(time);
}

void Asteroid::finishTurn()
{
    setTime(m_time + 1.0);
    SpaceObject::finishTurn();
}
}
}
