#include "LuaBindings.h"
#include "libRanger.h"
#include "Log.h"
#include <fstream>
#include <string>
#include <cstring>
#include <cerrno>

namespace Rangers
{
int execScript(const std::wstring& fileName)
{
#ifdef WIN32
    std::ifstream stream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
#else
    std::ifstream stream(toLocal(fileName).c_str(), std::ifstream::in | std::ifstream::binary);
#endif
    if (!stream.is_open())
    {
        Log::error() << "Cannot open lua script file: " << strerror(errno);
        return -1;
    }
    stream.seekg(0, std::ios_base::end);
    size_t size = stream.tellg();
    stream.seekg(0, std::ios_base::beg);
    char *data = new char[size];
    stream.read(data, size);
    stream.close();
    int state = execScript(data, size, toLocal(basename(fileName) + L"." + suffix(fileName)));
    delete[] data;
    return state;
}

int execScript(const char *data, size_t size, const std::string& name)
{
    lua_State *luaState = lua_open();
    luaopen_base(luaState);
    luaopen_table(luaState);
    luaopen_math(luaState);
    tolua_Engine_open(luaState);
    tolua_libRanger_open(luaState);
    tolua_Object_open(luaState);
    tolua_Types_open(luaState);
    tolua_ResourceManager_open(luaState);
    tolua_Sprite_open(luaState);
    tolua_AnimatedSprite_open(luaState);
    tolua_LuaBindings_open(luaState);
    tolua_AnimatedTexture_open(luaState);
    tolua_LuaWidget_open(luaState);
    tolua_GAISprite_open(luaState);
    tolua_Button_open(luaState);
    int state;
    if (state = (luaL_loadbuffer(luaState, data, size, name.c_str()) || lua_pcall(luaState, 0, LUA_MULTRET, 0)))
        Log::error() << "Cannot exec lua script: " << lua_tostring(luaState, -1);
    return state;
}

std::wstring fromLua(const char *s)
{
    return Rangers::fromUTF8(s);
}

Font* getPointer(boost::shared_ptr<Font> sp)
{
    return sp.get();
}

Texture* getPointer(boost::shared_ptr<Texture> sp)
{
    return sp.get();
}

AnimatedTexture* getPointer(boost::shared_ptr<AnimatedTexture> sp)
{
    return sp.get();
}

GAISprite *getPointer(boost::shared_ptr<GAISprite> sp)
{
    return sp.get();
}

void luaDebug(std::wstring s)
{
    Log::debug() << L"Lua: " << s;
}

void luaWarning(std::wstring s)
{
    Log::warning() << L"Lua: " << s;
}

void luaError(std::wstring s)
{
    Log::error() << L"Lua: " << s;
}

void freePointer(boost::shared_ptr<Font> *sp)
{
    delete sp;
}

void freePointer(boost::shared_ptr<Texture> *sp)
{
    delete sp;
}

void freePointer(boost::shared_ptr<AnimatedTexture> *sp)
{
    delete sp;
}
}
