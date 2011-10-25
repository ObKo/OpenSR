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

#ifndef RANGERS_ENGINE_H
#define RANGERS_ENGINE_H

#include "Node.h"
#include "Label.h"
#include <list>
#include "ConsoleWidget.h"
#include "Texture.h"
#include <map>
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Rangers
{
class Widget;
class Object;
//! Main engine class
class Engine
{
public:
    //! Construct engine from main's args
    Engine(int argc, char **argv);
    ~Engine();

    //! Init engine
    void init(int width, int height, bool fullscreen = false);
    //! Main paint function
    void paint();
    //! End loop
    void quit(int code = 0);
    //! Main loop
    int run();
    //! Exec command (e.g. from console)
    void execCommand(const std::wstring& what);

    //! Mark object been updated in main (OpenGL) thread
    void markToUpdate(Object* object);
    //! Disable object been updated in main (OpenGL) thread
    void unmarkToUpdate(Object* object);
    //! Add widget for mouse input handling
    void addWidget(Widget *w);
    //! Focus widget for key input handling
    void focusWidget(Widget *w);

    //! Current screen height
    int screenHeight() const;
    //! Current screen width
    int screenWidth() const;

    //! Default engine font
    boost::shared_ptr<Font> coreFont() const;
    //! Default engine service font (e.g. console font)
    boost::shared_ptr<Font> serviceFont() const;

    //! Engine instance
    static Engine *instance();

    //! Current ticks
    static long long getTicks();

    //! Engine fps counter loop
    static int fpsCounter();
    //! Logic loop
    static int logic();

    //! Global program options
    boost::shared_ptr<boost::property_tree::ptree> properties() const;

    //! Get object by name
    Object* getObject(const std::wstring& name) const;
    //! Remove object.
    void removeObject(const std::wstring& name);
    //! Add object. Engine becomes owner of object.
    std::wstring addObject(Object* object, const std::wstring& name = std::wstring());

    //! Get current OpenGL texture format
    GLint textureInternalFormat(TextureType t) const;

private:
    int m_argc;
    char **m_argv;
    int m_height;
    int m_width;

    boost::recursive_mutex m_updateMutex;
    boost::thread *m_fpsThread, *m_logicThread;

    boost::shared_ptr<boost::property_tree::ptree> m_properties;
    std::wstring m_configPath;
    std::wstring m_mainDataDir;

    int m_exitCode;
    bool m_gameRunning;
    bool m_consoleOpenned;
    bool m_showFPS;
    long m_frames;

    std::map<std::wstring, Object*> m_objects;

    void processEvents();
    void processMouseMove(SDL_MouseMotionEvent e);

    std::list<Object *> m_updateList;

    std::list<Widget *> m_widgets;
    Widget *m_currentWidget;
    Widget *m_focusedWidget;

    Node m_mainNode;
    Label m_fpsLabel;
    ConsoleWidget m_consoleWidget;
    boost::shared_ptr<Font> m_coreFont;
    boost::shared_ptr<Font> m_monospaceFont;
};
}

#endif
