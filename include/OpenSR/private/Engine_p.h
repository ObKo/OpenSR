/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include "global.h"
#include "ConsoleWidget.h"
#include "Node.h"
#include "Label.h"
#include "Styles.h"
#include <list>
#include <map>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/property_tree/ptree.hpp>

struct lua_State;

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
       
    int m_argc;
    char **m_argv;
    int m_height;
    int m_width;
    int m_fpsTime;

    boost::recursive_mutex m_updateMutex;
    boost::thread *m_logicThread;

    boost::shared_ptr<boost::property_tree::ptree> m_properties;
    std::wstring m_configPath;
    std::wstring m_mainDataDir;

    int m_exitCode;
    bool m_gameRunning;
    bool m_consoleOpenned;
    bool m_showFPS;
    long m_frames;

    std::list<Object *> m_updateList;

    std::list<Widget *> m_widgets;
    std::list<Widget *> m_widgetsToDelete;
    Widget *m_currentWidget;
    Widget *m_focusedWidget;

    std::list<Plugin *> m_plugins;

    Node m_mainNode;
    Label m_fpsLabel;
    ConsoleWidget m_consoleWidget;
    boost::shared_ptr<Font> m_coreFont;
    boost::shared_ptr<Font> m_monospaceFont;

    lua_State *m_luaConsoleState;
    Skin m_skin;
    
    Engine *m_q;
    
    void processEvents();
    void processMouseMove(const SDL_MouseMotionEvent& e);
};
}

#endif
