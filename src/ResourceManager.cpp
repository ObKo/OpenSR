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

#include "ResourceManager.h"
#include "RPKGAdapter.h"
#include "FSAdapter.h"
#include "Texture.h"
#include "AnimatedTexture.h"
#include "Log.h"
#include "Font.h"
#include <algorithm>

using namespace Rangers;
using namespace std;
ResourceManager *manager = 0;

ResourceManager* ResourceManager::instance()
{
    if (!manager)
        manager = new ResourceManager();
    return manager;
}

void ResourceManager::addRPKG(const std::wstring& path)
{
    boost::shared_ptr<RPKGAdapter> a = boost::shared_ptr<RPKGAdapter>(new RPKGAdapter());
    a->load(path);
    adaptors.push_back(a);
    list<wstring> adaptorFiles = a->getFiles();
    for (list<wstring>::const_iterator i = adaptorFiles.begin(); i != adaptorFiles.end(); ++i)
        files[(*i)] = a;
}

void ResourceManager::addDir(const std::wstring& path)
{
    boost::shared_ptr<FSAdapter> a = boost::shared_ptr<FSAdapter>(new FSAdapter());
    a->load(path);
    adaptors.push_back(a);
    list<wstring> adaptorFiles = a->getFiles();
    for (list<wstring>::const_iterator i = adaptorFiles.begin(); i != adaptorFiles.end(); ++i)
        files[(*i)] = a;
}


boost::shared_ptr<Texture> ResourceManager::loadTexture(const std::wstring& name)
{
    map<wstring, boost::shared_ptr<Texture> >::const_iterator it = textures.find(name);
    if(it != textures.end())
	return it->second;
    
    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gi")
    {
        size_t s;
        char *data = loadData(name, s);
        if (!data)
            return boost::shared_ptr<Texture>();
        GIFrame frame = loadGIFile(data);
        delete data;
        Texture *t = new Texture(frame.width, frame.height, Rangers::TEXTURE_R8G8B8A8, frame.data);
        delete frame.data;
	textures[name] = boost::shared_ptr<Texture>(t);
        return textures[name];
    }
    else
        logger() << LERROR << "Unknown texture format: " << sfx << LEND;

    return boost::shared_ptr<Texture>();
}

boost::shared_ptr<AnimatedTexture> ResourceManager::loadAnimation(const std::wstring& name)
{
    map<wstring, boost::shared_ptr<AnimatedTexture> >::const_iterator it = animations.find(name);
    if(it != animations.end())
	return it->second;
    
    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gai")
    {
        size_t s;
        char *data = loadData(name, s);
        if (!data)
            return boost::shared_ptr<AnimatedTexture>();

        GAIAnimation a = loadGAIAnimation(data);

        delete data;
        AnimatedTexture *t = new AnimatedTexture(a);
        for (int i = 0; i < a.frameCount; i++)
            delete a.frames[i].data;
        delete[] a.frames;
	animations[name] = boost::shared_ptr<AnimatedTexture>(t);
        return animations[name];
    }
    else
        logger() << LERROR << "Unknown animation format: " << sfx << LEND;

    return boost::shared_ptr<AnimatedTexture>();
}

boost::shared_ptr< Font > ResourceManager::loadFont(const std::wstring& name, int size)
{
    wostringstream s(name);
    s.seekp(0, ios_base::end);
    s << size;
    wstring mapName = s.str();
    
    map<wstring, boost::shared_ptr<Font> >::const_iterator it = fonts.find(mapName);
    if(it != fonts.end())
	return it->second;
    
    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    
    if (sfx == L"ttf")
    {
        size_t dataSize;
	char *data = loadData(name, dataSize);
	if(!data)
	    return boost::shared_ptr<Font>();
	
	Font *f = new Font(data, dataSize, size);
	delete data;
	fonts[mapName] = boost::shared_ptr<Font>(f);
	return fonts[mapName];
    }
    else
        logger() << LERROR << "Unknown font format: " << sfx << LEND;
    
}


void ResourceManager::processMain()
{
    for(map<std::wstring, boost::shared_ptr<Texture> >::iterator i = textures.begin(); i != textures.end(); i++)
	if((*i).second.use_count() < 2)
	    textures.erase(i);
	
    for(map<std::wstring, boost::shared_ptr<AnimatedTexture> >::iterator i = animations.begin(); i != animations.end(); i++)
	if((*i).second.use_count() < 2)
	    animations.erase(i);
}

char* ResourceManager::loadData(const std::wstring& name, size_t &size)
{
    if (files.find(name) == files.end())
    {
        logger() << LERROR << "No such file: " << name << LEND;
        return 0;
    }
    return files[name]->loadData(name, size);
}
