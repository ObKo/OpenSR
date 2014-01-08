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

#include "OpenSR/ColorLabel.h"
#include "OpenSR/Font.h"
#include "OpenSR/Texture.h"
#include "OpenSR/Engine.h"

#include "OpenSR/private/ColorLabel_p.h"

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
ColorLabel::ColorLabel(): Label(*(new ColorLabelPrivate()))
{

}

/*!
 * \param text color text
 * \param parent parent object
 * \param font label font
 * \param xpos sprite x origin
 * \param ypos sprite y origin
 */
ColorLabel::ColorLabel(const std::string& text, boost::shared_ptr< Font > font): Label(*(new ColorLabelPrivate()))
{
    RANGERS_D(ColorLabel);
    if (!font)
        d->font = Engine::instance().coreFont();
    else
        d->font = font;

    setText(text);
    d->wordWrap = false;
    d->fixedWidth = false;
    d->fixedHeight = false;
    d->scaling = TEXTURE_NO;
}

/*!
 * \param text color text
 * \param parent parent object
 * \param font label font
 * \param xpos sprite x origin
 * \param ypos sprite y origin
 */
ColorLabel::ColorLabel(const std::wstring& text, boost::shared_ptr< Font > font): Label(*(new ColorLabelPrivate()))
{
    RANGERS_D(ColorLabel);
    if (!font)
        d->font = Engine::instance().coreFont();
    else
        d->font = font;

    setText(text);
    d->wordWrap = false;
    d->fixedWidth = false;
    d->fixedHeight = false;
    d->scaling = TEXTURE_NO;
}

ColorLabel::ColorLabel(ColorLabelPrivate &p): Label(p)
{
}

void ColorLabel::processMain()
{
    lock();
    RANGERS_D(ColorLabel);

    if (!d->wordWrap)
        d->region = TextureRegion(d->font->renderColoredText(d->text, d->color.toRGB()));
    else
        d->region = TextureRegion(d->font->renderColoredText(d->text, d->color.toRGB(), d->width));

    if (!d->fixedWidth)
        d->width = d->region.texture->width();
    if (!d->fixedHeight)
        d->height = d->region.texture->height();
    unlock();
    Sprite::processMain();
}

void ColorLabel::setColor(const Color& color)
{
    markToUpdate();
    Object::setColor(color);
}
}

