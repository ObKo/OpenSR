/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ft2build.h>
#include <stdio.h>
#include FT_FREETYPE_H
#include <cstdio>
#include <libRanger.h>
#include <lua.hpp>
#include <libintl.h>
#include "Utils.h"
#include "Log.h"
#include "ResourceManager.h"
#include <sstream>
#include "LuaBindings.h"
#include "LuaWidget.h"
#include "AnimatedSprite.h"
#include "Action.h"
#include "JSONHelper.h"
#include "Styles.h"


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

Engine::Engine(int argc, char **argv): m_argc(argc), m_argv(argv), m_focusedWidget(0), m_luaConsoleState(0)
{
    if (engineInstance)
    {
        delete engineInstance;
        engineInstance = 0;
    }

    textdomain("OpenSR");

    std::ifstream configFile;
    m_properties = boost::shared_ptr<boost::property_tree::ptree>(new boost::property_tree::ptree());

#ifdef WIN32
    char *path = new char[1024];
    GetModuleFileName(0, path, 1024);
    m_configPath = fromLocal((directory(std::string(path)) + "\\OpenSR.ini").c_str());
    delete[] path;
    configFile.open(m_configPath, ios_base::in);
#else
    char *path;
    if ((path = getenv("XDG_CONFIG_HOME")) == NULL)
    {
        if ((path = getenv("HOME")) == NULL)
        {
            m_configPath = fromLocal((directory(std::string(argv[0])) + "/OpenSR.conf").c_str());
        }
        else
        {
            m_configPath = fromLocal((std::string(path) + "/.OpenSR/OpenSR.conf").c_str());
        }
    }
    else
        m_configPath = fromLocal((std::string(path) + "/OpenSR/OpenSR.conf").c_str());
    configFile.open(toLocal(m_configPath).c_str(), ios_base::in);
#endif

    if (configFile.is_open())
    {
        try
        {
            boost::property_tree::read_ini(configFile, *m_properties);
        }
        catch (boost::property_tree::ini_parser_error &error)
        {
            Log::error() << "Error parsing ini file " << m_configPath << ":" << error.line() << ": " << error.message();
        }
    }

    configFile.close();

#ifdef WIN32
    Log::instance()->setColorOutput(m_properties->get<bool>("log.color", false));
#else
    bool colorTerminal = false;
    const char *term = getenv("TERM");
    if (term)
    {
        std::string terminal(term);
        //FIXME: looks quite ugly
        if ((terminal.find("xterm") != string::npos)
                || (terminal.find("color") != string::npos))
            colorTerminal = true;
    }
    Log::instance()->setColorOutput(m_properties->get<bool>("log.color", colorTerminal));
#endif

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
    if (!createDirPath(m_configPath))
        Log::error() << "Cannot create dir for config: " << m_configPath;

    std::ofstream configFile;
#ifdef WIN32
    configFile.open(m_configPath, ios_base::out);
#else
    configFile.open(toLocal(m_configPath).c_str(), ios_base::out);
#endif
    if (configFile.is_open())
        boost::property_tree::write_ini(configFile, *m_properties);
    configFile.close();

    std::list<Widget *>::iterator end = m_widgets.end();
    for (std::list<Widget *>::iterator it = m_widgets.begin(); it != end; ++it)
    {
        delete *it;
    }
}

int Engine::fpsCounter()
{
    while (engineInstance->m_gameRunning)
    {
        SDL_Delay(1000);
        ostringstream fps(ostringstream::out);
        fps << engineInstance->m_frames;
        if (engineInstance->m_frames >= 60)
            engineInstance->m_fpsLabel.setColor(0, 1, 0);
        else
            engineInstance->m_fpsLabel.setColor(1, 0, 0);
        engineInstance->m_fpsLabel.setText(fps.str());
        engineInstance->m_frames = 0;
    }
    return 0;
}

void Engine::addWidget(Widget *w)
{
    if (!w)
        return;

    m_mainNode.addChild(w);

    std::list<Widget*>::iterator end = m_widgets.end();
    for (std::list<Widget*>::iterator i = m_widgets.begin(); i != end; ++i)
    {
        if ((*i)->layer() < w->layer())
        {
            m_widgets.insert(i, w);
            return;
        }
    }
    m_widgets.push_back(w);
}

void Engine::removeWidget(Widget *w)
{
    if (w == m_currentWidget)
        m_currentWidget = 0;
    if (w == m_focusedWidget)
        m_focusedWidget = 0;
    m_widgets.remove(w);
    m_mainNode.removeChild(w);

    m_updateMutex.lock();
    m_updateList.remove(w);
    m_updateMutex.unlock();
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

    m_mainDataDir = fromLocal(m_properties->get<std::string>("data.mainDataDir", "").c_str());
    if (m_mainDataDir != L"")
    {
        if (!createDirPath(m_mainDataDir) && !checkDirWritable(m_mainDataDir))
        {
            Log::error() << "Cannot create data dir or dir not writable: " << m_mainDataDir;
            m_mainDataDir = L"";
        }
    }
    Log::info() << m_mainDataDir;
#ifdef WIN32
    if (m_mainDataDir == L"")
    {
        m_mainDataDir = fromLocal((directory(std::string(m_argv[0]))).c_str());
    }
#else
    if (m_mainDataDir == L"")
    {
#ifdef __linux
        char *path;
        if ((path = getenv("XDG_DATA_HOME")) == NULL)
        {
#endif
            if ((path = getenv("HOME")) != NULL)
            {
                m_mainDataDir = fromLocal((std::string(path) + "/.OpenSR/").c_str());
                if (!createDirPath(m_mainDataDir) && !checkDirWritable(m_mainDataDir))
                {
                    Log::error() << "Cannot create data dir or dir not writable: " << m_mainDataDir;
                    m_mainDataDir = L"";
                }
            }
#ifdef __linux
        }
        else
        {
            m_mainDataDir = fromLocal((std::string(path) + "/OpenSR/").c_str());
            if (!createDirPath(m_mainDataDir) && !checkDirWritable(m_mainDataDir))
            {
                Log::error() << "Cannot create data dir or dir not writable: " << m_mainDataDir;
                m_mainDataDir = L"";
            }
        }
#endif
    }
#endif
    if (m_mainDataDir == L"")
        throw std::runtime_error("No writable data dir available.");

    Log::debug() << "Using data dir: " << m_mainDataDir;
    ResourceManager::instance()->addDir(m_mainDataDir);
#ifdef __linux
    char *pathes;
    if ((pathes = getenv("XDG_DATA_DIRS")) != NULL)
    {
        std::vector<std::wstring> list = split(fromLocal(pathes), L':');
        for (std::vector<std::wstring>::const_iterator i = list.begin(); i != list.end(); i++)
            ResourceManager::instance()->addDir((*i) + L"/OpenSR/");
    }
    else
    {
        //FIXME: use install prefix.
        ResourceManager::instance()->addDir(L"/usr/share/OpenSR/");
    }
#endif

    w = m_properties->get<int>("graphics.width", w);
    h = m_properties->get<int>("graphics.height", h);
    fullscreen = m_properties->get<bool>("graphics.fullscreen", fullscreen);

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

    std::vector<std::string> coreFontStrings = split(m_properties->get<std::string>("graphics.corefont", "DroidSans.ttf:14"), ':');
    int coreFontSize = 13;
    if (coreFontStrings.size() > 1)
        coreFontSize = atoi(coreFontStrings.at(1).c_str());
    m_coreFont = ResourceManager::instance()->loadFont(fromLocal(coreFontStrings.at(0).c_str()), coreFontSize);

    std::vector<std::string> monoFontStrings = split(m_properties->get<std::string>("graphics.monofont", "DroidSansMono.ttf:13"), ':');
    int monoFontSize = 13;
    if (monoFontStrings.size() > 1)
        monoFontSize = atoi(monoFontStrings.at(1).c_str());
    m_monospaceFont = ResourceManager::instance()->loadFont(fromLocal(monoFontStrings.at(0).c_str()), monoFontSize);

    setDefaultSkin(fromLocal(m_properties->get<std::string>("graphics.defaultSkin", "").c_str()));

    m_frames = 0;

    m_mainNode.setPosition(0, 0);

    m_fpsLabel = Label("0", 0, m_monospaceFont, POSITION_X_LEFT, POSITION_Y_TOP);
    m_fpsLabel.setPosition(5, 5);

    m_consoleWidget = ConsoleWidget(m_width, 168);
    m_luaConsoleState = initLuaState();
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
    std::wstring startupScript = fromLocal(m_properties->get<std::string>("engine.startupScript", "startup.lua").c_str());
    execLuaScript(startupScript);
    while (m_gameRunning)
    {
        m_updateMutex.lock();
        std::list<Widget*>::const_iterator widgetEnd = m_widgetsToDelete.end();
        for (std::list<Widget*>::const_iterator i = m_widgetsToDelete.begin(); i != widgetEnd; ++i)
        {
            removeWidget(*i);
            delete(*i);
        }
        m_widgetsToDelete.clear();

        std::list<Object *>::const_iterator end = m_updateList.end();
        for (std::list<Object *>::const_iterator i = m_updateList.begin(); i != end; ++i)
            if ((*i)->needUpdate())
                (*i)->processMain();
        m_updateList.clear();

        m_updateMutex.unlock();

        ResourceManager::instance()->processMain();

        processEvents();
        paint();
    }
    m_fpsThread->join();
    m_logicThread->join();
    SDL_Quit();
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
}

int Engine::screenHeight() const
{
    return m_height;
}

int Engine::screenWidth() const
{
    return m_width;
}

boost::shared_ptr<boost::property_tree::ptree> Engine::properties() const
{
    return m_properties;
}

Engine *Engine::instance()
{
    return engineInstance;
}

void Engine::focusWidget(Widget* w)
{
    if (!m_consoleOpenned)
    {
        if (m_focusedWidget)
            m_focusedWidget->unFocus();
        m_focusedWidget = w;
        w->focus();
    }
}

void Engine::markWidgetDeleting(Widget *w)
{
    m_updateMutex.lock();
    removeWidget(w);
    m_widgetsToDelete.push_back(w);
    m_updateMutex.unlock();
}

Skin Engine::defaultSkin() const
{
    return m_skin;
}

void Engine::setDefaultSkin(const Skin& skin)
{
    m_skin = skin;
}

void Engine::setDefaultSkin(const std::wstring& skinPath)
{
    size_t size;
    char *json = ResourceManager::instance()->loadData(skinPath, size);
    bool error;
    m_skin = JSONHelper::parseSkin(std::string(json, size), error);
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
                m_focusedWidget->action(Action(m_focusedWidget, Rangers::Action::KEY_PRESSED, event.key.keysym));
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
        Rect bb = (*i)->mapToGlobal((*i)->getBoundingRect());
        Vector globalMouse = m_mainNode.mapFromScreen(Vector(e.x, e.y));
        if (bb.contains(globalMouse))
        {
            if ((*i) != m_currentWidget)
            {
                if (m_currentWidget)
                    m_currentWidget->mouseLeave();
                m_currentWidget = *i;
                m_currentWidget->mouseEnter();
            }
            Vector mouse = (*i)->mapFromParent(globalMouse);
            (*i)->mouseMove(mouse.x, mouse.y);
            return;
        }
    }
    if (m_currentWidget)
        m_currentWidget->mouseLeave();
    m_currentWidget = 0;
}

Node* Engine::rootNode()
{
    return &m_mainNode;
}

void Engine::execCommand(const std::wstring& what)
{
    if (what.empty())
        return;

    int state;
    std::string line = toUTF8(what);
    if (state = (luaL_loadbuffer(m_luaConsoleState, line.c_str(), line.length(), "") || lua_pcall(m_luaConsoleState, 0, LUA_MULTRET, 0)))
        Log::error() << "Cannot exec command: " << lua_tostring(m_luaConsoleState, -1);
}
}
