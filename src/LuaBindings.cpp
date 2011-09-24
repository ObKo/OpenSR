#include "LuaBindings.h"
#include "libRanger.h"
#include "Log.h"

namespace Rangers
{
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
