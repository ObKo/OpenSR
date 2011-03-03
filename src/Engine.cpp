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

using namespace Rangers;
using namespace std;

static Engine *engineInstance = 0;
FT_Library trueTypeLibrary;

#ifdef WIN32
#include <windows.h>
long long getTicks()
{
    return GetTickCount();
}
#else
#include <sys/time.h>

struct timeval tv;
struct timezone tz;
long long getTicks()
{
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

Engine* Rangers::getEngine()
{
    return Engine::instance();
}

Engine::Engine(int argc, char **argv): argc(argc), argv(argv)
{
    if (engineInstance)
    {
        delete engineInstance;
        engineInstance = 0;
    }

    engineInstance = this;
    currentWidget = 0;
    focusedWidget = 0;
    consoleOpenned = false;
    showFPS = true;
    frames = 0;
}

Engine::~Engine()
{
    engineInstance = 0;
}

int Engine::fpsCounter(void *data)
{
    while (engineInstance->gameRunning)
    {
        SDL_Delay(1000);
	char str[255];
        sprintf(str, "%d", engineInstance->frames);
        if (engineInstance->frames >= 60)
            engineInstance->fpsLabel.setColor(0, 1, 0);
        else
            engineInstance->fpsLabel.setColor(1, 0, 0);
        engineInstance->fpsLabel.setText(str);
        engineInstance->frames = 0;
    }
    return 0;
}

void Engine::addWidget(Widget *w)
{
    mainNode.addChild(w);

    for (std::list<Widget*>::iterator i = widgets.begin(); i != widgets.end(); i++)
    {
        if ((*i)->layer() < w->layer())
        {
            widgets.insert(i, w);
            return;
        }
    }
    widgets.push_back(w);
}

int Engine::logic(void *data)
{
    SDL_mutexP(engineInstance->logicMutex);
    long t = getTicks();
    while (engineInstance->gameRunning)
    {
        int dt = getTicks() - t;
        while (!dt)
        {
            t = getTicks();
            SDL_Delay(1);
            dt = getTicks() - t;
        }
        engineInstance->mainNode.processLogic(dt);
        if (engineInstance->consoleOpenned)
            engineInstance->consoleWidget.processLogic(dt);
        t = getTicks();
    }
    SDL_mutexV(engineInstance->logicMutex);
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

    updateMutex = SDL_CreateMutex();
    logicMutex = SDL_CreateMutex();

    width = screen->w;
    height = screen->h;

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

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    
    std::ifstream startupScript("opensrrc");
    if(startupScript.is_open())
    {
	char str[255];
	while(!startupScript.eof())
	{
	    startupScript.getline(str, 255);
	    execCommand(fromUTF8(str));
	}
	startupScript.close();
    }

    engineFont = ResourceManager::instance()->loadFont(L"DroidSans.ttf", 13);
    monospaceFont = ResourceManager::instance()->loadFont(L"DroidSansMono.ttf", 13);

    frames = 0;
    
    mainNode.setPosition(0, 0);

    fpsLabel = Label("0", 0, monospaceFont, POSITION_X_LEFT, POSITION_Y_TOP);
    fpsLabel.setPosition(5, 5);

    consoleWidget = ConsoleWidget(width, 168);
    
    LuaWidget *lw = new LuaWidget(L"data/test.lua");
    this->addWidget(lw);

}

boost::shared_ptr<Font> Engine::defaultFont() const
{
    return engineFont;
}

boost::shared_ptr<Font> Engine::serviceFont() const
{
    return monospaceFont;
}

void Engine::paint()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mainNode.draw();
    if (consoleOpenned)
        consoleWidget.draw();
    if (showFPS)
        fpsLabel.draw();
    frames++;

    SDL_GL_SwapBuffers();
}

int Engine::run()
{
    gameRunning = true;
    fpsThread = SDL_CreateThread(&fpsCounter, NULL);
    logicThread = SDL_CreateThread(&logic, NULL);
    while (gameRunning)
    {
        SDL_mutexP(updateMutex);
        for (std::list<Object *>::const_iterator i = updateList.begin(); i != updateList.end(); i++)
            (*i)->processMain();
        updateList.clear();
        SDL_mutexV(updateMutex);
	
	ResourceManager::instance()->processMain();

        processEvents();
        paint();
    }
    return exitCode;
}

void Engine::markToUpdate(Object* object)
{
    SDL_mutexP(updateMutex);
    updateList.push_back(object);
    SDL_mutexV(updateMutex);
}

void Engine::unmarkToUpdate(Object* object)
{
    SDL_mutexP(updateMutex);
    updateList.remove(object);
    SDL_mutexV(updateMutex);
}

void Engine::quit(int code)
{
    gameRunning = false;
    exitCode = code;
    SDL_WaitThread(logicThread, 0);
    SDL_WaitThread(fpsThread, 0);
}

int Engine::screenHeight() const
{
    return height;
}

int Engine::screenWidth() const
{
    return width;
}

Engine *Engine::instance()
{
    return engineInstance;
}

void Engine::focusWidget(Widget* w)
{
    if (!consoleOpenned)
        focusedWidget = w;
}

std::wstring Engine::addObject(Object* object, const std::wstring& name)
{
    if(!object)
	return wstring();
    
    wstring objectName;
    
    if(name.empty())
	objectName = L"object";
    else
        objectName = name;
    
    if(!(object->parent()))
	mainNode.addChild(object);
    
    std::map<std::wstring, Object*>::iterator it = objects.find(objectName);
    if(it != objects.end())
    {
	int i = 1;
	wstring n;
	while(it != objects.end())
	{
	    wostringstream s(objectName);
	    s.seekp(0, ios_base::end);
	    s << i;
	    n = s.str();
	    it = objects.find(n);
	    i++;
	}
	logger() << LWARNING << L"Object \"" << objectName << "\" exists. Renamed to \"" << n << "\"" << LEND; 
	objects[n] = object;
	return n;
    }
    objects[objectName] = object;
    return objectName;
}

Object* Engine::getObject(const std::wstring& name)
{
    if(name.empty())
        return 0;
    std::map<std::wstring, Object*>::iterator it = objects.find(name);
    if(it != objects.end())
        return (*it).second;
    else
    {
        logger() << LWARNING << L"No such object: " << name << LEND; 
        return 0;
    }
}

void Engine::removeObject(const std::wstring& name)
{
    if(name.empty())
        return;
    
    std::map<std::wstring, Object*>::iterator it = objects.find(name);
    if(it != objects.end())
    {
        delete (*it).second;
        objects.erase(it);
    }
    else
        logger() << LWARNING << L"No such object: " << name << LEND; 
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
                if (consoleOpenned)
                {
                    consoleOpenned = false;
                    focusedWidget = 0;
                }
                else
                {
                    consoleOpenned = true;
                    focusedWidget = &consoleWidget;
                }
                continue;
            }
            if (focusedWidget)
                focusedWidget->keyPressed(event.key.keysym);
            break;
        case SDL_MOUSEMOTION:
            processMouseMove(event.motion);
            break;
        case SDL_QUIT:
            quit();
            break;
        }
    }
}

void Engine::processMouseMove(SDL_MouseMotionEvent e)
{
    //cout << "X: " << e.x << " Y: " << e.y << endl;
    for (std::list<Widget*>::iterator i = widgets.begin(); i != widgets.end(); i++)
    {
        Rect bb = (*i)->getBoundingRect();
        if ((bb.x1 < e.x) && (bb.x2 > e.x) && (bb.y1 < e.y) && (bb.y2 > e.y))
        {
            if ((*i) != currentWidget)
            {
                if (currentWidget)
                    currentWidget->mouseLeave();
                currentWidget = *i;
                currentWidget->mouseEnter();
            }
            (*i)->mouseMove(e.x - bb.x1, e.y - bb.y1);
            return;
        }
    }
    if (currentWidget)
        currentWidget->mouseLeave();
    currentWidget = 0;
}

void Engine::execCommand(const std::wstring& what)
{
    if(what.empty())
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
            showFPS = enable;
            logger() << LINFO << L"FPS label " << (showFPS ? (L"enabled") : (L"disabled")) << LEND;
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
                Sprite *s = new Sprite(t, &mainNode, (TextureScaling)scaling);
                mainNode.addChild(s);
		wstring objName = addObject(s, fileName);
                s->setGeometry(w, h);
                s->setPosition(x, y);
		logger() << LINFO << "Sprite \"" << objName << "\" loaded" << LEND;
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
                AnimatedSprite *s = new AnimatedSprite(t, &mainNode);
		s->setFrame(currentFrame);
                mainNode.addChild(s);
		wstring objName = addObject(s, fileName);
                s->setGeometry(w, h);
                s->setPosition(x, y);
                s->setTextureScaling((TextureScaling)scaling);
		logger() << LINFO << "Animation \"" << objName << "\" loaded" << LEND;
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
        logger() << LERROR << wstring(L"Unknown command: ") << command << LEND;

    if (args.fail())
        logger() << LERROR << L"Invalid arguments" << LEND;
}

