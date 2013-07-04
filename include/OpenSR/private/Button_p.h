/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_BUTTON_P_H
#define RANGERS_BUTTON_P_H

#include "Widget_p.h"

#include "OpenSR/Styles.h"
#include "OpenSR/ActionListener.h"

namespace Rangers
{
class Label;
class ButtonPrivate: public WidgetPrivate
{
    RANGERS_DECLARE_PUBLIC(Button)
public:
    class ButtonListener: public ActionListener
    {
    public:
        void actionPerformed(const Action &action);
    };

    ButtonPrivate();

    void calcAutoResize();
    void initFromStyle();

    ButtonStyle style;
    boost::shared_ptr<Label> label;
    std::wstring text;
    boost::shared_ptr<Sprite> normalSprite;
    boost::shared_ptr<Sprite> hoverSprite;
    boost::shared_ptr<Sprite> pressedSprite;
    boost::shared_ptr<Sprite> sprite;
    bool autoResize;

    boost::shared_ptr<Sound> enterSound;
    boost::shared_ptr<Sound> leaveSound;
    boost::shared_ptr<Sound> clickSound;

    boost::shared_ptr<ButtonListener> buttonListener;
};
}

#endif