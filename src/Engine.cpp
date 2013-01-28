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

#include "Engine.h"

#include "config.h"
#include <cstdlib>
#include <stdexcept>
#include <SDL.h>
#include <SDL_mouse.h>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cstdio>
#include <libRanger.h>
#include <libintl.h>
#include <sstream>

#include "Log.h"
#include "ResourceManager.h"
#include "LuaBindings.h"
#include "Action.h"
#include "JSONHelper.h"
#include "Plugin.h"

#include "private/Engine_p.h"


using namespace std;

namespace Rangers
{
FT_Library trueTypeLibrary;

#ifdef WIN32
#include <windows.h>
long long Engine::getTicks()
{
    return GetTickCount();
}
#else
#include <sys/time.h>

long long Engine::getTicks()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

Engine::EnginePrivate::EnginePrivate()
{
}

Engine::EnginePrivate::~EnginePrivate()
{
}

Engine::Engine(): m_d(new EnginePrivate())
{
    RANGERS_D(Engine);

    d->focusedWidget = 0;
    d->luaConsoleState = 0;
    d->argc = 0;
    d->argv = 0;
    d->currentWidget = 0;
    d->focusedWidget = 0;
    d->consoleOpenned = false;
    d->showFPS = true;
    d->frames = 0;
    d->m_q = this;
    d->mainNode = 0;
    d->consoleWidget = 0;
    d->fpsLabel = 0;

    textdomain("OpenSR");
}

Engine::Engine(const Engine& other)
{
}

Engine::~Engine()
{
    RANGERS_D(Engine);
    removeWidget(d->consoleWidget);
    if (!createDirPath(d->configPath))
        Log::error() << "Cannot create dir for config: " << d->configPath;

    std::ofstream configFile;
#if defined(WIN32) && defined(_MSC_VER)
    configFile.open(d->configPath, ios_base::out);
#else
    configFile.open(toLocal(d->configPath).c_str(), ios_base::out);
#endif
    if (configFile.is_open())
        boost::property_tree::write_ini(configFile, *d->properties);
    configFile.close();

    lua_close(d->luaConsoleState);

    std::list<Widget *>::iterator wend = d->widgets.end();
    for (std::list<Widget *>::iterator it = d->widgets.begin(); it != wend; ++it)
    {
        delete *it;
    }
    std::list<Plugin*>::const_iterator pend = d->plugins.end();
    for (std::list<Plugin*>::const_iterator i = d->plugins.begin(); i != pend; ++i)
    {
        delete *i;
    }
    delete d->consoleWidget;
    delete d->fpsLabel;
    delete d->mainNode;
    delete m_d;
}

void Engine::addWidget(Widget *w)
{
    RANGERS_D(Engine);
    if (!w)
        return;

    d->mainNode->addChild(w);

    std::list<Widget*>::iterator end = d->widgets.end();
    for (std::list<Widget*>::iterator i = d->widgets.begin(); i != end; ++i)
    {
        if ((*i)->layer() < w->layer())
        {
            d->widgets.insert(i, w);
            return;
        }
    }
    d->widgets.push_back(w);
}

void Engine::removeWidget(Widget *w)
{
    RANGERS_D(Engine);
    widgetHide(w);
    d->widgets.remove(w);
    d->mainNode->removeChild(w);
}

/*!
 * Tells engine that w will be hidden or destroyed, so engine can remove it from mouse and key handling.
 */

void Engine::widgetHide(Widget *w)
{
    if (!w)
        return;

    RANGERS_D(Engine);

    std::list<Widget*> children = w->childWidgets();

    if (d->currentWidget)
    {
        if (w == d->currentWidget)
        {
            d->currentWidget = 0;
            w->action(Action(w, Action::MOUSE_LEAVE));
        }
        else
        {
            std::list<Widget*>::const_iterator end = children.end();
            for (std::list<Widget*>::const_iterator i = children.begin(); i != end; ++i)
            {
                if ((*i) == d->currentWidget)
                {
                    d->currentWidget = 0;
                    (*i)->action(Action((*i), Action::MOUSE_LEAVE));
                }
            }
        }
    }
    if (d->focusedWidget)
    {
        if (w == d->focusedWidget)
        {
            d->focusedWidget = 0;
            w->unFocus();
        }
        else
        {
            std::list<Widget*>::const_iterator end = children.end();
            for (std::list<Widget*>::const_iterator i = children.begin(); i != end; ++i)
            {
                if ((*i) == d->focusedWidget)
                {
                    d->focusedWidget = 0;
                    (*i)->unFocus();
                }
            }
        }
    }
}


void Engine::widgetDestroyed(Widget *w)
{
    if (!w)
        return;

    RANGERS_D(Engine);

    std::list<Widget*> children = w->childWidgets();

    if (d->currentWidget)
    {
        if (w == d->currentWidget)
            d->currentWidget = 0;
    }
    if (d->focusedWidget)
    {
        if (w == d->focusedWidget)
            d->focusedWidget = 0;
    }
}

int Engine::logic()
{
    long t = getTicks();
    Engine &engine = Engine::instance();
    while (engine.m_d->gameRunning)
    {
        int dt = getTicks() - t;
        while (!dt)
        {
            //t = getTicks();
            SDL_Delay(1);
            dt = getTicks() - t;
        }
        engine.m_d->mainNode->processLogic(dt);
        if (engine.m_d->consoleOpenned)
            engine.m_d->consoleWidget->processLogic(dt);
        t = getTicks();
    }
    return 0;
}

void Engine::init(int argc, char **argv, int w, int h, bool fullscreen)
{
    RANGERS_D(Engine);

    d->argc = argc;
    d->argv = argv;

    std::ifstream configFile;
    d->properties = boost::shared_ptr<boost::property_tree::ptree>(new boost::property_tree::ptree());

#ifdef WIN32
    wchar_t *path = new wchar_t[1024];
    GetModuleFileName(0, path, 1024);
    d->configPath = (directory(std::wstring(path)) + L"\\OpenSR.ini");
    delete[] path;
#ifdef _MSC_VER
    configFile.open(d->configPath, ios_base::in);
#else
    configFile.open(toLocal(d->configPath).c_str(), ios_base::in);
#endif

#else
    char *path;
    if ((path = getenv("XDG_CONFIG_HOME")) == NULL)
    {
        if ((path = getenv("HOME")) == NULL)
        {
            d->configPath = fromLocal((directory(std::string(argv[0])) + "/OpenSR.conf").c_str());
        }
        else
        {
            d->configPath = fromLocal((std::string(path) + "/.config/OpenSR/OpenSR.conf").c_str());
        }
    }
    else
        d->configPath = fromLocal((std::string(path) + "/OpenSR/OpenSR.conf").c_str());
    configFile.open(toLocal(d->configPath).c_str(), ios_base::in);
#endif

    if (configFile.is_open())
    {
        try
        {
            boost::property_tree::read_ini(configFile, *d->properties);
        }
        catch (boost::property_tree::ini_parser_error &error)
        {
            Log::error() << "Error parsing ini file " << d->configPath << ":" << error.line() << ": " << error.message();
        }
    }

    configFile.close();

#ifdef WIN32
    Log::instance()->setColorOutput(d->properties->get<bool>("log.color", false));
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
    Log::instance()->setColorOutput(d->properties->get<bool>("log.color", colorTerminal));
#endif

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error("Unable to init SDL");

    d->mainDataDir = fromLocal(d->properties->get<std::string>("data.mainDataDir", "").c_str());
    if (d->mainDataDir != L"")
    {
        if (!createDirPath(d->mainDataDir) && !checkDirWritable(d->mainDataDir))
        {
            Log::error() << "Cannot create data dir or dir not writable: " << d->mainDataDir;
            d->mainDataDir = L"";
        }
    }
    Log::info() << d->mainDataDir;
#ifdef WIN32
    if (d->mainDataDir == L"")
    {
        d->mainDataDir = fromLocal((directory(std::string(d->argv[0]))).c_str());
    }
#else
    if (d->mainDataDir == L"")
    {
        char *path;
#ifdef __linux
        if ((path = getenv("XDG_DATA_HOME")) == NULL)
        {
#endif
            if ((path = getenv("HOME")) != NULL)
            {
                d->mainDataDir = fromLocal((std::string(path) + "/.OpenSR/").c_str());
                if (!createDirPath(d->mainDataDir) && !checkDirWritable(d->mainDataDir))
                {
                    Log::error() << "Cannot create data dir or dir not writable: " << d->mainDataDir;
                    d->mainDataDir = L"";
                }
            }
#ifdef __linux
        }
        else
        {
            d->mainDataDir = fromLocal((std::string(path) + "/OpenSR/").c_str());
            if (!createDirPath(d->mainDataDir) && !checkDirWritable(d->mainDataDir))
            {
                Log::error() << "Cannot create data dir or dir not writable: " << d->mainDataDir;
                d->mainDataDir = L"";
            }
        }
#endif
    }
#endif
    if (d->mainDataDir == L"")
        throw std::runtime_error("No writable data dir available.");

    Log::debug() << "Using data dir: " << d->mainDataDir;
    ResourceManager::instance().addDir(d->mainDataDir);
#ifdef __linux
    char *pathes;
    if ((pathes = getenv("XDG_DATA_DIRS")) != NULL)
    {
        std::vector<std::wstring> list = split(fromLocal(pathes), L':');
        for (std::vector<std::wstring>::const_iterator i = list.begin(); i != list.end(); i++)
            ResourceManager::instance().addDir((*i) + L"/OpenSR/");
    }
    else
    {
        //FIXME: use install prefix.
        ResourceManager::instance().addDir(L"/usr/share/OpenSR/");
    }
#endif

    w = d->properties->get<int>("graphics.width", w);
    h = d->properties->get<int>("graphics.height", h);
    fullscreen = d->properties->get<bool>("graphics.fullscreen", fullscreen);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Surface* screen = SDL_SetVideoMode(w, h, 32, SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));

    SDL_EnableKeyRepeat(660, 40);
    SDL_EnableUNICODE(1);

    d->width = screen->w;
    d->height = screen->h;

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

    glViewport(0, 0, d->width, d->height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, d->width, d->height, 0.0f, -1.0f, 1.0f);

    std::vector<std::string> coreFontStrings = split(d->properties->get<std::string>("graphics.corefont", "DroidSans.ttf:14"), ':');
    int coreFontSize = 13;
    bool coreFontAA = true;
    if (coreFontStrings.size() > 1)
        coreFontSize = atoi(coreFontStrings.at(1).c_str());
    if (coreFontStrings.size() > 2 && coreFontStrings.at(2) == "false")
        coreFontAA = false;
    d->coreFont = ResourceManager::instance().loadFont(fromLocal(coreFontStrings.at(0).c_str()), coreFontSize, coreFontAA);

    std::vector<std::string> monoFontStrings = split(d->properties->get<std::string>("graphics.monofont", "DroidSansMono.ttf:13"), ':');
    int monoFontSize = 13;
    bool monoFontAA = true;
    if (monoFontStrings.size() > 1)
        monoFontSize = atoi(monoFontStrings.at(1).c_str());
    if (monoFontStrings.size() > 2 && monoFontStrings.at(2) == "false")
        monoFontAA = false;
    d->monospaceFont = ResourceManager::instance().loadFont(fromLocal(monoFontStrings.at(0).c_str()), monoFontSize, monoFontAA);

    setDefaultSkin(fromLocal(d->properties->get<std::string>("graphics.defaultSkin", "").c_str()));

    d->frames = 0;

    d->mainNode = new Node();
    d->mainNode->setPosition(0, 0);

    d->fpsLabel = new Label("0", 0, d->monospaceFont, POSITION_X_LEFT, POSITION_Y_TOP);
    d->fpsLabel->setPosition(5, 5);

    d->consoleWidget = new ConsoleWidget(d->width, 168);
    d->consoleWidget->setVisible(false);
    d->luaConsoleState = initLuaState();
    addWidget(d->consoleWidget);
}

boost::shared_ptr<Font> Engine::coreFont() const
{
    RANGERS_D(const Engine);
    return d->coreFont;
}

boost::shared_ptr<Font> Engine::serviceFont() const
{
    RANGERS_D(const Engine);
    return d->monospaceFont;
}

void Engine::paint()
{
    RANGERS_D(Engine);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    d->mainNode->draw();
    if (d->consoleOpenned)
        d->consoleWidget->draw();
    if (d->showFPS)
        d->fpsLabel->draw();
    d->frames++;

    SDL_GL_SwapBuffers();
}

int Engine::run()
{
    RANGERS_D(Engine);
    d->gameRunning = true;
    d->logicThread = new boost::thread(&Engine::logic);
    std::wstring startupScript = fromLocal(d->properties->get<std::string>("engine.startupScript", "startup.lua").c_str());
    execLuaScript(startupScript);
    d->fpsTime = getTicks();
    while (d->gameRunning)
    {
        int fpsdt;
        if ((fpsdt = getTicks() - d->fpsTime) >= 1000)
        {
            char str[11];
            double fps = (float)d->frames / fpsdt * 1000;
#if defined(WIN32) && defined(_MSC_VER)
            _snprintf(str, 10, "%.1lf", fps);
#else
            snprintf(str, 10, "%.1lf", fps);
#endif
            d->fpsLabel->setText(str);
            if (fps >= 30)
                d->fpsLabel->setColor(0.0f, 1.0f, 0.0f);
            else
                d->fpsLabel->setColor(1.0f, 0.0f, 0.0f);
            d->frames = 0;
            d->fpsTime = getTicks();
        }
        d->updateMutex.lock();

        std::list<Widget*>::const_iterator widgetEnd = d->widgetsToDelete.end();
        for (std::list<Widget*>::const_iterator i = d->widgetsToDelete.begin(); i != widgetEnd; ++i)
        {
            removeWidget(*i);
            delete(*i);
        }

        std::list<Object *>::const_iterator end = d->updateList.end();
        for (std::list<Object *>::const_iterator i = d->updateList.begin(); i != end; ++i)
            if ((*i)->needUpdate())
                (*i)->processMain();

        d->widgetsToDelete.clear();
        d->updateList.clear();

        d->updateMutex.unlock();


        ResourceManager::instance().processMain();

        d->processEvents();
        paint();
    }
    d->logicThread->join();
    SDL_Quit();
    return d->exitCode;
}

void Engine::markToUpdate(Object* object)
{
    RANGERS_D(Engine);
    d->updateMutex.lock();
    d->updateList.push_back(object);
    d->updateMutex.unlock();
}

void Engine::unmarkToUpdate(Object* object)
{
    RANGERS_D(Engine);
    d->updateMutex.lock();
    d->updateList.remove(object);
    d->updateMutex.unlock();
}

void Engine::quit(int code)
{
    RANGERS_D(Engine);
    d->gameRunning = false;
    d->exitCode = code;
}

int Engine::screenHeight() const
{
    RANGERS_D(const Engine);
    return d->height;
}

int Engine::screenWidth() const
{
    RANGERS_D(const Engine);
    return d->width;
}

boost::shared_ptr<boost::property_tree::ptree> Engine::properties() const
{
    RANGERS_D(const Engine);
    return d->properties;
}

Engine& Engine::instance()
{
    static Engine engine;
    return engine;
}

void Engine::focusWidget(Widget* w)
{
    RANGERS_D(Engine);
    if (d->focusedWidget)
        d->focusedWidget->unFocus();
    d->focusedWidget = w;
    w->focus();
}

void Engine::unfocusWidget(Widget* w)
{
    RANGERS_D(Engine);
    if (d->focusedWidget != w)
        return;
    d->focusedWidget->unFocus();
    d->focusedWidget = 0;
}

void Engine::markWidgetDeleting(Widget *w)
{
    RANGERS_D(Engine);
    d->updateMutex.lock();
    removeWidget(w);
    d->widgetsToDelete.push_back(w);
    d->updateMutex.unlock();
}

Skin Engine::defaultSkin() const
{
    RANGERS_D(const Engine);
    return d->skin;
}

void Engine::setDefaultSkin(const Skin& skin)
{
    RANGERS_D(Engine);
    d->skin = skin;
}

void Engine::setDefaultSkin(const std::wstring& skinPath)
{
    RANGERS_D(Engine);
    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(skinPath);
    if (!json)
        return;
    bool error;
    d->skin = JSONHelper::parseSkin(*json, error);
}

void Engine::EnginePrivate::processEvents()
{
    RANGERS_Q(Engine);
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_BACKQUOTE)
            {
                consoleOpenned = !consoleOpenned;
                consoleWidget->setVisible(consoleOpenned);
                continue;
            }
            if (focusedWidget)
                focusedWidget->action(Action(focusedWidget, Rangers::Action::KEY_PRESSED, event.key.keysym));
            break;
        case SDL_MOUSEMOTION:
            processMouseMove(event.motion);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (currentWidget)
                currentWidget->mouseDown(event.button.button, event.button.x, event.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            if (currentWidget)
                currentWidget->mouseUp(event.button.button, event.button.x, event.button.y);
            break;
        case SDL_QUIT:
            q->quit();
            break;
        }
    }
}

void Engine::EnginePrivate::processMouseMove(const SDL_MouseMotionEvent &e)
{
    RANGERS_Q(Engine);
    for (std::list<Widget*>::iterator i = widgets.begin(); i != widgets.end(); i++)
    {
        Rect bb = (*i)->mapToGlobal((*i)->getBoundingRect());
        Vector globalMouse = mainNode->mapFromScreen(Vector(e.x, e.y));
        if ((*i)->isVisible() && bb.contains(globalMouse))
        {
            if ((*i) != currentWidget)
            {
                if (currentWidget)
                    currentWidget->action(Action(currentWidget, Action::MOUSE_LEAVE));
                currentWidget = *i;
                currentWidget->action(Action(currentWidget, Action::MOUSE_ENTER));
            }
            Vector mouse = (*i)->mapFromParent(globalMouse);
            (*i)->mouseMove(mouse.x, mouse.y);
            return;
        }
    }
    if (currentWidget)
        currentWidget->action(Action(currentWidget, Action::MOUSE_LEAVE));
    currentWidget = 0;
}

Node* Engine::rootNode()
{
    RANGERS_D(Engine);
    return d->mainNode;
}

void Engine::loadPlugin(const std::wstring& pluginName)
{
    RANGERS_D(Engine);

    std::wstring name = basename(pluginName);

    std::wstring pluginPath = fromUTF8(d->properties->get<std::string>("data.pluginPath", "").c_str());
    std::vector<std::wstring> searchPaths = split(pluginPath, L':');
    std::wstring searchSuffix;

#ifdef WIN32
    searchSuffix += L".dll";
#elif __APPLE__
    searchSuffix += L".dylib";
#else
    searchSuffix += L".so";
#endif

    std::vector<std::wstring> searchNames;
    searchNames.push_back(name + searchSuffix);
    searchNames.push_back(L"lib" + name + searchSuffix);

    std::vector<std::wstring>::const_iterator end = searchPaths.end();
    for (std::vector<std::wstring>::const_iterator i = searchPaths.begin(); i != end; ++i)
    {
        searchNames.push_back((*i) + L"/" + name + searchSuffix);
        searchNames.push_back((*i) + L"/" + L"lib" + name + searchSuffix);
    }

    Log::debug() << "Plugin \"" << pluginName << "\" search names:";
    end = searchNames.end();
    for (std::vector<std::wstring>::const_iterator i = searchNames.begin(); i != end; ++i)
        Log::debug() << "\t" << (*i);

    std::wstring realName;
    end = searchNames.end();
    for (std::vector<std::wstring>::const_iterator i = searchNames.begin(); i != end; ++i)
    {
        if (fileExists(*i))
        {
            realName = *i;
            break;
        }
    }
    Plugin *p = new Plugin(realName);
    if (p->load())
    {
        delete p;
        return;
    }
    p->initLua(d->luaConsoleState);
    d->plugins.push_back(p);
}

void Engine::initPluginLua(lua_State *state)
{
    RANGERS_D(Engine);
    std::list<Plugin*>::const_iterator end = d->plugins.end();
    for (std::list<Plugin*>::const_iterator i = d->plugins.begin(); i != end; ++i)
    {
        (*i)->initLua(state);
    }
}

void Engine::execCommand(const std::wstring& what)
{
    RANGERS_D(Engine);
    if (what.empty())
        return;

    int state;
    std::string line = toUTF8(what);
    if (state = (luaL_loadbuffer(d->luaConsoleState, line.c_str(), line.length(), "") || lua_pcall(d->luaConsoleState, 0, LUA_MULTRET, 0)))
        Log::error() << "Cannot exec command: " << lua_tostring(d->luaConsoleState, -1);
}
}
