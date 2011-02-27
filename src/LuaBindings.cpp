#include "LuaBindings.h"
#include "libRanger.h"

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
