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

#ifndef RANGERS_LABEL_H
#define RANGERS_LABEL_H

#include "OpenSR/Sprite.h"

namespace Rangers
{
class Font;
class Object;
class LabelPrivate;
class RANGERS_ENGINE_API Label: public Sprite
{
    RANGERS_DECLARE_PRIVATE(Label)
public:
    Label();
    Label(const std::string& text, boost::shared_ptr<Font> font = boost::shared_ptr<Font>());
    Label(const std::wstring& text, boost::shared_ptr<Font> font = boost::shared_ptr<Font>());

    void setText(const std::string& text);
    void setText(const std::wstring& text);
    void setFont(boost::shared_ptr<Font> font);

    std::wstring text() const;
    boost::shared_ptr<Font> font() const;

    void setWordWrap(bool wordWrap);
    void setFixedSize(float w, float h);

    virtual void processMain();

protected:
    Label(LabelPrivate &p);

    RANGERS_DISABLE_COPY(Label)
};
}

#endif
