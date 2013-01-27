/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#include "Sprite.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "private/Sprite_p.h"

namespace Rangers
{
SpritePrivate::SpritePrivate(): ObjectPrivate()
{
    vertices = 0;
    vertexCount = 0;
    width = 0;
    height = 0;
    xOrigin = POSITION_X_LEFT;
    yOrigin = POSITION_Y_TOP;
    scaling = TEXTURE_NORMAL;
    buffer = 0;
}

Sprite::Sprite(Object *parent): Object(*(new SpritePrivate), parent)
{
}

Sprite::Sprite(boost::shared_ptr<Texture> texture,  Object *parent, TextureScaling  ts, SpriteXOrigin xpos, SpriteYOrigin ypos): Object(*(new SpritePrivate), parent)
{
    RANGERS_D(Sprite);

    d->region = TextureRegion(texture);
    d->scaling = ts;
    if (texture)
    {
        d->width = texture->width();
        d->height = texture->height();
    }
    d->xOrigin = xpos;
    d->yOrigin = ypos;
    markToUpdate();
}

Sprite::Sprite(const std::wstring& texture,  Object *parent, TextureScaling  ts, SpriteXOrigin xpos, SpriteYOrigin ypos): Object(*(new SpritePrivate), parent)
{
    RANGERS_D(Sprite);
    d->scaling = ts;
    d->region = TextureRegion(ResourceManager::instance().loadTexture(texture));
    if (d->region.texture)
    {
        d->width = d->region.texture->width();
        d->height = d->region.texture->height();
    }
    d->xOrigin = xpos;
    d->yOrigin = ypos;
    markToUpdate();
}

Sprite::Sprite(const TextureRegion& region, Object *parent): Object(*(new SpritePrivate), parent)
{
    RANGERS_D(Sprite);
    d->xOrigin = POSITION_X_LEFT;
    d->yOrigin = POSITION_Y_TOP;
    d->scaling = TEXTURE_NORMAL;
    d->region = region;
    if (d->region.texture)
    {
        d->width = d->region.texture->width() * (d->region.u2 - d->region.u1);
        d->height = d->region.texture->height() * (d->region.v2 - d->region.v1);
    }
    markToUpdate();
}


Sprite::Sprite(SpritePrivate &p, Object *parent): Object(p, parent)
{
}

Sprite::~Sprite()
{
    RANGERS_D(Sprite);
    if (d->buffer)
        glDeleteBuffers(1, &d->buffer);
}

void Sprite::draw() const
{
    RANGERS_D(const Sprite);
    if (!d->region.texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, d->region.texture->openGLTexture());

    if (d->scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    if (d->scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    if (d->scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

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

void Sprite::setOrigin(SpriteXOrigin xpos, SpriteYOrigin ypos)
{
    lock();
    RANGERS_D(Sprite);
    d->xOrigin = xpos;
    d->yOrigin = ypos;
    markToUpdate();
    unlock();
}

float Sprite::normalHeight() const
{
    RANGERS_D(const Sprite);
    if (d->region.texture)
        return (d->region.v2 - d->region.v1) * d->region.texture->height();
    else
        return 0.0f;
}

float Sprite::normalWidth() const
{
    RANGERS_D(const Sprite);
    if (d->region.texture)
        return (d->region.u2 - d->region.u1) * d->region.texture->width();
    else
        return 0.0f;
}

void Sprite::setTexture(boost::shared_ptr<Texture> texture)
{
    lock();
    RANGERS_D(Sprite);
    d->region = TextureRegion(texture);
    unlock();
}

void Sprite::setGeometry(float width, float height)
{
    lock();
    RANGERS_D(Sprite);
    if (d->region.texture)
    {
        if (width <= 0)
            width = d->region.texture->width() * (d->region.u2 - d->region.u1);
        if (height <= 0)
            height = d->region.texture->height() * (d->region.v2 - d->region.v1);
    }

    d->width = width;
    d->height = height;
    markToUpdate();
    unlock();
}

void Sprite::setHeight(float height)
{
    lock();
    RANGERS_D(Sprite);
    if (d->region.texture)
        if (height <= 0)
            height = d->region.texture->height() * (d->region.v2 - d->region.v1);

    d->height = height;
    markToUpdate();
    unlock();
}

void Sprite::setWidth(float width)
{
    lock();
    RANGERS_D(Sprite);
    if (d->region.texture)
        if (width <= 0)
            width = d->region.texture->width() * (d->region.u2 - d->region.u1);

    d->width = width;
    markToUpdate();
    unlock();
}


void Sprite::setTextureScaling(TextureScaling ts)
{
    lock();
    RANGERS_D(Sprite);
    d->scaling = ts;
    markToUpdate();
    unlock();
}


void Sprite::processMain()
{
    Object::processMain();

    RANGERS_D(Sprite);
    if (!d->region.texture)
        return;

    lock();
    if (!d->buffer)
    {
        d->vertices = new Vertex[4];
        d->vertexCount = 4;
        memset(d->vertices, 0, d->vertexCount * sizeof(Vertex));

        glGenBuffers(1, &d->buffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * d->vertexCount, d->vertices, GL_DYNAMIC_DRAW);
        delete[] d->vertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->buffer);
    d->vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    float u1 = 0, u2 = 0, v1 = 0, v2 = 0;

    switch (d->xOrigin)
    {
    case POSITION_X_CENTER:
        x1 = -d->width / 2.0f;
        x2 = d->width / 2.0f;
        break;
    case POSITION_X_LEFT:
        x1 = 0;
        x2 = d->width;
        break;
    case POSITION_X_RIGHT:
        x1 = -d->width;
        x2 = 0;
        break;
    }

    switch (d->yOrigin)
    {
    case POSITION_Y_CENTER:
        y1 = -d->height / 2.0f;
        y2 = d->height / 2.0f;
        break;
    case POSITION_Y_TOP:
        y1 = 0;
        y2 = d->height;
        break;
    case POSITION_Y_BOTTOM:
        y1 = -d->height;
        y2 = 0;
        break;
    }

    //FIXME: Scaling on texture region working incorrectly.
    //TODO: Keepaspect
    switch (d->scaling)
    {
    case TEXTURE_NO:
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u1 + d->width / ((d->region.u2 - d->region.u1) * d->region.texture->width());
        v2 = d->region.v1 + d->height / ((d->region.v2 - d->region.v1) * d->region.texture->height());
        break;
    case TEXTURE_NORMAL:
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u2;
        v2 = d->region.v2;
        break;
    case TEXTURE_KEEPASPECT:
        /*u1 = 0;
        v1 = 0;
        if (width / texture->width() * texture->height() > height)
        {
            u2 = texture->height() / texture->width();
            v2 = 1;
        }
        else
        {
            u2 = 1;
            v2 = texture->width() / texture->height();
        }*/
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u2;
        v2 = d->region.v2;
        break;
    case TEXTURE_KEEPASPECT_EXPANDING:
        /*u1 = 0;
        v1 = 0;
        if (width / texture->width() * texture->height() > height)
        {
            u2 = 1;
            v2 = height / texture->height();
        }
        else
        {
            u2 = width / texture->width();
            v2 = 1;
        }*/
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u2;
        v2 = d->region.v2;
        break;
    case TEXTURE_TILE_X:
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u1 + d->width / ((d->region.u2 - d->region.u1) * d->region.texture->width());
        v2 = d->region.v2;
        break;
    case TEXTURE_TILE_Y:
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u2;
        v2 = d->region.v1 + d->height / ((d->region.v2 - d->region.v1) * d->region.texture->height());
        break;
    case TEXTURE_TILE:
        u1 = d->region.u1;
        v1 = d->region.v1;
        u2 = d->region.u1 + d->width / ((d->region.u2 - d->region.u1) * d->region.texture->width());
        v2 = d->region.v1 + d->height / ((d->region.v2 - d->region.v1) * d->region.texture->height());
        break;
    }

    d->vertices[0].x = x1;
    d->vertices[0].y = y1;
    d->vertices[0].u = u1;
    d->vertices[0].v = v1;

    d->vertices[1].x = x2;
    d->vertices[1].y = y1;
    d->vertices[1].u = u2;
    d->vertices[1].v = v1;

    d->vertices[2].x = x2;
    d->vertices[2].y = y2;
    d->vertices[2].u = u2;
    d->vertices[2].v = v2;

    d->vertices[3].x = x1;
    d->vertices[3].y = y2;
    d->vertices[3].u = u1;
    d->vertices[3].v = v2;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    unlock();
}

float Sprite::height() const
{
    RANGERS_D(const Sprite);
    return d->height;
}

float Sprite::width() const
{
    RANGERS_D(const Sprite);
    return d->width;
}

TextureRegion Sprite::region() const
{
    RANGERS_D(const Sprite);
    return d->region;
}
}
