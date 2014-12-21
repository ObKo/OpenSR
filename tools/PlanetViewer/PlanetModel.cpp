/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

namespace OpenSR
{
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
    QJsonObject root;
    foreach(Planet p, m_planets)
    {
        QString colorText = QString("#%1%2%3").arg((ushort)((p.ambientColor.rgba() >> 16) & 0xff), 2, 16, QChar('0'))
                            .arg((ushort)((p.ambientColor.rgba() >> 8) & 0xff), 2, 16, QChar('0'))
                            .arg((ushort)((p.ambientColor.rgba() >> 0) & 0xff), 2, 16, QChar('0')).toUpper();
        QJsonObject planet;
        planet["ambientColor"] = colorText;
        planet["hasCloud"] = p.hasCloud;
        planet["size"] = p.size;
        planet["speed"] = p.speed;
        planet["texture"] = p.texture;
        planet["hasRing"] = p.hasRing;
        planet["hasRingBackground"] = p.hasRingBackground;

        if (p.hasCloud)
        {
            planet["cloud"] = p.cloud;
            planet["cloudSpeed"] = p.cloudSpeed;
        }

        if (p.hasRing)
        {
            planet["ring"] = p.ring;
            if (p.ringOffsetX != 0.0f)
                planet["ringOffsetX"] = p.ringOffsetX;
            if (p.ringOffsetY != 0.0f)
                planet["ringOffsetY"] = p.ringOffsetX;
        }

        if (p.hasRingBackground)
        {
            planet["ringBackground"] = p.ringBackground;
            if (p.ringBgOffsetX != 0.0f)
                planet["ringBgOffsetX"] = p.ringBgOffsetX;
            if (p.ringBgOffsetY != 0.0f)
                planet["ringBgOffsetY"] = p.ringBgOffsetY;
        }

        root[p.id] = planet;
    }
    QJsonDocument doc(root);
    QFile f(file);
    f.open(QIODevice::WriteOnly);
    f.write(doc.toJson());
    f.close();
}

void PlanetModel::loadJSON(const QString& file)
{
    QJsonParseError err;
    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
    f.close();

    if (err.error != QJsonParseError::NoError)
    {
        emit(error(tr("Load JSON"), tr("Cannot parse JSON file: %1").arg(err.errorString())));
        return;
    }

    QJsonObject root = doc.object();
    QList<Planet> planets;

    QJsonObject::const_iterator end = root.end();
    for (QJsonObject::const_iterator i = root.begin(); i != end; ++i)
    {
        if (!i.value().isObject())
        {
            emit(error(tr("Load JSON"), tr("Invalid planet style: \"%1\"").arg(i.key())));
            continue;
        }

        QVariantMap jsonPlanet = i.value().toObject().toVariantMap();
        Planet p;
        QString colorText = jsonPlanet.value("ambientColor", "#FFFFFF").toString();
        p.ambientColor = qRgb(colorText.mid(1, 2).toUShort(0, 16), colorText.mid(3, 2).toUShort(0, 16),
                              colorText.mid(5, 2).toUShort(0, 16));
        p.texture = jsonPlanet.value("texture", "").toString();
        p.speed = jsonPlanet.value("speed", 0.0f).toDouble();
        p.hasCloud = jsonPlanet.value("hasCloud", false).toBool();
        p.id = i.key();
        p.size = jsonPlanet.value("size", 120).toInt();
        p.hasRing = jsonPlanet.value("hasRing", false).toBool();
        p.hasRingBackground = jsonPlanet.value("hasRingBackground", false).toBool();

        if (p.hasCloud)
        {
            p.cloud = jsonPlanet.value("cloud", "").toString();
            p.cloudSpeed = jsonPlanet.value("cloudSpeed", 0.0f).toDouble();
        }
        else
        {
            p.cloudSpeed = 0.0f;
        }
        if (p.hasRing)
        {
            p.ring = jsonPlanet.value("ring", "").toString();
            p.ringOffsetX = jsonPlanet.value("ringOffsetX", 0.0f).toDouble();
            p.ringOffsetY = jsonPlanet.value("ringOffsetY", 0.0f).toDouble();
        }
        if (p.hasRingBackground)
        {
            p.ringBackground = jsonPlanet.value("ringBackground", "").toString();
            p.ringBgOffsetX = jsonPlanet.value("ringBgOffsetX", 0.0f).toDouble();
            p.ringBgOffsetY = jsonPlanet.value("ringBgOffsetY", 0.0f).toDouble();
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
}
