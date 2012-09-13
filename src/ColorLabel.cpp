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

#include "ColorLabel.h"
#include "Font.h"
#include "Texture.h"
#include "Engine.h"

#include "private/ColorLabel_p.h"

using namespace std;
namespace Rangers
{
/*!
 * This class is used to draw text with different color
 * Color in text is defined as \\cXXXXXX - where XXXXXX - RGB color in hex format
 * \\cR resets color to default sprite color
 */

/*!
 * \param parent parent object
 */
ColorLabel::ColorLabel(Object* parent): Label(*(new ColorLabelPrivate()), parent)
{

}

/*!
 * \param text color text
 * \param parent parent object
 * \param font label font
 * \param xpos sprite x origin
 * \param ypos sprite y origin
 */
ColorLabel::ColorLabel(const std::string& text, Object* parent, boost::shared_ptr< Font > font, SpriteXOrigin xpos, SpriteYOrigin ypos): Label(*(new ColorLabelPrivate()), parent)
{
    RANGERS_D(ColorLabel);
    if (!font)
        d->m_font = Engine::instance()->coreFont();
    else
        d->m_font = font;

    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    setText(text);
    d->m_wordWrap = false;
    d->m_fixedSize = false;
    d->m_scaling = TEXTURE_NO;
}

/*!
 * \param text color text
 * \param parent parent object
 * \param font label font
 * \param xpos sprite x origin
 * \param ypos sprite y origin
 */
ColorLabel::ColorLabel(const std::wstring& text, Object* parent, boost::shared_ptr< Font > font, SpriteXOrigin xpos, SpriteYOrigin ypos): Label(*(new ColorLabelPrivate()), parent)
{
    RANGERS_D(ColorLabel);
    if (!font)
        d->m_font = Engine::instance()->coreFont();
    else
        d->m_font = font;

    d->m_xOrigin = xpos;
    d->m_yOrigin = ypos;
    setText(text);
    d->m_wordWrap = false;
    d->m_fixedSize = false;
    d->m_scaling = TEXTURE_NO;
}

ColorLabel::ColorLabel(ColorLabelPrivate &p, Object *parent): Label(p, parent)
{
}

void ColorLabel::processMain()
{
    lock();
    RANGERS_D(ColorLabel);

    if (!d->m_wordWrap)
        d->m_region = TextureRegion(d->m_font->renderColoredText(d->m_text, (d->m_color >> 8) & 0xffffff));
    else
        d->m_region = TextureRegion(d->m_font->renderColoredText(d->m_text, (d->m_color >> 8) & 0xffffff));

    if (!d->m_fixedSize)
    {
        d->m_width = d->m_region.texture->width();
        d->m_height = d->m_region.texture->height();
    }
    unlock();
    Sprite::processMain();
}

void ColorLabel::setColor(int color)
{
    markToUpdate();
    Object::setColor(color);
}
}

