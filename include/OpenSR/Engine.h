/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/global.h"
#include "OpenSR/Styles.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Rangers
{
class Widget;
class Object;
class Plugin;
class Font;
class Node;
//! Main engine class
class RANGERS_ENGINE_API Engine
{
    class EnginePrivate;
    RANGERS_DECLARE_PRIVATE(Engine)

public:
    //! Init engine
    void init(int argc, char **argv, int width, int height, bool fullscreen = false);
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
    //! Add widget to top widgets
    void addWidget(boost::shared_ptr<Widget> w);
    //! Remove widget from top widgets
    void removeWidget(boost::shared_ptr<Widget> w);
    //! Focus widget for key input handling
    void focusWidget(boost::weak_ptr<Widget> w);
    //! Unfocus widget for key input handling
    void unfocusWidget(boost::weak_ptr<Widget> w);
    //! Widget will be hidden now.
    void widgetHide(Widget *w);
    //! Widget will be destroyed now.
    void widgetDestroyed(Widget *w);
    //! Try to get smart pointer of object
    boost::shared_ptr<Object> getObjectPointer(Object *object) const;

    //! Current screen height
    int screenHeight() const;
    //! Current screen width
    int screenWidth() const;

    //! Default engine font
    boost::shared_ptr<Font> coreFont() const;
    //! Default engine service font (e.g. console font)
    boost::shared_ptr<Font> serviceFont() const;

    //! Engine instance
    static Engine& instance();

    //! Current ticks
    static long long getTicks();

    //! Logic loop
    static int logic();

    //! Global program options
    boost::shared_ptr<boost::property_tree::ptree> properties() const;

    boost::shared_ptr<Node> rootNode();

    boost::shared_ptr<Skin> defaultSkin() const;
    void setDefaultSkin(boost::shared_ptr<Skin> skin);
    void setDefaultSkin(const std::string& skinObjectPath);

    void loadPlugin(const std::wstring &path);

private:
    Engine();
    Engine(const Engine& other);
    ~Engine();

    EnginePrivate *m_d;
};
}

#endif
