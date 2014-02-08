/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/ResourceManager.h"

#include <algorithm>
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <SDL_rwops.h>

#include "OpenSR/RPKGAdapter.h"
#include "OpenSR/FSAdapter.h"
#include "OpenSR/Texture.h"
#include "OpenSR/AnimatedTexture.h"
#include "OpenSR/Log.h"
#include "OpenSR/FTFont.h"
#include "OpenSR/AnimatedSprite.h"
#include "OpenSR/Object.h"
#include "OpenSR/GAISprite.h"

using namespace std;

namespace
{
int64_t istreamSeek(struct SDL_RWops *context, int64_t offset, int whence)
{
    std::istream* stream = (std::istream*)context->hidden.unknown.data1;

    if (!stream)
        return -1;

    if (whence == SEEK_SET)
        stream->seekg(offset, std::ios::beg);
    else if (whence == SEEK_CUR)
        stream->seekg(offset, std::ios::cur);
    else if (whence == SEEK_END)
        stream->seekg(offset, std::ios::end);

    return stream->fail() ? -1 : int(stream->tellg());
}


size_t istreamRead(SDL_RWops *context, void *ptr, size_t size, size_t maxnum)
{
    if (size == 0) return -1;
    std::istream* stream = (std::istream*) context->hidden.unknown.data1;

    if (!stream)
        return -1;

    stream->read((char*)ptr, size * maxnum);

    return stream->bad() ? -1 : stream->gcount() / size;
}

int istreamClose(SDL_RWops *context)
{
    if (context)
    {
        std::istream* stream = (std::istream*) context->hidden.unknown.data1;

        if (stream)
        {
            delete stream;
            context->hidden.unknown.data1 = 0;
        }
        SDL_FreeRW(context);
    }
    return 0;
}
}

namespace Rangers
{
ResourceManager::ResourceManager()
{
    m_datRoot = boost::shared_ptr<DATRecord>(new DATRecord(DATRecord::NODE));
}

ResourceManager::ResourceManager(const ResourceManager& other)
{
}

class ResourceManager::GAIWorker
{
public:
    GAIWorker(const GAIHeader& header, boost::shared_ptr<std::istream> gai, boost::shared_ptr<std::istream> bg);
    ~GAIWorker();
    void run();

    GAIAnimation animation() const;
    bool loaded() const;
    void cleanFrame(int i);

    static void loadAnimation(GAIWorker *worker);

private:
    boost::shared_ptr<std::istream> m_gai;
    boost::shared_ptr<std::istream> m_bgFrame;
    GAIAnimation m_animation;
    boost::thread *m_thread;
    GAIHeader m_header;
    bool m_loaded;
};

ResourceManager& ResourceManager::instance()
{
    static ResourceManager manager;
    return manager;
}

SDL_RWops *ResourceManager::getSDLRW(const std::wstring& name)
{
    wstring realName = name;

    std::map<std::wstring, boost::shared_ptr<ResourceAdapter> >::iterator fileIt = m_files.find(realName);
    if (fileIt == m_files.end())
    {
        Log::error() << "No such file: " << realName;
        return 0;
    }

    std::istream *s = fileIt->second->getStream(realName);

    SDL_RWops *rwops;
    rwops = SDL_AllocRW();

    if (rwops != NULL)
    {
        rwops->seek = istreamSeek;
        rwops->read = istreamRead;
        rwops->write = 0;
        rwops->close = istreamClose;
        rwops->hidden.unknown.data1 = s;
    }
    return rwops;
}

void ResourceManager::addRPKG(const std::wstring& path)
{
    boost::shared_ptr<RPKGAdapter> a = boost::shared_ptr<RPKGAdapter>(new RPKGAdapter());
    a->load(path);
    m_adapters.push_back(a);
    list<wstring> adaptorFiles = a->getFiles();
    for (list<wstring>::const_iterator i = adaptorFiles.begin(); i != adaptorFiles.end(); ++i)
        m_files[(*i)] = a;
}

void ResourceManager::addDir(const std::wstring& path)
{
    boost::shared_ptr<FSAdapter> a = boost::shared_ptr<FSAdapter>(new FSAdapter());
    a->load(path);
    m_adapters.push_back(a);
    list<wstring> adaptorFiles = a->getFiles();
    for (list<wstring>::const_iterator i = adaptorFiles.begin(); i != adaptorFiles.end(); ++i)
        m_files[(*i)] = a;
}


boost::shared_ptr<Texture> ResourceManager::loadTexture(const std::wstring& name)
{
    map<wstring, boost::shared_ptr<Texture> >::const_iterator it = m_textures.find(name);
    if (it != m_textures.end())
        return it->second;

    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gi")
    {
        boost::shared_ptr<std::istream> s = getFileStream(name);
        if (!s)
            return boost::shared_ptr<Texture>();
        GIFrame frame = loadGIFrame(*s);

        Texture *t;
        if (frame.format == GIFrame::Format_RGB16)
            t = new Texture(frame.width, frame.height, Rangers::TEXTURE_R5G6B5, frame.data);
        else
            t = new Texture(frame.width, frame.height, Rangers::TEXTURE_B8G8R8A8, frame.data);

        delete[] frame.data;
        m_textures[name] = boost::shared_ptr<Texture>(t);
        return m_textures[name];
    }
    else if (sfx == L"png")
    {
        boost::shared_ptr<std::istream> stream = getFileStream(name);
        if (!stream)
            return boost::shared_ptr<Texture>();
        PNGFrame frame = loadPNG(*stream);
        if (frame.type == PNGFrame::TYPE_INVALID)
        {
            Log::warning() << "Invalid/unsupported PNG file";
            return boost::shared_ptr<Texture>();
        }
        TextureType type;
        switch (frame.type)
        {
        case PNGFrame::TYPE_GRAY:
            //FIXME: grayscale as alpha
            type = Rangers::TEXTURE_A8;
            break;
        case PNGFrame::TYPE_RGB:
            type = Rangers::TEXTURE_R8G8B8;
            break;
        case PNGFrame::TYPE_RGBA:
            type = Rangers::TEXTURE_R8G8B8A8;
            break;
        }
        Texture *t = new Texture(frame.width, frame.height, type, frame.data);
        delete[] frame.data;
        m_textures[name] = boost::shared_ptr<Texture>(t);
        return m_textures[name];
    }
    else if (sfx == L"gai")
    {
        boost::shared_ptr<std::istream> s = getFileStream(name);
        if (!s)
            return boost::shared_ptr<Texture>();

        boost::shared_ptr<std::istream> bgFrameStream;
        GAIHeader header = loadGAIHeader(*s);

        s->seekg(0, ios_base::beg);

        if (header.haveBackground)
        {
            return loadTexture(directory(name) + basename(name) + L".gi");
        }

        GAIAnimation animation = loadGAIAnimation(*s, header);
        m_textures[name] = boost::shared_ptr<Texture>(
                               new Texture(animation.frames[0].width,
                                           animation.frames[0].height,
                                           Rangers::TEXTURE_B8G8R8A8,
                                           animation.frames[0].data)
                           );

        for (int i = 0; i < animation.frameCount; i++)
            delete[] animation.frames[i].data;
        delete[] animation.frames;
        delete[] animation.times;
        return m_textures[name];
    }
    else if (sfx == L"jpg" || sfx == L"jpeg")
    {
        boost::shared_ptr<std::istream> stream = getFileStream(name);
        if (!stream)
            return boost::shared_ptr<Texture>();
        JPEGFrame frame = loadJPEG(*stream);
        if (frame.type == PNGFrame::TYPE_INVALID)
        {
            Log::warning() << "Invalid/unsupported PNG file";
            return boost::shared_ptr<Texture>();
        }
        TextureType type;
        switch (frame.type)
        {
        case JPEGFrame::TYPE_GRAY:
            //FIXME: grayscale as alpha
            type = Rangers::TEXTURE_A8;
            break;
        case JPEGFrame::TYPE_RGB:
            type = Rangers::TEXTURE_R8G8B8;
            break;
        }
        Texture *t = new Texture(frame.width, frame.height, type, frame.data);
        delete[] frame.data;
        m_textures[name] = boost::shared_ptr<Texture>(t);
        return m_textures[name];
    }
    else
        Log::error() << "Unknown texture format: " << sfx;

    return boost::shared_ptr<Texture>();
}

boost::shared_ptr<AnimatedTexture> ResourceManager::loadAnimation(const std::wstring& name, bool backgroundLoading)
{
    map<wstring, boost::shared_ptr<AnimatedTexture> >::const_iterator it = m_animations.find(name);
    if (it != m_animations.end())
        return it->second;

    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gai")
    {
        boost::shared_ptr<std::istream> s = getFileStream(name);
        if (!s)
            return boost::shared_ptr<AnimatedTexture>();

        boost::shared_ptr<std::istream> bgFrameStream;
        GAIHeader header = loadGAIHeader(*s);

        s->seekg(0, ios_base::beg);

        if (header.haveBackground)
        {
            bgFrameStream = getFileStream(directory(name) + basename(name) + L".gi");
            if (!bgFrameStream)
                return boost::shared_ptr<AnimatedTexture>();
        }

        if (backgroundLoading)
        {
            AnimatedTexture *t = new AnimatedTexture(header.finishX - header.startX,
                    header.finishY - header.startY,
                    header.waitSeek, header.waitSize,
                    header.frameCount);

            m_animations[name] = boost::shared_ptr<AnimatedTexture>(t);
            GAIWorker *worker = new GAIWorker(header, s, bgFrameStream);
            m_gaiQueue[m_animations[name]] = worker;
            worker->run();
        }
        else
        {
            GAIWorker worker(header, s, bgFrameStream);
            worker.loadAnimation(&worker);
            m_animations[name] = boost::shared_ptr<AnimatedTexture>(new AnimatedTexture(worker.animation()));
        }
        return m_animations[name];
    }
    /*else if (sfx == L"dds")
    {
        size_t s;
        char *data = loadData(name, s);
        if (!data)
            return boost::shared_ptr<AnimatedTexture>();

        if (*(uint32_t*)(data) != 0x20534444)
        {
            Log::error() << "Invalid dds file.";
            return boost::shared_ptr<AnimatedTexture>();
        }
        DDSHeader header = *((DDSHeader*)(data + 4));

        if (!(header.ddspf.flags & DDPF_FOURCC))
        {
            Log::warning() << "Unsupported DDS file";
            delete data;
            return boost::shared_ptr<AnimatedTexture>();
        }

        if (!((header.caps & DDSCAPS_COMPLEX) && (header.caps2 & DDSCAPS2_VOLUME)
                && (header.flags & DDSD_LINEARSIZE) && (header.flags & DDSD_LINEARSIZE)))
        {
            Log::warning() << "Unsupported DDS file";
            delete data;
            return boost::shared_ptr<AnimatedTexture>();
        }

        switch (header.ddspf.fourCC)
        {
        case 0x31545844:
        case 0x33545844:
        case 0x35545844:
            break;
        default:
            Log::warning() << "Unsupported DDS file";
            delete data;
            return boost::shared_ptr<AnimatedTexture>();
            break;
        }

        AnimatedTexture *t = new AnimatedTexture(header.width, header.height, header.reserved1[0], header.reserved1[1], header.depth);
        for (int i = 0; i < header.depth; i++)
        {
            char *dxt = data + sizeof(DDSHeader) + 4 + (header.pitchOrLinearSize * header.height) * i;
            switch (header.ddspf.fourCC)
            {
            case 0x31545844:
                t->loadFrame(dxt, header.width, header.height, Rangers::TEXTURE_DXT1, header.pitchOrLinearSize * header.height);
                break;
            case 0x33545844:
                t->loadFrame(dxt, header.width, header.height, Rangers::TEXTURE_DXT3, header.pitchOrLinearSize * header.height);
                break;
            case 0x35545844:
                t->loadFrame(dxt, header.width, header.height, Rangers::TEXTURE_DXT5, header.pitchOrLinearSize * header.height);
                break;
            }
        }
        return boost::shared_ptr<AnimatedTexture>(t);
        delete data;
    }*/
    else
        Log::error() << "Unknown animation format: " << sfx;

    return boost::shared_ptr<AnimatedTexture>();
}

boost::shared_ptr< Font > ResourceManager::loadFont(const FontDescriptor& desc)
{
    return loadFont(desc.path, desc.size, desc.antialiasing);
}

boost::shared_ptr< Font > ResourceManager::loadFont(const std::wstring& name, int size, bool antialiased)
{
    wostringstream s(name);
    s.seekp(0, ios_base::end);
    s << L":" << size << L":" << antialiased;
    wstring mapName = s.str();

    map<wstring, boost::shared_ptr<Font> >::const_iterator it = m_fonts.find(mapName);
    if (it != m_fonts.end())
        return it->second;

    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);

    if (sfx == L"ttf")
    {
        boost::shared_ptr<std::istream> s = getFileStream(name);
        if (!s)
            return boost::shared_ptr<Font>();

        s->seekg(0, std::ios_base::end);
        uint32_t dataSize = s->tellg();
        s->seekg(0, std::ios_base::beg);

        char *data = new char[dataSize];
        s->read(data, dataSize);

        Font *f = new FTFont(data, dataSize, size, antialiased);
        delete[] data;
        m_fonts[mapName] = boost::shared_ptr<Font>(f);
        return m_fonts[mapName];
    }
    else
        Log::error() << "Unknown font format: " << sfx;

    return boost::shared_ptr<Font>();
}

void ResourceManager::processMain()
{
    cleanupUnused();
    processGAIQueue();
}

boost::shared_array<char> ResourceManager::loadData(const std::wstring& name, size_t &size)
{
    boost::shared_ptr<std::istream> s = ResourceManager::instance().getFileStream(name);
    if (!s)
        return boost::shared_array<char>();

    s->seekg(0, std::ios_base::end);
    size = s->tellg();
    if (!size)
        return boost::shared_array<char>();

    char *data = new char[size];
    s->seekg(0, std::ios_base::beg);
    s->read(data, size);

    return boost::shared_array<char>(data);
}

bool ResourceManager::resourceExists(const std::wstring& path)
{
    wstring realName = path;

    std::map<std::wstring, boost::shared_ptr<ResourceAdapter> >::iterator fileIt = m_files.find(realName);
    if (fileIt == m_files.end())
        return false;
    else
        return true;
}


boost::shared_ptr<std::istream> ResourceManager::getFileStream(const std::wstring& name)
{
    wstring realName = name;

    std::map<std::wstring, boost::shared_ptr<ResourceAdapter> >::iterator fileIt = m_files.find(realName);
    if (fileIt == m_files.end())
    {
        Log::error() << "No such file: " << realName;
        return boost::shared_ptr<std::istream>();
    }
    return boost::shared_ptr<std::istream>(fileIt->second->getStream(realName));
}

void ResourceManager::processGAIQueue()
{
    std::list<boost::shared_ptr<AnimatedTexture> > animationsToRemove;

    for (std::map<boost::shared_ptr<AnimatedTexture>, GAIWorker*>::iterator i = m_gaiQueue.begin(); i != m_gaiQueue.end(); ++i)
    {
        boost::shared_ptr<AnimatedTexture> t = (*i).first;
        GAIWorker *w = (*i).second;
        if (!w->loaded())
            continue;

        if (t->needFrames())
        {
            int f = t->loadedFrames();
            t->loadFrame((char *)w->animation().frames[f].data,
                         w->animation().frames[f].width,
                         w->animation().frames[f].height, TEXTURE_B8G8R8A8);
            w->cleanFrame(f);
            if (t->loadedFrames() >= t->frameCount())
            {
                delete w;
                animationsToRemove.push_back(i->first);
            }
        }
    }
    for (std::list<boost::shared_ptr<AnimatedTexture> >::const_iterator i = animationsToRemove.begin(); i != animationsToRemove.end(); ++i)
        m_gaiQueue.erase(*i);
    animationsToRemove.clear();
}

void ResourceManager::cleanupUnused()
{
    std::list<std::wstring> animationsToRemove;
    std::list<std::wstring> texturesToRemove;
    std::list<std::wstring> fontsToRemove;

    map<std::wstring, boost::shared_ptr<Texture> >::iterator texturesEnd = m_textures.end();
    for (map<std::wstring, boost::shared_ptr<Texture> >::iterator i = m_textures.begin(); i != texturesEnd; ++i)
        if ((*i).second.use_count() < 2)
            texturesToRemove.push_back(i->first);

    map<std::wstring, boost::shared_ptr<AnimatedTexture> >::iterator animEnd = m_animations.end();
    for (map<std::wstring, boost::shared_ptr<AnimatedTexture> >::iterator i = m_animations.begin(); i != animEnd; ++i)
        if ((*i).second.use_count() < 2)
            animationsToRemove.push_back(i->first);

    std::map<std::wstring, boost::shared_ptr<Font> >::iterator fontEnd = m_fonts.end();
    for (map<std::wstring, boost::shared_ptr<Font> >::iterator i = m_fonts.begin(); i != fontEnd; ++i)
        if ((*i).second.use_count() < 2)
            fontsToRemove.push_back(i->first);


    std::list<std::wstring>::const_iterator end = animationsToRemove.end();
    for (std::list<std::wstring>::const_iterator i = animationsToRemove.begin(); i != end; ++i)
    {
        Log::debug() << "Cleanup " << *i;
        m_animations.erase(*i);
    }
    end = texturesToRemove.end();
    for (std::list<std::wstring>::const_iterator i = texturesToRemove.begin(); i != end; ++i)
    {
        Log::debug() << "Cleanup " << *i;
        m_textures.erase(*i);
    }
    end = fontsToRemove.end();
    for (std::list<std::wstring>::const_iterator i = fontsToRemove.begin(); i != end; ++i)
    {
        Log::debug() << "Cleanup " << *i;
        m_fonts.erase(*i);
    }
}

ResourceManager::GAIWorker::GAIWorker(const GAIHeader& header, boost::shared_ptr<std::istream> gai, boost::shared_ptr<std::istream> bg)
{
    m_bgFrame = bg;
    m_gai = gai;
    m_loaded = false;
    m_thread = 0;
    m_header = header;
}

void ResourceManager::GAIWorker::run()
{
    m_thread = new boost::thread(loadAnimation, this);
}

void ResourceManager::GAIWorker::loadAnimation(GAIWorker *w)
{
    GIFrame *bg = 0;
    if (w->m_bgFrame)
    {
        bg = new GIFrame();
        (*bg) = loadGIFrame(*(w->m_bgFrame), true, 0, 0, w->m_header.startX, w->m_header.startY, w->m_header.finishX, w->m_header.finishY);
    }

    w->m_animation = loadGAIAnimation(*(w->m_gai), w->m_header, bg);
    w->m_loaded = true;
}

ResourceManager::GAIWorker::~GAIWorker()
{
    if (m_thread)
    {
        m_thread->join();
        delete m_thread;
    }
    for (int i = 0; i < m_animation.frameCount; i++)
        cleanFrame(i);
    delete[] m_animation.frames;
    delete[] m_animation.times;
}

GAIAnimation ResourceManager::GAIWorker::animation() const
{
    return m_animation;
}

bool ResourceManager::GAIWorker::loaded() const
{
    return m_loaded;
}

void ResourceManager::GAIWorker::cleanFrame(int i)
{
    delete[] m_animation.frames[i % m_animation.frameCount].data;
    m_animation.frames[i % m_animation.frameCount].data = 0;
}

ResourceObjectManager& ResourceManager::objectManager()
{
    return m_objectManager;
}

void ResourceManager::addDATFile(const std::wstring& name)
{
    boost::shared_ptr<std::istream> stream = getFileStream(name);
    if (!stream)
        return;

    DATRecord r = loadDAT(*stream);
    for (const DATRecord c : r)
    {
        if (m_datRoot->find(c.name) != m_datRoot->end())
            Log::warning() << "[DAT] Item \"" << c.name << "\" already exists in root.";

        m_datRoot->add(c);
    }
}

boost::shared_ptr<DATRecord> ResourceManager::datRoot()
{
    return m_datRoot;
}

}


