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

#ifndef RANGERS_RESOURCEMANAGER_H
#define RANGERS_RESOURCEMANAGER_H
#include "config.h"
#include "ResourceAdapter.h"
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

struct SDL_RWops;
namespace Rangers
{
class Texture;
class AnimatedTexture;
class Font;
class Sprite;
class AnimatedSprite;
class Object;
struct GAIAnimation;
class LuaWidget;
class Widget;

class RANGERS_ENGINE_API ResourceManager
{
public:
    void addRPKG(const std::wstring& path);
    void addDir(const std::wstring& path);
    void addMapping(const std::wstring& fileName);

    boost::shared_array<char> loadData(const std::wstring& name, size_t &size);
    boost::shared_ptr<std::istream> getFileStream(const std::wstring& name);
    SDL_RWops* getSDLRW(const std::wstring& name);

    boost::shared_ptr<Texture> loadTexture(const std::wstring& name);
    boost::shared_ptr<AnimatedTexture> loadAnimation(const std::wstring& name, bool backgroundLoading = false);
    boost::shared_ptr<Font> loadFont(const std::wstring& name, int size, bool antialiased = true);

    bool resourceExists(const std::wstring& path);

    static ResourceManager& instance();

    void processMain();

private:
    ResourceManager();
    ResourceManager(const ResourceManager& other);

    class GAIWorker;
    void processGAIQueue();
    void cleanupUnused();
    std::map<std::wstring, std::wstring> m_fileMapping;
    std::map<std::wstring, boost::shared_ptr<ResourceAdapter> > m_files;
    std::list<boost::shared_ptr<ResourceAdapter> > m_adapters;

    std::map<std::wstring, boost::shared_ptr<Texture> > m_textures;
    std::map<std::wstring, boost::shared_ptr<AnimatedTexture> > m_animations;
    std::map<std::wstring, boost::shared_ptr<Font> > m_fonts;
    std::map<boost::shared_ptr<AnimatedTexture>, GAIWorker *> m_gaiQueue;
};
}

#endif // RESOURCEMANAGER_H
