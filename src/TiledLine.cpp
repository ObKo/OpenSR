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

#include "OpenSR/TiledLine.h"
#include "OpenSR/private/TiledLine_p.h"

#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"


namespace Rangers
{
TiledLinePrivate::TiledLinePrivate(): ObjectPrivate()
{
    vertexCount = 0;
    buffer = 0;
}

TiledLine::TiledLine(Object *parent):
    Object(*(new TiledLinePrivate), parent)
{
}

TiledLine::TiledLine(boost::shared_ptr<Texture> texture, Object *parent):
    Object(*(new TiledLinePrivate), parent)
{
    RANGERS_D(TiledLine);
    d->texture = texture;

    markToUpdate();
}

TiledLine::TiledLine(boost::shared_ptr<Texture> texture, const Vector& start, const Vector& end, Object *parent):
    Object(*(new TiledLinePrivate), parent)
{
    RANGERS_D(TiledLine);
    d->texture = texture;
    d->start = start;
    d->end = end;

    markToUpdate();
}

TiledLine::TiledLine(const std::wstring& texture, Object *parent):
    Object(*(new TiledLinePrivate), parent)
{
    RANGERS_D(TiledLine);
    d->texture = ResourceManager::instance().loadTexture(texture);

    markToUpdate();
}

TiledLine::TiledLine(const std::wstring& texture, const Vector& start, const Vector& end, Object *parent):
    Object(*(new TiledLinePrivate), parent)
{
    RANGERS_D(TiledLine);
    d->texture = ResourceManager::instance().loadTexture(texture);
    d->start = start;
    d->end = end;

    markToUpdate();
}

TiledLine::TiledLine(TiledLinePrivate &p, Object *parent):
    Object(p, parent)
{
    markToUpdate();
}

TiledLine::~TiledLine()
{
    RANGERS_D(TiledLine);
    if (d->buffer)
        glDeleteBuffers(1, &d->buffer);
}

void TiledLine::processMain()
{
    Object::processMain();

    RANGERS_D(TiledLine);

    if (!d->texture)
        return;

    Vertex *vertices = 0;

    if (!d->buffer)
    {
        vertices = new Vertex[4];
        d->vertexCount = 4;
        memset(vertices, 0, d->vertexCount * sizeof(Vertex));

        glGenBuffers(1, &d->buffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * d->vertexCount, vertices, GL_DYNAMIC_DRAW);
        delete[] vertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->buffer);
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    Vector dir = (d->end - d->start);
    Vector n;

    float l = float(sqrt(dir * dir)) / d->texture->width();
    float dl = float(floor(l));
    dl = sqrt((l - dl) * d->texture->width());
    l = float(floor(l));

    dir = dir.norm();

    n.x = - dir.y;
    n.y = dir.x;

    vertices[0].x = d->start.x + n.x / 2.0f * d->texture->height() - dir.x * dl;
    vertices[0].y = d->start.y + n.y / 2.0f * d->texture->height() - dir.y * dl;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;

    vertices[1].x = d->end.x + n.x / 2.0f * d->texture->height() - dir.x * dl;
    vertices[1].y = d->end.y + n.y / 2.0f * d->texture->height() - dir.y * dl;
    vertices[1].u = l;
    vertices[1].v = 0.0f;

    vertices[2].x = d->end.x - n.x / 2.0f * d->texture->height() - dir.x * dl;
    vertices[2].y = d->end.y - n.y / 2.0f * d->texture->height() - dir.y * dl;
    vertices[2].u = l;
    vertices[2].v = 1.0f;

    vertices[3].x = d->start.x - n.x / 2.0f * d->texture->height() - dir.x * dl;
    vertices[3].y = d->start.y - n.y / 2.0f * d->texture->height() - dir.y * dl;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void TiledLine::draw() const
{
    RANGERS_D(const TiledLine);

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

    glBindBuffer(GL_ARRAY_BUFFER, d->buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, d->vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

Vector TiledLine::start() const
{
    RANGERS_D(const TiledLine);
    return d->start;
}

Vector TiledLine::end() const
{
    RANGERS_D(const TiledLine);
    return d->end;
}

boost::shared_ptr<Texture> TiledLine::texture() const
{
    RANGERS_D(const TiledLine);
    return d->texture;
}

void TiledLine::setLine(const Vector& start, const Vector& end)
{
    RANGERS_D(TiledLine);
    d->start = start;
    d->end = end;
    markToUpdate();
}

void TiledLine::setTexture(boost::shared_ptr<Texture> texture)
{
    RANGERS_D(TiledLine);
    d->texture = texture;
    TiledLine::markToUpdate();
}
}
