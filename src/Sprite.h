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

#ifndef RANGERS_SPRITE_H
#define RANGERS_SPRITE_H

#include "Object.h"
#include "Types.h"
#include "Texture.h"

namespace Rangers
{
enum TextureScaling {TEXTURE_NO, TEXTURE_NORMAL, TEXTURE_KEEPASPECT, TEXTURE_TILE_X, TEXTURE_TILE_Y, TEXTURE_TILE};
enum SpriteXPosition {POSITION_X_LEFT, POSITION_X_RIGHT, POSITION_X_CENTER};
enum SpriteYPosition {POSITION_Y_TOP, POSITION_Y_BOTTOM, POSITION_Y_CENTER};
class Sprite: public Object
{
public:
    Sprite(Object *parent = 0);
    Sprite(boost::shared_ptr<Texture> texture, Object *parent = 0, TextureScaling  ts = TEXTURE_NORMAL, SpriteXPosition xpos = POSITION_X_LEFT, SpriteYPosition ypos = POSITION_Y_TOP);
    Sprite(const Sprite& other);
    
    ~Sprite();

    void draw() const;

    void processMain();

    void setSpriteOrigin(SpriteXPosition xpos, SpriteYPosition ypos);
    void setGeometry(float width, float height);
    void setWidth(float width);
    void setHeight(float height);
    void setTextureScaling(TextureScaling  ts);
    
    float width() const;
    float height() const;
    
    Sprite& operator=(const Sprite& other);

protected:
    Vertex *vertex;
    int vertexCount;

    float spriteWidth, spriteHeight;
    SpriteXPosition xPos;
    SpriteYPosition yPos;
    TextureScaling textureScaling;

    GLuint vertexBuffer;
    boost::shared_ptr<Texture> spriteTexture;
};
};

#endif
