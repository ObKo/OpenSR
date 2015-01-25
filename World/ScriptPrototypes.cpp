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

#include "ScriptPrototypes.h"

#include "Race.h"
#include <QtScript>

namespace OpenSR
{
namespace World
{
RaceStylePrototype::RaceStylePrototype(QObject *parent): QObject(parent)
{
}

RaceStylePrototype::~RaceStylePrototype()
{
}

QString RaceStylePrototype::icon() const
{
    RaceStyle *race = qscriptvalue_cast<RaceStyle*>(thisObject());
    if (race)
        return race->icon;
    return QString();
}

void RaceStylePrototype::setIcon(const QString& icon)
{
    RaceStyle *race = qscriptvalue_cast<RaceStyle*>(thisObject());
    if (race)
        race->icon = icon;
}

QString RaceStylePrototype::color() const
{
    RaceStyle *race = qscriptvalue_cast<RaceStyle*>(thisObject());
    if (race)
        return race->color.name();
    return QString();
}

void RaceStylePrototype::setColor(const QString& color)
{
    RaceStyle *race = qscriptvalue_cast<RaceStyle*>(thisObject());
    if (race)
        race->color = QColor(color);
}

QString RaceStylePrototype::sound() const
{
    RaceStyle *race = qscriptvalue_cast<RaceStyle*>(thisObject());
    if (race)
        return race->color.name();
    return QString();
}

void RaceStylePrototype::setSound(const QString& sound)
{
    RaceStyle *race = qscriptvalue_cast<RaceStyle*>(thisObject());
    if (race)
        race->sound = sound;
}

QScriptValue RaceStylePrototype::ctor(QScriptContext *context, QScriptEngine *engine)
{
    RaceStyle style;
    style.icon = context->argument(0).toString();
    style.color = QColor(context->argument(1).toString());
    style.sound = context->argument(2).toString();
    return engine->toScriptValue(style);
}
}
}
