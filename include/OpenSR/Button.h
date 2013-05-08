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

#ifndef RANGERS_BUTTON_H
#define RANGERS_BUTTON_H

#include "Widget.h"

namespace Rangers
{
class ButtonPrivate;
class Font;
class Sound;
struct ButtonStyle;

class RANGERS_ENGINE_API Button: public Widget
{
    RANGERS_DECLARE_PRIVATE(Button);
public:
    //TODO: Make consrtuctors more consistent
    Button(Widget *parent = 0);
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

    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual int preferredWidth() const;
    virtual int preferredHeight() const;

    void setText(const std::wstring& text);
    virtual void setColor(uint32_t color);
    void setFont(boost::shared_ptr<Font> font);
    void setAutoResize(bool autoResize);
    void setSounds(boost::shared_ptr<Sound> clickSound, boost::shared_ptr<Sound> leaveSound, boost::shared_ptr<Sound> enterSound);
    void setSounds(const std::wstring& clickSound, const std::wstring& leaveSound, const std::wstring& enterSound);

    std::wstring text() const;
    virtual uint32_t color() const;
    boost::shared_ptr<Font> font() const;
    bool autoResize() const;

protected:
    Button(ButtonPrivate &p, Widget *parent = 0);

    RANGERS_DISABLE_COPY(Button)
};
}

#endif
