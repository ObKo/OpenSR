/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include "PlanetModel.h"
#include "PlanetViewer.h"
#include <QDir>
#include <json/reader.h>
#include <json/writer.h>
#include <locale.h>

Planet::Planet()
{
    hasCloud = false;
    speed = 0.0f;
    cloudSpeed = 0.0f;
    size = 0;
    hasRing = false;
    hasRingBackground = false;
    ringOffsetX = 0.0f;
    ringOffsetY = 0.0f;
    ringBgOffsetX = 0.0f;
    ringBgOffsetY = 0.0f;
}

void PlanetModel::addPlanet(const Planet& planet)
{
    QImage icon(32, 32, QImage::Format_ARGB32);
    PlanetViewer::generatePlanetImage(icon, QImage(QDir(m_settings.value("data/dataDir", "").toString()).absoluteFilePath(planet.texture)), 32);

    if (!m_planets.contains(planet.id))
    {
        int i = qLowerBound(m_planets.keys(), planet.id) - m_planets.keys().begin();

        beginInsertRows(QModelIndex(), i, i);
        m_planets[planet.id] = planet;
        m_icons[planet.id] = QIcon(QPixmap::fromImage(icon));
        endInsertRows();
    }
    else
    {
        int i = qFind(m_planets.keys(), planet.id) - m_planets.keys().begin();

        m_planets[planet.id] = planet;
        m_icons[planet.id] = QIcon(QPixmap::fromImage(icon));

        emit(dataChanged(index(i), index(i)));
    }
}

void PlanetModel::removePlanets(const QModelIndexList& list)
{
    foreach(QModelIndex i, list)
    {
        int row = i.row();
        QString key = m_planets.keys().at(row);

        beginRemoveRows(QModelIndex(), row, row);
        m_planets.remove(key);
        m_icons.remove(key);
        endRemoveRows();
    }
}

void PlanetModel::saveJSON(const QString& file) const
{
    Json::Value root;
    Json::StyledWriter writer;
    foreach(Planet p, m_planets)
    {
        QString colorText = QString("#%1%2%3").arg((ushort)((p.ambientColor.rgba() >> 16) & 0xff), 2, 16, QChar('0'))
                            .arg((ushort)((p.ambientColor.rgba() >> 8) & 0xff), 2, 16, QChar('0'))
                            .arg((ushort)((p.ambientColor.rgba() >> 0) & 0xff), 2, 16, QChar('0')).toUpper();
        Json::Value planet;
        planet["ambientColor"] = colorText.toLatin1().data();
        planet["hasCloud"] = p.hasCloud;
        planet["size"] = p.size;
        planet["speed"] = p.speed;
        planet["texture"] = p.texture.toUtf8().data();
        planet["hasRing"] = p.hasRing;
        planet["hasRingBackground"] = p.hasRingBackground;

        if (p.hasCloud)
        {
            planet["cloud"] = p.cloud.toUtf8().data();
            planet["cloudSpeed"] = p.cloudSpeed;
        }

        if (p.hasRing)
        {
            planet["ring"] = p.ring.toUtf8().data();
            if (p.ringOffsetX != 0.0f)
                planet["ringOffsetX"] = p.ringOffsetX;
            if (p.ringOffsetY != 0.0f)
                planet["ringOffsetY"] = p.ringOffsetX;
        }

        if (p.hasRingBackground)
        {
            planet["ringBackground"] = p.ringBackground.toUtf8().data();
            if (p.ringBgOffsetX != 0.0f)
                planet["ringBgOffsetX"] = p.ringBgOffsetX;
            if (p.ringBgOffsetY != 0.0f)
                planet["ringBgOffsetY"] = p.ringBgOffsetY;
        }

        root[p.id.toLatin1().data()] = planet;
    }
    QFile f(file);
    f.open(QIODevice::WriteOnly);
    setlocale(LC_NUMERIC, "C");
    f.write(QString::fromUtf8(writer.write(root).c_str()).toUtf8());
    setlocale(LC_NUMERIC, "");
    f.close();
}

void PlanetModel::loadJSON(const QString& file)
{
    Json::Value root;
    Json::Reader reader;

    QFile f(file);
    f.open(QIODevice::ReadOnly);
    if (!reader.parse(f.readAll().data(), root))
        return;
    f.close();

    if (!root.isObject())
        return;

    Json::Value::iterator end = root.end();

    QList<Planet> planets;

    for (Json::Value::iterator i = root.begin(); i != end; ++i)
    {
        if (!(*i).isObject())
            return;

        Planet p;
        QString colorText = QString::fromLatin1((*i).get("ambientColor", "#FFFFFF").asCString());
        p.ambientColor = qRgb(colorText.mid(1, 2).toUShort(0, 16), colorText.mid(3, 2).toUShort(0, 16),
                              colorText.mid(5, 2).toUShort(0, 16));
        p.texture = QString::fromUtf8((*i).get("texture", "").asCString());
        p.speed = (*i).get("speed", 0.0f).asDouble();
        p.hasCloud = (*i).get("hasCloud", false).asBool();
        p.id = QString::fromLatin1(i.key().asCString());
        p.size = (*i).get("size", 32).asInt();
        p.hasRing = (*i).get("hasRing", false).asBool();
        p.hasRingBackground = (*i).get("hasRingBackground", false).asBool();

        if (p.hasCloud)
        {
            p.cloud = QString::fromUtf8((*i).get("cloud", "").asCString());
            p.cloudSpeed = (*i).get("cloudSpeed", 0.0f).asDouble();
        }
        else
        {
            p.cloudSpeed = 0.0f;
        }
        if (p.hasRing)
        {
            p.ring = QString::fromUtf8((*i).get("ring", "").asCString());
            p.ringOffsetX = (*i).get("ringOffsetX", 0.0f).asDouble();
            p.ringOffsetY = (*i).get("ringOffsetY", 0.0f).asDouble();
        }
        if (p.hasRingBackground)
        {
            p.ringBackground = QString::fromUtf8((*i).get("ringBackground", "").asCString());
            p.ringBgOffsetX = (*i).get("ringBgOffsetX", 0.0f).asDouble();
            p.ringBgOffsetY = (*i).get("ringBgOffsetY", 0.0f).asDouble();
        }
        planets.append(p);
    }
    m_planets.clear();
    foreach(Planet p, planets)
    {
        addPlanet(p);
    }
}

QVariant PlanetModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_planets.value(m_planets.keys().at(index.row())).id;
    }
    else if (role == Qt::DecorationRole)
    {
        return m_icons.value(m_icons.keys().at(index.row()));
    }
    else
    {
        return QVariant();
    }
}

int PlanetModel::rowCount(const QModelIndex& parent) const
{
    return m_planets.size();
}

Planet PlanetModel::getPlanet(const QModelIndex& i) const
{
    return m_planets.value(m_planets.keys().at(i.row()));
}
