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
    vertexBuffer = 0;
    vertexCount = 0;
}

TiledPolyline::TiledPolyline(): Object(*(new TiledPolylinePrivate()))
{
}

TiledPolyline::TiledPolyline(boost::shared_ptr<Texture> texture): Object(*(new TiledPolylinePrivate()))
{
    RANGERS_D(TiledPolyline);
    d->texture = texture;
}

TiledPolyline::TiledPolyline(const std::string& texture): Object(*(new TiledPolylinePrivate()))
{
    RANGERS_D(TiledPolyline);
    d->texture = ResourceManager::instance().loadTexture(texture);
}

TiledPolyline::TiledPolyline(TiledPolylinePrivate &p): Object(p)
{
}

TiledPolyline::~TiledPolyline()
{
    RANGERS_D(TiledPolyline);
    if (d->vertexBuffer)
        glDeleteBuffers(1, &d->vertexBuffer);
}

void TiledPolyline::draw() const
{
    RANGERS_D(const TiledPolyline);

    if (!d->texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, d->texture->openGLTexture());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->vertexBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, d->vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

void TiledPolyline::processMain()
{
    Object::processMain();

    // TODO: Optimize this
    RANGERS_D(TiledPolyline);

    if (d->points.size() < 2)
        return;

    if (!d->texture)
        return;

    if (!d->vertexBuffer)
    {
        glGenBuffers(1, &d->vertexBuffer);
    }

    /*
     * Calc vertex positions for polyline.
     */

    std::vector<Vector> vertexPoints;

    std::vector<Vector>::const_iterator end = d->points.end() - 1;
    std::vector<Vector>::const_iterator prev = d->points.begin();
    std::vector<Vector>::const_iterator next = d->points.begin() + 1;

    Vector v1, v2;
    Vector dir = ((*next) - (*prev)).norm();
    Vector n = Vector(- dir.y, dir.x);

    v1 = Vector((*prev).x + n.x * d->texture->height() / 2.0f, (*prev).y + n.y * d->texture->height() / 2.0f);
    v2 = Vector((*prev).x - n.x * d->texture->height() / 2.0f, (*prev).y - n.y * d->texture->height() / 2.0f);

    vertexPoints.push_back(v1);
    vertexPoints.push_back(v2);

    ++next;

    for (std::vector<Vector>::const_iterator i = prev + 1; i != end; ++i)
    {
        Vector d1 = ((*i - *prev).norm() + (*next - *i).norm()).norm();
        Vector d2 = (*i - *prev).norm();
        Vector n1 = Vector(- d1.y, d1.x);
        Vector n2 = Vector(- d2.y, d2.x);
        float cosa = n1.x * n2.x + n1.y * n2.y;

        v1 = Vector((*i).x + n1.x * d->texture->height() / 2.0f / cosa, (*i).y + n1.y * d->texture->height() / 2.0f / cosa);
        v2 = Vector((*i).x - n1.x * d->texture->height() / 2.0f / cosa, (*i).y - n1.y * d->texture->height() / 2.0f / cosa);

        vertexPoints.push_back(v1);
        vertexPoints.push_back(v2);

        ++next;
        prev = i;
    }

    end = d->points.end() - 1;
    prev = end - 1;

    dir = ((*end) - (*prev)).norm();
    n = Vector(- dir.y, dir.x);

    v1 = Vector((*end).x + n.x * d->texture->height() / 2.0f, (*end).y + n.y * d->texture->height() / 2.0f);
    v2 = Vector((*end).x - n.x * d->texture->height() / 2.0f, (*end).y - n.y * d->texture->height() / 2.0f);

    vertexPoints.push_back(v1);
    vertexPoints.push_back(v2);

    d->vertexCount = (d->points.size() - 1) * 4;
    Vertex *vertices = new Vertex[d->vertexCount];


    /*
     * Calc texture coordinates.
     */

    float uStart = 0.0f, uDelta = 0.0f;
    bool bottomStart = false;
    Vector &prevTop = vertexPoints[0];
    Vector &prevBottom = vertexPoints[1];

    for (int i = 1; i < d->points.size(); i++)
    {
        Vector dT, dB;
        int j = (i - 1) * 4;

        dT = vertexPoints[i * 2] - prevTop;
        dB = vertexPoints[i * 2 + 1] - prevBottom;

        vertices[j].x = prevTop.x;
        vertices[j].y = prevTop.y;

        vertices[j + 1].x = prevBottom.x;
        vertices[j + 1].y = prevBottom.y;

        vertices[j + 2].x = vertexPoints[i * 2 + 1].x;
        vertices[j + 2].y = vertexPoints[i * 2 + 1].y;

        vertices[j + 3].x = vertexPoints[i * 2].x;
        vertices[j + 3].y = vertexPoints[i * 2].y;

        if (bottomStart)
        {
            vertices[j].u = uStart + uDelta;
            vertices[j + 1].u = uStart;
        }
        else
        {
            vertices[j].u = uStart;
            vertices[j + 1].u = uStart + uDelta;
        }

        vertices[j].v = 0.0f;
        vertices[j + 1].v = 1.0f;

        vertices[j + 2].u = vertices[j + 1].u + (sqrt((dB.x * dB.x) + (dB.y * dB.y))) / d->texture->width();
        vertices[j + 2].v = 1.0f;

        vertices[j + 3].u = vertices[j].u + (sqrt((dT.x * dT.x) + (dT.y * dT.y))) / d->texture->width();
        vertices[j + 3].v = 0.0f;

        if (vertices[j + 3].u > vertices[j + 2].u)
        {
            bottomStart = false;
            uStart = fmod(vertices[j + 3].u, d->texture->width());
            uDelta = vertices[j + 3].u - vertices[j + 2].u;
        }
        else
        {
            bottomStart = true;
            uStart = fmod(vertices[j + 2].u, d->texture->width());
            uDelta = vertices[j + 2].u - vertices[j + 3].u;
        }

        prevTop = vertexPoints[i * 2];
        prevBottom = vertexPoints[i * 2 + 1];
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * d->vertexCount, vertices, GL_DYNAMIC_DRAW);

    delete[] vertices;

}

boost::shared_ptr<Texture> TiledPolyline::texture() const
{
    RANGERS_D(const TiledPolyline);
    return d->texture;
}

std::vector<Vector> TiledPolyline::polyline() const
{
    RANGERS_D(const TiledPolyline);
    return d->points;
}

void TiledPolyline::setPolyline(const std::vector<Vector> points)
{
    RANGERS_D(TiledPolyline);

    d->points = points;

    markToUpdate();
}

void TiledPolyline::setTexture(boost::shared_ptr<Texture> texture)
{
    RANGERS_D(TiledPolyline);
    d->texture = texture;

    markToUpdate();
}

void TiledPolyline::setTexture(const std::string& texture)
{
    RANGERS_D(TiledPolyline);
    d->texture = ResourceManager::instance().loadTexture(texture);

    markToUpdate();
}
}
