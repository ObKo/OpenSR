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

#ifndef RANGERS_COLORLABEL_H
#define RANGERS_COLORLABEL_H

#include "OpenSR/Label.h"

namespace Rangers
{
class ColorLabelPrivate;
//! Color text label
class RANGERS_ENGINE_API ColorLabel: public Label
{
    RANGERS_DECLARE_PRIVATE(ColorLabel)
public:
    //! Default constructor
    ColorLabel();
    //! Construct from std::string
    ColorLabel(const std::string& text, boost::shared_ptr<Font> font = boost::shared_ptr<Font>());
    //! Construct from std::wstring
    ColorLabel(const std::wstring& text, boost::shared_ptr<Font> font = boost::shared_ptr<Font>());

    virtual void processMain();
    //! Set default text color
    virtual void setColor(const Color& color);

protected:
    ColorLabel(ColorLabelPrivate &p);

    RANGERS_DISABLE_COPY(ColorLabel)
};
}

#endif // COLORLABEL_H
