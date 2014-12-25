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

#include "OpenSR/Action.h"
#include "OpenSR/Widget.h"
#include <boost/variant.hpp>

/*!
 * \var Action::Type Action::BUTTON_CLICKED
 * UI button clicked. Used, for example, in \ref Button class.
 * \var Action::Type Action::KEY_PRESSED
 * Keyboard key pressed. \ref Argument is \ref SDL_keysym
 * \var Action::Type Action::CHECKBOX_TOGGLED
 * Checkbox toogled. \ref Argument is \ref bool, represents new checkbox state.
 */

namespace Rangers
{
Action::Action(): m_type(NONE)
{
}

/*!
 * \param source action source widget
 * \param type action type
 * \param arg action argument
 */
Action::Action(boost::shared_ptr<Widget> source, uint32_t type, const Action::Argument& arg): m_source(source), m_type(type), m_argument(arg)
{
}

/*!
 * \returns source widget
 */
boost::shared_ptr<Widget> Action::source() const
{
    return m_source;
}

/*!
 * \returns action type
 */
uint32_t Action::type() const
{
    return m_type;
}

/*!
 * \returns action argument
 */
Action::Argument Action::argument() const
{
    return m_argument;
}
}
