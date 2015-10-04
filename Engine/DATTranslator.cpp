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

#include "OpenSR/DATTranslator.h"

#include "OpenSR/Engine.h"

#include <QVariant>

namespace OpenSR
{
DATTranslator::DATTranslator(QObject *parent) : QTranslator(parent)
{

}

bool DATTranslator::isEmpty() const
{
    return false;
}

QString DATTranslator::translate(const char* context, const char* sourceText, const char* disambiguation, int n) const
{
    if (!QString(context).isEmpty())
        return QString();

    return qobject_cast<Engine*>(qApp)->datValue(sourceText).toString();
}
}
