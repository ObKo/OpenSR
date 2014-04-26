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

#include <OpenSR/libRanger.h>
#include <OpenSR/ResourceManager.h>

#include <libintl.h>
#include <string>

namespace Rangers
{
std::string _(const std::string& text, const std::string& datName, const std::string& gettextDomain)
{
    char *gt;
    if (gettextDomain.empty())
        gt = gettext(text.c_str());
    else
        gt = dgettext(gettextDomain.c_str(), text.c_str());

    if (text != gt)
        return text;

    boost::shared_ptr<DATRecord> root = ResourceManager::instance().datRoot();
    std::vector<std::string> path = split(datName, L'.');

    const DATRecord* current = root.get();
    for (const std::string& id : path)
    {
        DATRecord::const_iterator i = current->find(id);
        if (i == current->end())
            return text;

        current = i.operator->();
    }

    return current->value;
}
}