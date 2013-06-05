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

#ifndef RANGERS_ENGINE_P_H
#define RANGERS_ENGINE_P_H

#include <list>
#include <map>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/property_tree/ptree.hpp>

#include "OpenSR/global.h"
#include "OpenSR/ConsoleWidget.h"
#include "OpenSR/Node.h"
#include "OpenSR/Label.h"
#include "OpenSR/Styles.h"

namespace Rangers
{
class Engine;
class Object;
class Widget;
class Plugin;
class Font;
class Engine::EnginePrivate
{
    RANGERS_DECLARE_PUBLIC(Engine)
public:
    EnginePrivate();
    virtual ~EnginePrivate();

    int argc;
    char **argv;
    int height;
    int width;
    int fpsTime;

    boost::recursive_mutex updateMutex;
    //boost::thread *logicThread;

    boost::shared_ptr<boost::property_tree::ptree> properties;
    std::wstring configPath;
    std::wstring mainDataDir;

    int exitCode;
    bool gameRunning;
    bool consoleOpenned;
    bool showFPS;
    long frames;

    std::list<Object *> updateList;

    std::list<boost::shared_ptr<Widget> > widgets;
    boost::weak_ptr<Widget> currentWidget;
    boost::weak_ptr<Widget> focusedWidget;

    std::list<Plugin *> plugins;

    Node *mainNode;
    Label *fpsLabel;
    boost::shared_ptr<Widget> consoleWidget;
    boost::shared_ptr<Font> coreFont;
    boost::shared_ptr<Font> monospaceFont;

    Skin skin;

    SDL_Window *window;

    Engine *m_q;

    void processEvents();
    void processMouseMove(const SDL_MouseMotionEvent& e);
    void processLogic();
};
}

#endif
