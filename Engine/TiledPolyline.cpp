/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/TiledPolyline.h"
#include "OpenSR/TiledLine.h"
#include "OpenSR/private/TiledPolyline_p.h"

#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"


namespace Rangers
{
TiledPolylinePrivate::TiledPolylinePrivate()
{
}

TiledPolyline::TiledPolyline(): Object(*(new TiledPolylinePrivate()))
{
}

TiledPolyline::TiledPolyline(boost::shared_ptr<Texture> texture): Object(*(new TiledPolylinePrivate()))
{
    RANGERS_D(TiledPolyline);
    d->texture = texture;
}

TiledPolyline::TiledPolyline(const std::wstring& texture): Object(*(new TiledPolylinePrivate()))
{
    RANGERS_D(TiledPolyline);
    d->texture = ResourceManager::instance().loadTexture(texture);
}

TiledPolyline::TiledPolyline(TiledPolylinePrivate &p): Object(p)
{
}

TiledPolyline::~TiledPolyline()
{

}

void TiledPolyline::draw() const
{
    RANGERS_D(const TiledPolyline);
    if (!prepareDraw())
        return;

    std::vector<boost::shared_ptr<TiledLine> >::const_iterator end = d->lines.end();
    for (std::vector<boost::shared_ptr<TiledLine> >::const_iterator i = d->lines.begin(); i != end; ++i)
    {
        (*i)->draw();
    }

    endDraw();
}

boost::shared_ptr<Texture> TiledPolyline::texture() const
{
    RANGERS_D(const TiledPolyline);
    return d->texture;
}

std::vector<Vector> TiledPolyline::polyline() const
{
}

void TiledPolyline::setPolyLine(const std::vector<Vector> points)
{
    RANGERS_D(TiledPolyline);
    d->lines.clear();

    if (points.size() < 2)
        return;

    std::vector<Vector>::const_iterator end = points.end();
    std::vector<Vector>::const_iterator prev = points.begin();
    for (std::vector<Vector>::const_iterator i = prev + 1; i != end; ++i)
    {
        boost::shared_ptr<TiledLine> l = boost::shared_ptr<TiledLine>(new TiledLine(d->texture, *prev, *i));
        d->lines.push_back(l);
        addChild(l);
        prev = i;
    }
}

void TiledPolyline::setTexture(boost::shared_ptr<Texture> texture)
{
    RANGERS_D(TiledPolyline);
    d->texture = texture;
}

void TiledPolyline::setTexture(const std::wstring& texture)
{
    RANGERS_D(TiledPolyline);
    d->texture = ResourceManager::instance().loadTexture(texture);
}
}
