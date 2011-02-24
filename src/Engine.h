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

#include "Object.h"
#include "Node.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Label.h"
#include "Widget.h"
#include <list>
#include "ConsoleWidget.h"
#include <map>

namespace Rangers
{
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
    boost::shared_ptr<Font> defaultFont() const;
    //! Default engine service font (e.g. console font)
    boost::shared_ptr<Font> serviceFont() const;

    //! Engine instance
    static Engine *instance();

    //! Engine fps counter loop
    static int fpsCounter(void *data);
    //! Logic loop
    static int logic(void *data);
    
    //! Get object by name
    Object* getObject(const std::wstring& name);
    //! Remove object.
    void removeObject(const std::wstring& name);
    //! Add object. Engine becomes owner of object.
    std::wstring addObject(Object* object, const std::wstring& name = std::wstring());

private:
    int argc;
    char **argv;
    int height;
    int width;

    SDL_mutex *updateMutex, *logicMutex;
    SDL_Thread *fpsThread, *logicThread;

    int exitCode;
    bool gameRunning;
    bool consoleOpenned;
    bool showFPS;
    long frames;
    
    std::map<std::wstring, Object*> objects;

    void processEvents();
    void processMouseMove(SDL_MouseMotionEvent e);

    std::list<Object *> updateList;

    std::list<Widget *> widgets;
    Widget *currentWidget;
    Widget *focusedWidget;

    Node mainNode;
    Label fpsLabel;
    ConsoleWidget consoleWidget;
    boost::shared_ptr<Font> engineFont;
    boost::shared_ptr<Font> monospaceFont;
};
};

#endif
