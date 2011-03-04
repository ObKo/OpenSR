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

using namespace Rangers;

Sprite::Sprite(Object *parent): Object(parent)
{
    vertexBuffer = 0;
    vertex = 0;
    vertexCount = 0;
    spriteWidth = 0;
    spriteHeight = 0;
    xPos = POSITION_X_CENTER;
    yPos = POSITION_Y_CENTER;
    textureScaling = TEXTURE_NORMAL;
}

Sprite::Sprite(const Rangers::Sprite& other): Object(other)
{
    vertexBuffer = 0;
    vertex = 0;
    vertexCount = 0;

    spriteWidth = other.spriteWidth;
    spriteHeight = other.spriteHeight;
    xPos = other.xPos;
    yPos = other.yPos;
    textureScaling = other.textureScaling;
    spriteTexture = other.spriteTexture;
    
    markToUpdate();
}


Sprite::Sprite(boost::shared_ptr<Texture> texture,  Object *parent, TextureScaling  ts, SpriteXPosition xpos, SpriteYPosition ypos): Object(parent)
{
    vertexBuffer = 0;
    vertex = 0;
    vertexCount = 0;
    spriteTexture = texture;
    textureScaling = ts;
    spriteWidth = texture->width();
    spriteHeight = texture->height();
    xPos = xpos;
    yPos = ypos;
    markToUpdate();
}

Sprite::~Sprite()
{
    glDeleteBuffers(1, &vertexBuffer);
}

Sprite& Sprite::operator=(const Rangers::Sprite& other)
{
    if (this == &other)
        return *this;
    
    vertexBuffer = 0;
    vertex = 0;
    vertexCount = 0;

    spriteWidth = other.spriteWidth;
    spriteHeight = other.spriteHeight;
    xPos = other.xPos;
    yPos = other.yPos;
    textureScaling = other.textureScaling;
    spriteTexture = other.spriteTexture;
    
    markToUpdate();
    ::Object::operator=(other);
    return *this;
}


void Sprite::draw() const
{
    if (!spriteTexture)
        return;
    SDL_mutexP(objectMutex);
    prepareDraw();

    glBindTexture(GL_TEXTURE_2D, spriteTexture->openGLTexture());
    
    if (textureScaling == TEXTURE_TILE_X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    if (textureScaling == TEXTURE_TILE_Y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    if (textureScaling == TEXTURE_TILE)
    {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }   

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float)*2));

    glDrawArrays(GL_QUADS, 0, vertexCount);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    endDraw();
    SDL_mutexV(objectMutex);
}

void Sprite::setSpriteOrigin(SpriteXPosition xpos, SpriteYPosition ypos)
{
    xPos = xpos;
    yPos = ypos;
    markToUpdate();
}

void Sprite::setGeometry(float width, float height)
{
    if (width <= 0)
        width = spriteTexture->width();
    if (height <= 0)
        height = spriteTexture->height();

    spriteWidth = width;
    spriteHeight = height;
    markToUpdate();
}

void Sprite::setHeight(float height)
{
    if (height <= 0)
        height = spriteTexture->height();

    spriteHeight = height;
    markToUpdate();
}

void Sprite::setWidth(float width)
{
     if (width <= 0)
        width = spriteTexture->width();
     
     spriteWidth = width;
     markToUpdate();
}


void Sprite::setTextureScaling(TextureScaling ts)
{
    textureScaling = ts;
    markToUpdate();
}


void Sprite::processMain()
{
    SDL_mutexP(objectMutex);
    ::Object::processMain();
    if (!vertexBuffer)
    {
        vertex = new Vertex[4];
        vertexCount = 4;
        memset(vertex, 0, vertexCount*sizeof(Vertex));

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertex, GL_DYNAMIC_DRAW);
        delete vertex;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    vertex = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    float u1 = 0, u2 = 0, w1 = 0, w2 = 0;

    switch (xPos)
    {
    case POSITION_X_CENTER:
        x1 = -spriteWidth / 2.0f;
        x2 = spriteWidth / 2.0f;
        break;
    case POSITION_X_LEFT:
        x1 = 0;
        x2 = spriteWidth;
        break;
    case POSITION_X_RIGHT:
        x1 = -spriteWidth;
        x2 = 0;
        break;
    }

    switch (yPos)
    {
    case POSITION_Y_CENTER:
        y1 = -spriteHeight / 2.0f;
        y2 = spriteHeight / 2.0f;
        break;
    case POSITION_Y_TOP:
        y1 = 0;
        y2 = spriteHeight;
        break;
    case POSITION_Y_BOTTOM:
        y1 = -spriteHeight;
        y2 = 0;
        break;
    }

    switch (textureScaling)
    {
    case TEXTURE_NO:
        u1 = 0;
        w1 = 0;
        u2 = spriteWidth / spriteTexture->width();
        w2 = spriteHeight / spriteTexture->height();
        break;
    case TEXTURE_NORMAL:
        u1 = 0;
        w1 = 0;
        u2 = 1;
        w2 = 1;
        break;
    case TEXTURE_KEEPASPECT:
        //TODO: keep aspect
        break;
    case TEXTURE_TILE_X:
        u1 = 0;
        w1 = 0;
        u2 = spriteWidth / spriteTexture->width();
        w2 = 1;
        break;
    case TEXTURE_TILE_Y:
        u1 = 0;
        w1 = 0;
        u2 = 1;
        w2 = spriteHeight / spriteTexture->height();
        break;
    case TEXTURE_TILE:
        u1 = 0;
        w1 = 0;
        u2 = spriteWidth / spriteTexture->width();
        w2 = spriteHeight / spriteTexture->height();
        break;
    }

    vertex[0].x = x1;
    vertex[0].y = y1;
    vertex[0].u = u1;
    vertex[0].w = w1;

    vertex[1].x = x2;
    vertex[1].y = y1;
    vertex[1].u = u2;
    vertex[1].w = w1;

    vertex[2].x = x2;
    vertex[2].y = y2;
    vertex[2].u = u2;
    vertex[2].w = w2;

    vertex[3].x = x1;
    vertex[3].y = y2;
    vertex[3].u = u1;
    vertex[3].w = w2;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    SDL_mutexV(objectMutex);
}

float Sprite::height() const
{
    return spriteHeight;
}

float Sprite::width() const
{
    return spriteWidth;
}

