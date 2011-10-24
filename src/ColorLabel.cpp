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
ColorLabel::ColorLabel(Object* parent): Label(parent)
{

}

/*!
 * \param text color text
 * \param parent parent object
 * \param font label font
 * \param xpos sprite x origin
 * \param ypos sprite y origin
 */
ColorLabel::ColorLabel(const std::string& text, Object* parent, boost::shared_ptr< Font > font, SpriteXOrigin xpos, SpriteYOrigin ypos): Label(text, parent, font, xpos, ypos)
{

}

/*!
 * \param text color text
 * \param parent parent object
 * \param font label font
 * \param xpos sprite x origin
 * \param ypos sprite y origin
 */
ColorLabel::ColorLabel(const std::wstring& text, Object* parent, boost::shared_ptr< Font > font, SpriteXOrigin xpos, SpriteYOrigin ypos): Label(text, parent, font, xpos, ypos)
{

}

void ColorLabel::processMain()
{
    lock();

    if (!m_wordWrap)
        m_texture = m_font->renderColoredText(m_text, (char(m_colorR * 255) << 16) | (char(m_colorG * 255) << 8) | (char(m_colorB * 255)));
    else
        m_texture = m_font->renderColoredText(m_text, (char(m_colorR * 255) << 16) | (char(m_colorG * 255) << 8) | (char(m_colorB * 255)), m_width);

    if (!m_fixedSize)
    {
        m_width = m_texture->width();
        m_height = m_texture->height();
    }
    unlock();
    Sprite::processMain();
}

void ColorLabel::setColor(float r, float g, float b, float a)
{
    markToUpdate();
    Object::setColor(r, g, b, a);
}
}

