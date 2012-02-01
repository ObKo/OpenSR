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

#ifndef RANGERS_BUTTON_H
#define RANGERS_BUTTON_H

#include "Widget.h"
#include "Sprite.h"
#include <boost/shared_ptr.hpp>
#include "Styles.h"

namespace Rangers
{
class Button: public Widget
{
public:
    Button(Widget *parent = 0);
    Button(const Button& other);
    Button(boost::shared_ptr<Texture> texture, Widget *parent = 0);
    Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, Widget *parent = 0);
    Button(boost::shared_ptr<Texture> texture, boost::shared_ptr<Texture> hoverTexture, boost::shared_ptr<Texture> pressTexture, Widget *parent = 0);

    Button(const std::wstring& texture, Widget *parent = 0);
    Button(const std::wstring& texture, const std::wstring& hoverTexture, Widget *parent = 0);
    Button(const std::wstring& texture, const std::wstring& hoverTexture, const std::wstring& pressTexture, Widget *parent = 0);

    Button(const ButtonStyle& style, Widget *parent = 0);

    virtual ~Button();

    virtual void draw() const;
    virtual void processMain();

    virtual void mouseEnter();
    virtual void mouseLeave();

    virtual void mouseDown(uint8_t key, const Vector &p);
    virtual void mouseUp(uint8_t key, const Vector &p);
    virtual void mouseClick(const Vector &p);

    Button& operator=(const Button& other);

private:
    ButtonStyle m_style;
    Sprite *m_normalSprite;
    Sprite *m_hoverSprite;
    Sprite *m_pressedSprite;
    Sprite *m_sprite;

    void init();
};
}

#endif
