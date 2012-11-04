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

#ifndef PLANETMODEL_H
#define PLANETMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QSettings>
#include <QIcon>

struct Planet
{
    QString id;
    QString texture;
    QString cloud;
    bool hasCloud;
    float speed;
    float cloudSpeed;
    int size;
    QColor ambientColor;
};

class PlanetModel: public QAbstractListModel
{
    Q_OBJECT

public:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    void addPlanet(const Planet &planet);
    Planet getPlanet(const QModelIndex& i) const;
    void removePlanets(const QModelIndexList& list);

    void saveJSON(const QString& file) const;
    void loadJSON(const QString& file);

private:
    QMap<QString, Planet> m_planets;
    QMap<QString, QIcon> m_icons;
    QSettings m_settings;
};

#endif // PLANETMODEL_H
