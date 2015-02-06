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

#include "QMLHelper.h"
#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>

namespace OpenSR
{
namespace QML
{
QMLHelper::QMLHelper(QObject* parent): QObject(parent)
{

}

QVariant QMLHelper::questInfo(const QUrl& url)
{
    QIODevice *d = qobject_cast<Engine*>(qApp)->resources()->getIODevice(url);
    if (!d || !d->isOpen())
        return QVariant();

    QVariant result = convertQuestInfoToJS(QM::readQuestInfo(d));

    delete d;

    return result;
}

QVariant QMLHelper::convertQuestInfoToJS(const QM::QuestInfo& info)
{
    QVariantMap map;

    map["races"] = info.races;
    map["doneImmediately"] = info.doneImmediately;
    map["planetRaces"] = info.planetRaces;
    map["playerTypes"] = info.playerTypes;
    map["playerRaces"] = info.playerRaces;
    map["relation"] = info.relation;
    map["difficulty"] = info.difficulty;
    map["winnerText"] = info.winnerText;
    map["descriptionText"] = info.descriptionText;

    return map;
}

QVariant QMLHelper::convertBezierCurveToJS(const BezierCurve& curve)
{
    QVariantMap map;

    map["p0"] = curve.p0;
    map["p1"] = curve.p1;
    map["p2"] = curve.p2;
    map["p3"] = curve.p3;
    return map;
}

QVariant QMLHelper::bezierCurve(float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
{
    return bezierCurve(QPointF(p0x, p0y), QPointF(p1x, p1y), QPointF(p2x, p2y), QPointF(p3x, p3y));
}

QVariant QMLHelper::bezierCurve(const QPointF& p0, const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    BezierCurve curve;
    curve.p0 = p0;
    curve.p1 = p1;
    curve.p2 = p2;
    curve.p3 = p3;
    return convertBezierCurveToJS(curve);
}
}
}

