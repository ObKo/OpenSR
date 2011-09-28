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
#include "AnimatedSprite.h"
#include "Object.h"
#include "GAISprite.h"

using namespace std;

namespace Rangers
{
ResourceManager *manager = 0;

class ResourceManager::GAIWorker
{
public:
    GAIWorker(char *gaiData, char *bgData);
    ~GAIWorker();
    void run();

    GAIAnimation animation() const;
    bool loaded() const;
    void cleanFrame(int i);

    static void loadAnimation(GAIWorker *worker);

private:
    char *m_gaiData;
    char *m_bgFrameData;
    GAIAnimation m_animation;
    boost::thread *m_thread;
    bool m_loaded;
};

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
        size_t s;
        char *data = loadData(name, s);
        if (!data)
            return boost::shared_ptr<Texture>();
        GIFrame frame = loadGIFile(data);
        delete data;
        Texture *t = new Texture(frame.width, frame.height, Rangers::TEXTURE_B8G8R8A8, frame.data);
        delete frame.data;
        m_textures[name] = boost::shared_ptr<Texture>(t);
        return m_textures[name];
    }
    else if (sfx == L"dds")
    {
        size_t size;
        char *data = loadData(name, size);
        if (!data)
            return boost::shared_ptr<Texture>();
        if (*(uint32_t*)(data) != 0x20534444)
        {
            Log::error() << "Invalid dds file.";
            return boost::shared_ptr<Texture>();
        }
        DDSHeader header = *((DDSHeader*)(data + 4));
        int dxtSize = size - sizeof(DDSHeader) - 4;
        char *dxt = data + sizeof(DDSHeader) + 4;

        if (!(header.ddspf.flags & DDPF_FOURCC))
        {
            Log::warning() << "Unsupported DDS file";
            delete data;
            return boost::shared_ptr<Texture>();
        }

        Texture *t = 0;
        switch (header.ddspf.fourCC)
        {
        case 0x31545844:
            t = new Texture(header.width, header.height, Rangers::TEXTURE_DXT1, (unsigned char *)dxt, dxtSize);
            delete data;
            break;
        case 0x33545844:
            t = new Texture(header.width, header.height, Rangers::TEXTURE_DXT3, (unsigned char *)dxt, dxtSize);
            delete data;
            break;
        case 0x35545844:
            t = new Texture(header.width, header.height, Rangers::TEXTURE_DXT5, (unsigned char *)dxt, dxtSize);
            delete data;
            break;
        default:
            Log::warning() << "Unsupported DDS file";
            delete data;
            return boost::shared_ptr<Texture>();
            break;
        }
        return boost::shared_ptr<Texture>(t);
    }
    else
        Log::error() << "Unknown texture format: " << sfx;

    return boost::shared_ptr<Texture>();
}

GAISprite *ResourceManager::loadDeltaAnimation(const std::wstring& name, Object *parent)
{
    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gai")
    {
        size_t s;
        char *data = loadData(name, s);
        if (!data)
            return 0;

        char *bgFrameData = 0;
        GAIHeader header = loadGAIHeader(data);

        if (!header.haveBackground)
        {
            Log::error() << "Unsupported gai format";
            return 0;
        }

        size_t size;
        bgFrameData = loadData(directory(name) + L"/" + basename(name) + L".gi", size);
        if (!bgFrameData)
            return 0;

        GIFrame bgFrame = loadGIFile(bgFrameData);
        delete bgFrameData;
        GAISprite *bgSprite = new GAISprite(data, s, bgFrame, parent);
        return bgSprite;
    }
    else
        Log::error() << "Unknown animation format: " << sfx;

    return 0;
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
        size_t s;
        char *data = loadData(name, s);
        if (!data)
            return boost::shared_ptr<AnimatedTexture>();

        char *bgFrameData = 0;
        GAIHeader header = loadGAIHeader(data);

        if (header.haveBackground)
        {
            size_t size;
            bgFrameData = loadData(directory(name) + L"/" + basename(name) + L".gi", size);
            if (!bgFrameData)
                return boost::shared_ptr<AnimatedTexture>();
        }

        if (backgroundLoading)
        {
            AnimatedTexture *t = new AnimatedTexture(header.finishX - header.startX,
                    header.finishY - header.startY,
                    header.waitSeek, header.waitSize,
                    header.frameCount);

            m_animations[name] = boost::shared_ptr<AnimatedTexture>(t);
            GAIWorker *worker = new GAIWorker(data, bgFrameData);
            m_gaiQueue[m_animations[name]] = worker;
            worker->run();
        }
        else
        {
            GAIWorker worker(data, bgFrameData);
            worker.loadAnimation(&worker);
            m_animations[name] = boost::shared_ptr<AnimatedTexture>(new AnimatedTexture(worker.animation()));
        }
        return m_animations[name];
    }
    else if (sfx == L"dds")
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
    }
    else
        Log::error() << "Unknown animation format: " << sfx;

    return boost::shared_ptr<AnimatedTexture>();
}

boost::shared_ptr< Font > ResourceManager::loadFont(const std::wstring& name, int size)
{
    wostringstream s(name);
    s.seekp(0, ios_base::end);
    s << size;
    wstring mapName = s.str();

    map<wstring, boost::shared_ptr<Font> >::const_iterator it = m_fonts.find(mapName);
    if (it != m_fonts.end())
        return it->second;

    wstring sfx = suffix(name);
    transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);

    if (sfx == L"ttf")
    {
        size_t dataSize;
        char *data = loadData(name, dataSize);
        if (!data)
            return boost::shared_ptr<Font>();

        Font *f = new Font(data, dataSize, size);
        delete data;
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

char* ResourceManager::loadData(const std::wstring& name, size_t &size)
{
    if (m_files.find(name) == m_files.end())
    {
        Log::error() << "No such file: " << name;
        return 0;
    }
    return m_files[name]->loadData(name, size);
}

void ResourceManager::processGAIQueue()
{
    std::list<boost::shared_ptr<AnimatedTexture> > animationsToRemove;

    for (std::map<boost::shared_ptr<AnimatedTexture>, GAIWorker*>::iterator i = m_gaiQueue.begin(); i != m_gaiQueue.end(); i++)
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
    for (std::list<boost::shared_ptr<AnimatedTexture> >::const_iterator i = animationsToRemove.begin(); i != animationsToRemove.end(); i++)
        m_gaiQueue.erase(*i);
    animationsToRemove.clear();
}

void ResourceManager::cleanupUnused()
{
    std::list<std::wstring> animationsToRemove;
    std::list<std::wstring> texturesToRemove;

    for (map<std::wstring, boost::shared_ptr<Texture> >::iterator i = m_textures.begin(); i != m_textures.end(); i++)
        if ((*i).second.use_count() < 2)
            texturesToRemove.push_back(i->first);

    for (map<std::wstring, boost::shared_ptr<AnimatedTexture> >::iterator i = m_animations.begin(); i != m_animations.end(); i++)
        if ((*i).second.use_count() < 2)
            animationsToRemove.push_back(i->first);


    for (std::list<std::wstring>::const_iterator i = animationsToRemove.begin(); i != animationsToRemove.end(); i++)
        m_animations.erase(*i);
    for (std::list<std::wstring>::const_iterator i = texturesToRemove.begin(); i != texturesToRemove.end(); i++)
        m_textures.erase(*i);
}

AnimatedSprite ResourceManager::getAnimatedSprite(const std::wstring& name, bool backgroundLoading, Object *parent)
{
    return AnimatedSprite(loadAnimation(name, backgroundLoading), parent);
}

Sprite ResourceManager::getSprite(const std::wstring& name, Object *parent)
{
    return Sprite(loadTexture(name), parent);
}

ResourceManager::GAIWorker::GAIWorker(char* gaiData, char* bgData)
{
    m_bgFrameData = bgData;
    m_gaiData = gaiData;
    m_loaded = false;
    m_thread = 0;
}

void ResourceManager::GAIWorker::run()
{
    m_thread = new boost::thread(loadAnimation, this);
}

void ResourceManager::GAIWorker::loadAnimation(GAIWorker *w)
{
    GIFrame *bg = 0;
    if (w->m_bgFrameData)
    {
        bg = new GIFrame();
        (*bg) = loadGIFile(w->m_bgFrameData);
        delete w->m_bgFrameData;
    }

    w->m_animation = loadGAIAnimation(w->m_gaiData, bg);
    delete[] w->m_gaiData;

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
}


