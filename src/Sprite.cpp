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
    m_vertices = 0;
    m_vertexCount = 0;
    m_width = 0;
    m_height = 0;
    m_xOrigin = POSITION_X_LEFT;
    m_yOrigin = POSITION_Y_TOP;
    m_scaling = TEXTURE_NORMAL;
    m_buffer = 0;
}

Sprite::Sprite(Object *parent): Object(*(new SpritePrivate), parent)
{
}

Sprite::Sprite(const Rangers::Sprite& other): Object(*(new SpritePrivate), other)
{
    RANGERS_D(Sprite);

    d->m_width = other.d_func()->m_width;
    d->m_height = other.d_func()->m_height;
    d->m_xOrigin = other.d_func()->m_xOrigin;
    d->m_yOrigin = other.d_func()->m_yOrigin;
    d->m_scaling = other.d_func()->m_scaling;
    d->m_region = other.d_func()->m_region;

    markToUpdate();
}

Sprite::Sprite(SpritePrivate &p, const Sprite& other): Object(p, other)
{
    RANGERS_D(Sprite);

    d->m_width = other.d_func()->m_width;
    d->m_height = other.d_func()->m_height;
    d->m_xOrigin = other.d_func()->m_xOrigin;
    d->m_yOrigin = other.d_func()->m_yOrigin;
    d->m_scaling = other.d_func()->m_scaling;
    d->m_region = other.d_func()->m_region;

    markToUpdate();
}


Sprite::Sprite(boost::shared_ptr<Texture> texture,  Object *parent, TextureScaling  ts, SpriteXOrigin xpos, SpriteYOrigin ypos): Object(*(new SpritePrivate), parent)
{
    RANGERS_D(Sprite);

    d->m_region = TextureRegion(texture);
    d->m_scaling = ts;
    if (texture)
    {
        d->m_width = texture->width();
        d->m_height = texture->height();
    }
    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    markToUpdate();
}

Sprite::Sprite(const std::wstring& texture,  Object *parent, TextureScaling  ts, SpriteXOrigin xpos, SpriteYOrigin ypos): Object(*(new SpritePrivate), parent)
{
    RANGERS_D(Sprite);
    d->m_scaling = ts;
    d->m_region = TextureRegion(ResourceManager::instance()->loadTexture(texture));
    if (d->m_region.texture)
    {
        d->m_width = d->m_region.texture->width();
        d->m_height = d->m_region.texture->height();
    }
    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    markToUpdate();
}

Sprite::Sprite(const TextureRegion& region, Object *parent): Object(*(new SpritePrivate), parent)
{
    RANGERS_D(Sprite);
    d->m_xOrigin = POSITION_X_LEFT;
    d->m_yOrigin = POSITION_Y_TOP;
    d->m_scaling = TEXTURE_NORMAL;
    d->m_region = region;
    if (d->m_region.texture)
    {
        d->m_width = d->m_region.texture->width() * (d->m_region.u2 - d->m_region.u1);
        d->m_height = d->m_region.texture->height() * (d->m_region.v2 - d->m_region.v1);
    }
    markToUpdate();
}


Sprite::Sprite(SpritePrivate &p, Object *parent): Object(p, parent)
{
}

Sprite::~Sprite()
{
    RANGERS_D(Sprite);
    if (d->m_buffer)
        glDeleteBuffers(1, &d->m_buffer);
}

Sprite& Sprite::operator=(const Rangers::Sprite& other)
{
    if (this == &other)
        return *this;

    RANGERS_D(Sprite);

    d->m_buffer = 0;
    d->m_vertices = 0;
    d->m_vertexCount = 0;

    d->m_width = other.d_func()->m_width;
    d->m_height = other.d_func()->m_height;
    d->m_xOrigin = other.d_func()->m_xOrigin;
    d->m_yOrigin = other.d_func()->m_yOrigin;
    d->m_scaling = other.d_func()->m_scaling;
    d->m_region = other.d_func()->m_region;

    markToUpdate();
    Object::operator=(other);
    return *this;
}


void Sprite::draw() const
{
    RANGERS_D(const Sprite);
    if (!d->m_region.texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, d->m_region.texture->openGLTexture());

    if (d->m_scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    if (d->m_scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    if (d->m_scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, d->m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, d->m_vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

void Sprite::setOrigin(SpriteXOrigin xpos, SpriteYOrigin ypos)
{
    lock();
    RANGERS_D(Sprite);
    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    markToUpdate();
    unlock();
}

float Sprite::normalHeight() const
{
    RANGERS_D(const Sprite);
    if (d->m_region.texture)
        return (d->m_region.v2 - d->m_region.v1) * d->m_region.texture->height();
    else
        return 0.0f;
}

float Sprite::normalWidth() const
{
    RANGERS_D(const Sprite);
    if (d->m_region.texture)
        return (d->m_region.u2 - d->m_region.u1) * d->m_region.texture->width();
    else
        return 0.0f;
}

void Sprite::setTexture(boost::shared_ptr<Texture> texture)
{
    lock();
    RANGERS_D(Sprite);
    d->m_region = TextureRegion(texture);
    unlock();
}

void Sprite::setGeometry(float width, float height)
{
    lock();
    RANGERS_D(Sprite);
    if (d->m_region.texture)
    {
        if (width <= 0)
            width = d->m_region.texture->width() * (d->m_region.u2 - d->m_region.u1);
        if (height <= 0)
            height = d->m_region.texture->height() * (d->m_region.v2 - d->m_region.v1);
    }

    d->m_width = width;
    d->m_height = height;
    markToUpdate();
    unlock();
}

void Sprite::setHeight(float height)
{
    lock();
    RANGERS_D(Sprite);
    if (d->m_region.texture)
        if (height <= 0)
            height = d->m_region.texture->height() * (d->m_region.v2 - d->m_region.v1);

    d->m_height = height;
    markToUpdate();
    unlock();
}

void Sprite::setWidth(float width)
{
    lock();
    RANGERS_D(Sprite);
    if (d->m_region.texture)
        if (width <= 0)
            width = d->m_region.texture->width() * (d->m_region.u2 - d->m_region.u1);

    d->m_width = width;
    markToUpdate();
    unlock();
}


void Sprite::setTextureScaling(TextureScaling ts)
{
    lock();
    RANGERS_D(Sprite);
    d->m_scaling = ts;
    markToUpdate();
    unlock();
}


void Sprite::processMain()
{
    Object::processMain();

    RANGERS_D(Sprite);
    if (!d->m_region.texture)
        return;

    lock();
    if (!d->m_buffer)
    {
        d->m_vertices = new Vertex[4];
        d->m_vertexCount = 4;
        memset(d->m_vertices, 0, d->m_vertexCount * sizeof(Vertex));

        glGenBuffers(1, &d->m_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, d->m_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * d->m_vertexCount, d->m_vertices, GL_DYNAMIC_DRAW);
        delete d->m_vertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, d->m_buffer);
    d->m_vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    float u1 = 0, u2 = 0, v1 = 0, v2 = 0;

    switch (d->m_xOrigin)
    {
    case POSITION_X_CENTER:
        x1 = -d->m_width / 2.0f;
        x2 = d->m_width / 2.0f;
        break;
    case POSITION_X_LEFT:
        x1 = 0;
        x2 = d->m_width;
        break;
    case POSITION_X_RIGHT:
        x1 = -d->m_width;
        x2 = 0;
        break;
    }

    switch (d->m_yOrigin)
    {
    case POSITION_Y_CENTER:
        y1 = -d->m_height / 2.0f;
        y2 = d->m_height / 2.0f;
        break;
    case POSITION_Y_TOP:
        y1 = 0;
        y2 = d->m_height;
        break;
    case POSITION_Y_BOTTOM:
        y1 = -d->m_height;
        y2 = 0;
        break;
    }

    //FIXME: Scaling on texture region working incorrectly.
    //TODO: Keepaspect
    switch (d->m_scaling)
    {
    case TEXTURE_NO:
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u1 + d->m_width / ((d->m_region.u2 - d->m_region.u1) * d->m_region.texture->width());
        v2 = d->m_region.v1 + d->m_height / ((d->m_region.v2 - d->m_region.v1) * d->m_region.texture->height());
        break;
    case TEXTURE_NORMAL:
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u2;
        v2 = d->m_region.v2;
        break;
    case TEXTURE_KEEPASPECT:
        /*u1 = 0;
        v1 = 0;
        if (m_width / m_texture->width() * m_texture->height() > m_height)
        {
            u2 = m_texture->height() / m_texture->width();
            v2 = 1;
        }
        else
        {
            u2 = 1;
            v2 = m_texture->width() / m_texture->height();
        }*/
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u2;
        v2 = d->m_region.v2;
        break;
    case TEXTURE_KEEPASPECT_EXPANDING:
        /*u1 = 0;
        v1 = 0;
        if (m_width / m_texture->width() * m_texture->height() > m_height)
        {
            u2 = 1;
            v2 = m_height / m_texture->height();
        }
        else
        {
            u2 = m_width / m_texture->width();
            v2 = 1;
        }*/
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u2;
        v2 = d->m_region.v2;
        break;
    case TEXTURE_TILE_X:
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u1 + d->m_width / ((d->m_region.u2 - d->m_region.u1) * d->m_region.texture->width());
        v2 = d->m_region.v2;
        break;
    case TEXTURE_TILE_Y:
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u2;
        v2 = d->m_region.v1 + d->m_height / ((d->m_region.v2 - d->m_region.v1) * d->m_region.texture->height());
        break;
    case TEXTURE_TILE:
        u1 = d->m_region.u1;
        v1 = d->m_region.v1;
        u2 = d->m_region.u1 + d->m_width / ((d->m_region.u2 - d->m_region.u1) * d->m_region.texture->width());
        v2 = d->m_region.v1 + d->m_height / ((d->m_region.v2 - d->m_region.v1) * d->m_region.texture->height());
        break;
    }

    d->m_vertices[0].x = x1;
    d->m_vertices[0].y = y1;
    d->m_vertices[0].u = u1;
    d->m_vertices[0].v = v1;

    d->m_vertices[1].x = x2;
    d->m_vertices[1].y = y1;
    d->m_vertices[1].u = u2;
    d->m_vertices[1].v = v1;

    d->m_vertices[2].x = x2;
    d->m_vertices[2].y = y2;
    d->m_vertices[2].u = u2;
    d->m_vertices[2].v = v2;

    d->m_vertices[3].x = x1;
    d->m_vertices[3].y = y2;
    d->m_vertices[3].u = u1;
    d->m_vertices[3].v = v2;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    unlock();
}

float Sprite::height() const
{
    RANGERS_D(const Sprite);
    return d->m_height;
}

float Sprite::width() const
{
    RANGERS_D(const Sprite);
    return d->m_width;
}

TextureRegion Sprite::region() const
{
    RANGERS_D(const Sprite);
    return d->m_region;
}
}
