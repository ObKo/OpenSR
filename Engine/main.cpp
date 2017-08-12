/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 - 2017 Kosyak <ObKo@mail.ru>

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

#include <OpenSR/Engine.h>

namespace
{
static const QString SETTINGS_ORGANIZATION = "OpenSR";
static const QString SETTINGS_APPLICATION = "OpenSR";
static const QString STARTUP_SCRIPT = "res:/opensr.js";
static const QString MAIN_QML = "res:/OpenSR/GameWindow.qml";
}

int main(int argc, char **argv)
{
    OpenSR::Engine engine(argc, argv);

    QApplication::setOrganizationName(SETTINGS_ORGANIZATION);
    QApplication::setApplicationName(SETTINGS_APPLICATION);

    engine.setStartupScript(STARTUP_SCRIPT);
    engine.setMainQML(MAIN_QML);

    return engine.run();
}
