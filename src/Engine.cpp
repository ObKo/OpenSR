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

#include "Engine.h"
#include <cstdlib>
#include <stdexcept>
#include <SDL.h>
#include <SDL_mouse.h>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <ft2build.h>
#include <stdio.h>
#include FT_FREETYPE_H
#include <cstdio>
#include <libRanger.h>
#include "Utils.h"
#include "Log.h"
#include "ResourceManager.h"
#include <sstream>
#include "LuaBindings.h"
#include "LuaWidget.h"

using namespace std;

namespace Rangers
{
static Engine *engineInstance = 0;
FT_Library trueTypeLibrary;

#ifdef WIN32
#include <windows.h>
long long Engine::getTicks()
{
    return GetTickCount();
}
#else
#include <sys/time.h>

struct timeval tv;
struct timezone tz;
long long Engine::getTicks()
{
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

Engine::Engine(int argc, char **argv): m_argc(argc), m_argv(argv)
{
    if (engineInstance)
    {
        delete engineInstance;
        engineInstance = 0;
    }

    engineInstance = this;
    m_currentWidget = 0;
    m_focusedWidget = 0;
    m_consoleOpenned = false;
    m_showFPS = true;
    m_frames = 0;
}

Engine::~Engine()
{
    engineInstance = 0;
}

int Engine::fpsCounter()
{
    while (engineInstance->m_gameRunning)
    {
        SDL_Delay(1000);
        char str[255];
        sprintf(str, "%d", engineInstance->m_frames);
        if (engineInstance->m_frames >= 60)
            engineInstance->m_fpsLabel.setColor(0, 1, 0);
        else
            engineInstance->m_fpsLabel.setColor(1, 0, 0);
        engineInstance->m_fpsLabel.setText(str);
        engineInstance->m_frames = 0;
    }
    return 0;
}

void Engine::addWidget(Widget *w)
{
    m_mainNode.addChild(w);

    for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); i++)
    {
        if ((*i)->layer() < w->layer())
        {
            m_widgets.insert(i, w);
            return;
        }
    }
    m_widgets.push_back(w);
}

int Engine::logic()
{
    long t = getTicks();
    while (engineInstance->m_gameRunning)
    {
        int dt = getTicks() - t;
        while (!dt)
        {
            t = getTicks();
            SDL_Delay(1);
            dt = getTicks() - t;
        }
        engineInstance->m_mainNode.processLogic(dt);
        if (engineInstance->m_consoleOpenned)
            engineInstance->m_consoleWidget.processLogic(dt);
        t = getTicks();
    }
    return 0;
}

void Engine::init(int w, int h, bool fullscreen)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error("Unable to init SDL");

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Surface* screen = SDL_SetVideoMode(w, h, 32, SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));

    SDL_EnableKeyRepeat(660, 40);
    SDL_EnableUNICODE(1);

    m_width = screen->w;
    m_height = screen->h;

#ifdef _WIN32
    glewInit();
#endif
    FT_Init_FreeType(&trueTypeLibrary);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_ARB_texture_non_power_of_two);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, m_width, m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f);

    std::ifstream startupScript("opensrrc");
    if (startupScript.is_open())
    {
        char str[255];
        while (!startupScript.eof())
        {
            startupScript.getline(str, 255);
            execCommand(fromUTF8(str));
        }
        startupScript.close();
    }

    m_coreFont = ResourceManager::instance()->loadFont(L"DroidSans.ttf", 13);
    m_monospaceFont = ResourceManager::instance()->loadFont(L"DroidSansMono.ttf", 13);

    m_frames = 0;

    m_mainNode.setPosition(0, 0);

    m_fpsLabel = Label("0", 0, m_monospaceFont, POSITION_X_LEFT, POSITION_Y_TOP);
    m_fpsLabel.setPosition(5, 5);

    m_consoleWidget = ConsoleWidget(m_width, 168);

    LuaWidget *lw = new LuaWidget(L"data/test.lua");
    addWidget(lw);
}

boost::shared_ptr<Font> Engine::coreFont() const
{
    return m_coreFont;
}

boost::shared_ptr<Font> Engine::serviceFont() const
{
    return m_monospaceFont;
}

void Engine::paint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_mainNode.draw();
    if (m_consoleOpenned)
        m_consoleWidget.draw();
    if (m_showFPS)
        m_fpsLabel.draw();
    m_frames++;

    SDL_GL_SwapBuffers();
}

int Engine::run()
{
    m_gameRunning = true;
    m_fpsThread = new boost::thread(fpsCounter);
    m_logicThread = new boost::thread(logic);
    while (m_gameRunning)
    {
        m_updateMutex.lock();
        for (std::list<Object *>::const_iterator i = m_updateList.begin(); i != m_updateList.end(); i++)
            (*i)->processMain();
        m_updateList.clear();
        m_updateMutex.unlock();

        ResourceManager::instance()->processMain();

        processEvents();
        paint();
    }
    return m_exitCode;
}

void Engine::markToUpdate(Object* object)
{
    m_updateMutex.lock();
    m_updateList.push_back(object);
    m_updateMutex.unlock();
}

void Engine::unmarkToUpdate(Object* object)
{
    m_updateMutex.lock();
    m_updateList.remove(object);
    m_updateMutex.unlock();
}

void Engine::quit(int code)
{
    m_gameRunning = false;
    m_exitCode = code;
    m_logicThread->join();
    m_fpsThread->interrupt();
}

int Engine::screenHeight() const
{
    return m_height;
}

int Engine::screenWidth() const
{
    return m_width;
}

GLint Engine::textureInternalFormat(TextureType t) const
{
    switch (t)
    {
    case TEXTURE_A8:
        return GL_COMPRESSED_ALPHA_ARB;
        break;
    case TEXTURE_R8G8B8A8:
    case TEXTURE_B8G8R8A8:
        return GL_RGBA;
        break;
    }
    //GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
}

Engine *Engine::instance()
{
    return engineInstance;
}

void Engine::focusWidget(Widget* w)
{
    if (!m_consoleOpenned)
        m_focusedWidget = w;
}

std::wstring Engine::addObject(Object* object, const std::wstring& name)
{
    if (!object)
        return wstring();

    wstring objectName;

    if (name.empty())
        objectName = L"object";
    else
        objectName = name;

    if (!(object->parent()))
        m_mainNode.addChild(object);

    std::map<std::wstring, Object*>::iterator it = m_objects.find(objectName);
    if (it != m_objects.end())
    {
        int i = 1;
        wstring n;
        while (it != m_objects.end())
        {
            wostringstream s(objectName);
            s.seekp(0, ios_base::end);
            s << i;
            n = s.str();
            it = m_objects.find(n);
            i++;
        }
        Log::warning() << L"Object \"" << objectName << "\" exists. Renamed to \"" << n << "\"";
        m_objects[n] = object;
        return n;
    }
    m_objects[objectName] = object;
    return objectName;
}

Object* Engine::getObject(const std::wstring& name)
{
    if (name.empty())
        return 0;
    std::map<std::wstring, Object*>::iterator it = m_objects.find(name);
    if (it != m_objects.end())
        return (*it).second;
    else
    {
        Log::warning() << L"No such object: " << name;
        return 0;
    }
}

void Engine::removeObject(const std::wstring& name)
{
    if (name.empty())
        return;

    std::map<std::wstring, Object*>::iterator it = m_objects.find(name);
    if (it != m_objects.end())
    {
        delete(*it).second;
        m_objects.erase(it);
    }
    else
        Log::warning() << L"No such object: " << name;
}

void Engine::processEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            //case SDL_MOUSEBUTTONDOWN:
            //case SDL_MOUSEBUTTONUP:
            //currentWidget->
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_BACKQUOTE)
            {
                if (m_consoleOpenned)
                {
                    m_consoleOpenned = false;
                    m_focusedWidget = 0;
                }
                else
                {
                    m_consoleOpenned = true;
                    m_focusedWidget = &m_consoleWidget;
                }
                continue;
            }
            if (m_focusedWidget)
                m_focusedWidget->keyPressed(event.key.keysym);
            break;
        case SDL_MOUSEMOTION:
            processMouseMove(event.motion);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (m_currentWidget)
                m_currentWidget->mouseDown(event.button.button, event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            if (m_currentWidget)
                m_currentWidget->mouseUp(event.button.button, event.button.x, event.button.y);
            break;
        case SDL_QUIT:
            quit();
            break;
        }
    }
}

void Engine::processMouseMove(SDL_MouseMotionEvent e)
{
    for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); i++)
    {
        Rect bb = (*i)->getBoundingRect();
        if ((bb.x1 < e.x) && (bb.x2 > e.x) && (bb.y1 < e.y) && (bb.y2 > e.y))
        {
            if ((*i) != m_currentWidget)
            {
                if (m_currentWidget)
                    m_currentWidget->mouseLeave();
                m_currentWidget = *i;
                m_currentWidget->mouseEnter();
            }
            (*i)->mouseMove(e.x - bb.x1, e.y - bb.y1);
            return;
        }
    }
    if (m_currentWidget)
        m_currentWidget->mouseLeave();
    m_currentWidget = 0;
}

void Engine::execCommand(const std::wstring& what)
{
    if (what.empty())
        return;
    wistringstream args(what);
    wstring command;
    args >> command;

    if (command == L"quit")
        quit(0);
    else if (command == L"show_fps")
    {
        bool enable;
        args >> enable;

        if (!args.fail())
        {
            m_showFPS = enable;
            Log::info() << L"FPS label " << (m_showFPS ? (L"enabled") : (L"disabled"));
        }
    }
    else if (command == L"add_rpkg")
    {
        wstring fileName;
        args >> fileName;

        if (!args.fail())
            ResourceManager::instance()->addRPKG(fileName);
    }
    else if (command == L"add_dir")
    {
        wstring path;
        args >> path;

        if (!args.fail())
            ResourceManager::instance()->addDir(path);
    }
    else if (command == L"load_sprite")
    {
        wstring fileName;
        args >> fileName;

        if (!args.fail())
        {
            float x = 0, y = 0, w = 0, h = 0;
            int scaling = 1;
            args >> x >> y >> w >> h >> scaling;

            if (scaling > 5 || scaling < 0)
                scaling = 1;

            args.clear();

            boost::shared_ptr<Texture> t = ResourceManager::instance()->loadTexture(fileName);
            if (t)
            {
                Sprite *s = new Sprite(t, &m_mainNode, (TextureScaling)scaling);
                m_mainNode.addChild(s);
                wstring objName = addObject(s, fileName);
                s->setGeometry(w, h);
                s->setPosition(x, y);
                Log::info() << "Sprite \"" << objName << "\" loaded";
            }
        }
    }
    else if (command == L"load_asprite")
    {
        wstring fileName;
        args >> fileName;

        if (!args.fail())
        {
            float x = 0, y = 0, w = 0, h = 0;
            int scaling = 1;
            int currentFrame = 0;
            args >> currentFrame >> x >> y >> w >> h >> scaling;

            if (scaling > 5 || scaling < 0)
                scaling = 1;

            args.clear();

            boost::shared_ptr<AnimatedTexture> t = ResourceManager::instance()->loadAnimation(fileName);
            if (t)
            {
                AnimatedSprite *s = new AnimatedSprite(t, &m_mainNode);
                s->setFrame(currentFrame);
                m_mainNode.addChild(s);
                wstring objName = addObject(s, fileName);
                s->setGeometry(w, h);
                s->setPosition(x, y);
                s->setTextureScaling((TextureScaling)scaling);
                Log::info() << "Animation \"" << objName << "\" loaded";
            }
        }
    }
    else if (command == L"remove_object")
    {
        wstring name;
        args >> name;
        removeObject(name);
    }
    else
        Log::error() << wstring(L"Unknown command: ") << command;

    if (args.fail())
        Log::error() << L"Invalid arguments";
}
}
