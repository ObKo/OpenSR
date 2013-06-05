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

#include <boost/python.hpp>
#include <OpenSR/Sprite.h>
#include "Wrappers.h"

namespace Rangers
{
namespace Python
{
struct SpriteWrap : SpriteWrap_<Sprite>
{
    SpriteWrap(Object *parent = 0): SpriteWrap_<Sprite>(parent)
    {
    }

    SpriteWrap(boost::shared_ptr<Texture> texture, Object *parent = 0, TextureScaling  ts = TEXTURE_NORMAL,
               SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP)
        : SpriteWrap_<Sprite>(texture, parent, ts, xpos, ypos)
    {
    }
    SpriteWrap(const std::wstring& texture, Object *parent = 0, TextureScaling  ts = TEXTURE_NORMAL,
               SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP)
        : SpriteWrap_<Sprite>(texture, parent, ts, xpos, ypos)
    {
    }

    SpriteWrap(const TextureRegion& region, Object *parent = 0)
        : SpriteWrap_<Sprite>(region, parent)
    {
    }

    SpriteWrap(const TextureRegionDescriptor& region, Object *parent = 0)
        : SpriteWrap_<Sprite>(region, parent)
    {
    }
};

void exportSprite()
{
    using namespace boost::python;
    enum_<TextureScaling>("TextureScaling")
    .value("TEXTURE_NO", TEXTURE_NO)
    .value("TEXTURE_NORMAL", TEXTURE_NORMAL)
    .value("TEXTURE_KEEPASPECT", TEXTURE_KEEPASPECT)
    .value("TEXTURE_KEEPASPECT_EXPANDING", TEXTURE_KEEPASPECT_EXPANDING)
    .value("TEXTURE_TILE_X", TEXTURE_TILE_X)
    .value("TEXTURE_TILE_Y", TEXTURE_TILE_Y)
    .value("TEXTURE_TILE", TEXTURE_TILE);

    enum_<SpriteXOrigin>("SpriteXOrigin")
    .value("POSITION_X_LEFT", POSITION_X_LEFT)
    .value("POSITION_X_RIGHT", POSITION_X_RIGHT)
    .value("POSITION_X_CENTER", POSITION_X_CENTER);

    enum_<SpriteYOrigin>("SpriteYOrigin")
    .value("POSITION_Y_TOP", POSITION_Y_TOP)
    .value("POSITION_Y_BOTTOM", POSITION_Y_BOTTOM)
    .value("POSITION_Y_CENTER", POSITION_Y_CENTER);

    class_<SpriteWrap, bases<Object>, boost::noncopyable> c("Sprite", init<const std::wstring&, Object*>());
    c.def(init<const std::wstring&>())
    .def(init<const TextureRegion&, Object*>())
    .def(init<const TextureRegion&>())
    .def(init<const TextureRegionDescriptor&, Object*>())
    .def(init<const TextureRegionDescriptor&>())
    .def(init<Object*>())
    .def(init<>())
    .def("region", &Sprite::region)
    .def("setOrigin", &Sprite::setOrigin)
    .def("setGeometry", &Sprite::setGeometry)
    .def("setWidth", &Sprite::setWidth)
    .def("setHeight", &Sprite::setHeight)
    .def("setTextureScaling", &Sprite::setTextureScaling)
    .def("setTexture", &Sprite::setTexture)
    .def("width", &Sprite::width)
    .def("height", &Sprite::height);
    SpriteWrap::defWrapped(c);
}
}
}
/*
#include "Object.h"

namespace Rangers
{

class Texture;

class SpritePrivate;
class RANGERS_ENGINE_API Sprite: public Object
{
    RANGERS_DECLARE_PRIVATE(Sprite)
public:
    Sprite(Object *parent = 0);
    Sprite(boost::shared_ptr<Texture> texture, Object *parent = 0, TextureScaling  ts = TEXTURE_NORMAL, SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP);
    Sprite(const std::wstring& texture, Object *parent = 0, TextureScaling  ts = TEXTURE_NORMAL, SpriteXOrigin xpos = POSITION_X_LEFT, SpriteYOrigin ypos = POSITION_Y_TOP);
    Sprite(const TextureRegion& region, Object *parent = 0);
    Sprite(const TextureRegionDescriptor& region, Object *parent = 0);

    virtual ~Sprite();

    virtual void draw() const;
    virtual void processMain();

    void setOrigin(SpriteXOrigin xpos, SpriteYOrigin ypos);
    void setGeometry(float width, float height);
    void setWidth(float width);
    void setHeight(float height);
    void setTextureScaling(TextureScaling  ts);
    void setTexture(boost::shared_ptr<Texture> texture);

    float width() const;
    float height() const;

    virtual float normalWidth() const;
    virtual float normalHeight() const;

    TextureRegion region() const;

protected:
    Sprite(SpritePrivate &p, Object *parent = 0);

    RANGERS_DISABLE_COPY(Sprite)
};
}

#endif*/
