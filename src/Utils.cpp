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

#include "OpenSR/Utils.h"

#include <libRanger.h>
#include <libintl.h>
#include <string>

namespace Rangers
{
std::wstring _(const std::string& text, const std::string& domain)
{
    return fromLocal(dgettext(domain.c_str(), text.c_str()));
}

std::wstring _(const std::string& text)
{
    return fromLocal(gettext(text.c_str()));
}
}