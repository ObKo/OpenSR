/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/Object.h"

namespace Rangers
{
enum TextureScaling {TEXTURE_NO, TEXTURE_NORMAL, TEXTURE_KEEPASPECT, TEXTURE_KEEPASPECT_EXPANDING,
                     TEXTURE_TILE_X, TEXTURE_TILE_Y, TEXTURE_TILE
                    };
enum SpriteXOrigin {POSITION_X_LEFT, POSITION_X_RIGHT, POSITION_X_CENTER};
enum SpriteYOrigin {POSITION_Y_TOP, POSITION_Y_BOTTOM, POSITION_Y_CENTER};

class Texture;

class SpritePrivate;
class RANGERS_ENGINE_API Sprite: public Object
{
    RANGERS_DECLARE_PRIVATE(Sprite)
public:
    Sprite();
    Sprite(boost::shared_ptr<Texture> texture);
    Sprite(const std::wstring& texture);
    Sprite(const TextureRegion& region);
    Sprite(boost::shared_ptr<TextureRegionDescriptor> region);

    virtual ~Sprite();

    virtual void draw() const;
    virtual void processMain();

    void setOrigin(SpriteXOrigin xpos, SpriteYOrigin ypos);
    void setGeometry(float width, float height);
    void setWidth(float width);
    void setHeight(float height);
    void setTextureScaling(TextureScaling  ts);
    void setTexture(const std::wstring& texture);
    void setTexture(boost::shared_ptr<Texture> texture);
    void setTexture(const TextureRegion& texture);

    float width() const;
    float height() const;
    TextureScaling textureScaling() const;
    virtual float normalWidth() const;
    virtual float normalHeight() const;
    TextureRegion texture() const;
    SpriteXOrigin xOrigin() const;
    SpriteYOrigin yOrigin() const;

protected:
    Sprite(SpritePrivate &p);

    RANGERS_DISABLE_COPY(Sprite)
};
}

#endif
