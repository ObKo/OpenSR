/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 Kosyak <ObKo@mail.ru>

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
#include "Engine.h"
#include "ResourceManager.h"

namespace Rangers
{
Sprite::Sprite(Object *parent): Object(parent)
{
    m_buffer = 0;
    m_vertices = 0;
    m_vertexCount = 0;
    m_width = 0;
    m_height = 0;
    m_xOrigin = POSITION_X_LEFT;
    m_yOrigin = POSITION_Y_TOP;
    m_scaling = TEXTURE_NORMAL;
}

Sprite::Sprite(const Rangers::Sprite& other): Object(other)
{
    m_buffer = 0;
    m_vertices = 0;
    m_vertexCount = 0;

    m_width = other.m_width;
    m_height = other.m_height;
    m_xOrigin = other.m_xOrigin;
    m_yOrigin = other.m_yOrigin;
    m_scaling = other.m_scaling;
    m_texture = other.m_texture;

    markToUpdate();
}


Sprite::Sprite(boost::shared_ptr<Texture> texture,  Object *parent, TextureScaling  ts, SpriteXOrigin xpos, SpriteYOrigin ypos): Object(parent)
{
    m_buffer = 0;
    m_vertices = 0;
    m_vertexCount = 0;
    m_texture = texture;
    m_scaling = ts;
    if (!texture)
    {
        m_width = 0;
        m_height = 0;
    }
    else
    {
        m_width = texture->width();
        m_height = texture->height();
    }
    m_xOrigin = xpos;
    m_yOrigin = ypos;
    markToUpdate();
}

Sprite::Sprite(const std::wstring& texture,  Object *parent, TextureScaling  ts, SpriteXOrigin xpos, SpriteYOrigin ypos): Object(parent)
{
    m_buffer = 0;
    m_vertices = 0;
    m_vertexCount = 0;
    m_scaling = ts;
    m_texture = ResourceManager::instance()->loadTexture(texture);
    if (!m_texture)
    {
        m_width = 0;
        m_height = 0;
    }
    else
    {
        m_width = m_texture->width();
        m_height = m_texture->height();
    }
    m_xOrigin = xpos;
    m_yOrigin = ypos;
    markToUpdate();
}

Sprite::~Sprite()
{
    if (m_buffer)
        glDeleteBuffers(1, &m_buffer);
}

Sprite& Sprite::operator=(const Rangers::Sprite& other)
{
    if (this == &other)
        return *this;

    m_buffer = 0;
    m_vertices = 0;
    m_vertexCount = 0;

    m_width = other.m_width;
    m_height = other.m_height;
    m_xOrigin = other.m_xOrigin;
    m_yOrigin = other.m_yOrigin;
    m_scaling = other.m_scaling;
    m_texture = other.m_texture;

    markToUpdate();
    Object::operator=(other);
    return *this;
}


void Sprite::draw() const
{
    if (!m_texture)
        return;

    if (!prepareDraw())
        return;

    glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());

    if (m_scaling == TEXTURE_TILE_X)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    if (m_scaling == TEXTURE_TILE_Y)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    if (m_scaling == TEXTURE_TILE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, m_vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
}

void Sprite::setOrigin(SpriteXOrigin xpos, SpriteYOrigin ypos)
{
    lock();
    m_xOrigin = xpos;
    m_yOrigin = ypos;
    markToUpdate();
    unlock();
}

void Sprite::setTexture(boost::shared_ptr<Texture> texture)
{
    lock();
    m_texture = texture;
    unlock();
}

void Sprite::setGeometry(float width, float height)
{
    lock();
    if (m_texture)
    {
        if (width <= 0)
            width = m_texture->width();
        if (height <= 0)
            height = m_texture->height();
    }

    m_width = width;
    m_height = height;
    markToUpdate();
    unlock();
}

void Sprite::setHeight(float height)
{
    lock();
    if (m_texture)
        if (height <= 0)
            height = m_texture->height();

    m_height = height;
    markToUpdate();
    unlock();
}

void Sprite::setWidth(float width)
{
    lock();
    if (m_texture)
        if (width <= 0)
            width = m_texture->width();

    m_width = width;
    markToUpdate();
    unlock();
}


void Sprite::setTextureScaling(TextureScaling ts)
{
    lock();
    m_scaling = ts;
    markToUpdate();
    unlock();
}


void Sprite::processMain()
{
    Object::processMain();

    if (!m_texture)
        return;

    lock();
    if (!m_buffer)
    {
        m_vertices = new Vertex[4];
        m_vertexCount = 4;
        memset(m_vertices, 0, m_vertexCount * sizeof(Vertex));

        glGenBuffers(1, &m_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, m_vertices, GL_DYNAMIC_DRAW);
        delete m_vertices;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    m_vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    float u1 = 0, u2 = 0, w1 = 0, w2 = 0;

    switch (m_xOrigin)
    {
    case POSITION_X_CENTER:
        x1 = -m_width / 2.0f;
        x2 = m_width / 2.0f;
        break;
    case POSITION_X_LEFT:
        x1 = 0;
        x2 = m_width;
        break;
    case POSITION_X_RIGHT:
        x1 = -m_width;
        x2 = 0;
        break;
    }

    switch (m_yOrigin)
    {
    case POSITION_Y_CENTER:
        y1 = -m_height / 2.0f;
        y2 = m_height / 2.0f;
        break;
    case POSITION_Y_TOP:
        y1 = 0;
        y2 = m_height;
        break;
    case POSITION_Y_BOTTOM:
        y1 = -m_height;
        y2 = 0;
        break;
    }

    switch (m_scaling)
    {
    case TEXTURE_NO:
        u1 = 0;
        w1 = 0;
        u2 = m_width / m_texture->width();
        w2 = m_height / m_texture->height();
        break;
    case TEXTURE_NORMAL:
        u1 = 0;
        w1 = 0;
        u2 = 1;
        w2 = 1;
        break;
    case TEXTURE_KEEPASPECT:
        u1 = 0;
        w1 = 0;
        if (m_width / m_texture->width() * m_texture->height() > m_height)
        {
            u2 = m_texture->height() / m_texture->width();
            w2 = 1;
        }
        else
        {
            u2 = 1;
            w2 = m_texture->width() / m_texture->height();
        }
        break;
    case TEXTURE_KEEPASPECT_EXPANDING:
        u1 = 0;
        w1 = 0;
        if (m_width / m_texture->width() * m_texture->height() > m_height)
        {
            u2 = 1;
            w2 = m_height / m_texture->height();
        }
        else
        {
            u2 = m_width / m_texture->width();
            w2 = 1;
        }
        break;
    case TEXTURE_TILE_X:
        u1 = 0;
        w1 = 0;
        u2 = m_width / m_texture->width();
        w2 = 1;
        break;
    case TEXTURE_TILE_Y:
        u1 = 0;
        w1 = 0;
        u2 = 1;
        w2 = m_height / m_texture->height();
        break;
    case TEXTURE_TILE:
        u1 = 0;
        w1 = 0;
        u2 = m_width / m_texture->width();
        w2 = m_height / m_texture->height();
        break;
    }

    m_vertices[0].x = x1;
    m_vertices[0].y = y1;
    m_vertices[0].u = u1;
    m_vertices[0].w = w1;

    m_vertices[1].x = x2;
    m_vertices[1].y = y1;
    m_vertices[1].u = u2;
    m_vertices[1].w = w1;

    m_vertices[2].x = x2;
    m_vertices[2].y = y2;
    m_vertices[2].u = u2;
    m_vertices[2].w = w2;

    m_vertices[3].x = x1;
    m_vertices[3].y = y2;
    m_vertices[3].u = u1;
    m_vertices[3].w = w2;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    unlock();
}

float Sprite::height() const
{
    return m_height;
}

float Sprite::width() const
{
    return m_width;
}

boost::shared_ptr< Texture > Sprite::texture() const
{
    return m_texture;
}
}
