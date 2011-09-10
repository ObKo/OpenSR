#include "LuaBindings.h"
#include "libRanger.h"
#include "Log.h"

using namespace Rangers;

std::wstring fromLua(const char *s)
{
    return Rangers::fromUTF8(s);
}

Font* Rangers::getPointer(boost::shared_ptr<Font> sp)
{
    return sp.get();
}

Texture* Rangers::getPointer(boost::shared_ptr<Texture> sp)
{
    return sp.get();
}

AnimatedTexture* Rangers::getPointer(boost::shared_ptr<AnimatedTexture> sp)
{
    return sp.get();
}

void Rangers::luaDebug(std::wstring s)
{
     Log::debug() << L"Lua: " << s;
}

void Rangers::freePointer(boost::shared_ptr<Font> *sp)
{
    delete sp;
}

void Rangers::freePointer(boost::shared_ptr<Texture> *sp)
{
    delete sp;
}

void Rangers::freePointer(boost::shared_ptr<AnimatedTexture> *sp)
{
    delete sp;
}
