/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#include "GAISprite.h"
#include "Log.h"
#include "Texture.h"
#include <cstdlib>
#include "ResourceManager.h"

#include "private/GAISprite_p.h"

extern void drawF5ToBGRA(unsigned char * bufdes, int bufdesll, const unsigned char * graphbuf);

namespace Rangers
{
GAISpritePrivate::GAISpritePrivate(): AnimatedSpritePrivate()
{
    m_needNextFrame = false;
    m_textureBuffer = 0;
    m_baseFrameHeight = 0;
    m_baseFrameWidth = 0;
}

GAISprite::GAISprite(Object *parent): AnimatedSprite(*(new GAISpritePrivate()), parent)
{
}

GAISprite::GAISprite(const char *data, int size, const GIFrame& baseFrame, Object *parent): AnimatedSprite(*(new GAISpritePrivate()), parent)
{
    RANGERS_D(GAISprite);
    d->loadGAI(data, size, baseFrame);
    markToUpdate();
}

GAISprite::GAISprite(const std::wstring& name, Object *parent): AnimatedSprite(*(new GAISpritePrivate()), parent)
{
    RANGERS_D(GAISprite);

    std::wstring sfx = suffix(name);
    std::transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gai")
    {
        size_t s;
        char *data = ResourceManager::instance()->loadData(name, s);
        char *bgFrameData = 0;

        if (data)
        {
            GAIHeader header = loadGAIHeader(data);

            if (!header.haveBackground)
            {
                Log::error() << "Unsupported gai format";
            }
            else
            {
                size_t size;
                bgFrameData = ResourceManager::instance()->loadData(directory(name) + basename(name) + L".gi", size);
                if (bgFrameData)
                {
                    GIFrame bgFrame = loadGIFile(bgFrameData);
                    delete[] bgFrameData;
                    d->loadGAI(data, s, bgFrame);
                }
            }
            delete[] data;
        }
    }
    else
        Log::error() << "Unknown animation format: " << sfx;

    markToUpdate();
}

GAISprite::GAISprite(const GAISprite & other): AnimatedSprite(*(new GAISpritePrivate()), other)
{
    RANGERS_D(GAISprite);
    d->m_gaiFrames.assign(other.d_func()->m_gaiFrames.begin(), other.d_func()->m_gaiFrames.end());
    d->m_needNextFrame = true;
    d->m_gaiHeader = other.d_func()->m_gaiHeader;

    d->m_baseFrame = other.d_func()->m_baseFrame;
    d->m_baseFrameWidth = other.d_func()->m_baseFrameWidth;
    d->m_baseFrameHeight = other.d_func()->m_baseFrameHeight;

    d->m_textureBuffer = 0;
    d->m_region = TextureRegion(boost::shared_ptr<Texture>(new Texture(d->m_width, d->m_height)));
    reset();
    d->m_animationStarted = other.d_func()->m_animationStarted;
    markToUpdate();
}


GAISprite::GAISprite(GAISpritePrivate &p, Object *parent): AnimatedSprite(p, parent)
{
}

GAISprite::GAISprite(GAISpritePrivate &p, const GAISprite& other): AnimatedSprite(p, other)
{
    RANGERS_D(GAISprite);
    d->m_gaiFrames.assign(other.d_func()->m_gaiFrames.begin(), other.d_func()->m_gaiFrames.end());
    d->m_needNextFrame = true;
    d->m_gaiHeader = other.d_func()->m_gaiHeader;

    d->m_baseFrame = other.d_func()->m_baseFrame;
    d->m_baseFrameWidth = other.d_func()->m_baseFrameWidth;
    d->m_baseFrameHeight = other.d_func()->m_baseFrameHeight;

    d->m_textureBuffer = 0;
    d->m_region = TextureRegion(boost::shared_ptr<Texture>(new Texture(d->m_width, d->m_height)));
    reset();
    d->m_animationStarted = other.d_func()->m_animationStarted;
    markToUpdate();
}

GAISprite::~GAISprite()
{
    RANGERS_D(GAISprite);
    if (d->m_textureBuffer)
        glDeleteBuffers(1, &d->m_textureBuffer);
}

GAISprite& GAISprite::operator=(const GAISprite & other)
{
    if (this == &other)
        return *this;

    RANGERS_D(GAISprite);

    AnimatedSprite::operator=(other);

    if (d->m_textureBuffer)
        glDeleteBuffers(1, &d->m_textureBuffer);

    d->m_gaiFrames.assign(other.d_func()->m_gaiFrames.begin(), other.d_func()->m_gaiFrames.end());
    d->m_needNextFrame = true;
    d->m_gaiHeader = other.d_func()->m_gaiHeader;

    d->m_baseFrame = other.d_func()->m_baseFrame;
    d->m_baseFrameWidth = other.d_func()->m_baseFrameWidth;
    d->m_baseFrameHeight = other.d_func()->m_baseFrameHeight;

    d->m_textureBuffer = 0;
    d->m_region = TextureRegion(boost::shared_ptr<Texture>(new Texture(d->m_width, d->m_height)));

    reset();
    d->m_animationStarted = other.d_func()->m_animationStarted;
    markToUpdate();

    return *this;
}

void GAISprite::processLogic(int dt)
{
    RANGERS_D(GAISprite);

    if (!d->m_gaiFrames.size())
        return;

    lock();
    if (d->m_animationStarted && d->m_frameDuration)
    {
        if ((d->m_animationTime > d->m_frameDuration) && (!d->m_needNextFrame))
        {
            d->m_currentFrame = (d->m_currentFrame + 1) % d->m_gaiHeader.frameCount;
            d->m_animationTime -= d->m_frameDuration;
            d->m_needNextFrame = true;
            markToUpdate();
        }

        d->m_animationTime += dt;
    }
    unlock();
}

void GAISprite::draw() const
{
    Sprite::draw();
}

void GAISprite::processMain()
{
    //FIXME: m_baseFrameWidth
    AnimatedSprite::processMain();
    lock();
    RANGERS_D(GAISprite);

    if (!d->m_gaiFrames.size())
    {
        unlock();
        return;
    }

    if (!d->m_textureBuffer)
    {
        glGenBuffers(1, &d->m_textureBuffer);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, d->m_textureBuffer);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, d->m_width * d->m_height * 4, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

    if (d->m_needNextFrame)
    {
        glBindTexture(GL_TEXTURE_2D, d->m_region.texture->openGLTexture());
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, d->m_textureBuffer);
        if (d->m_currentFrame == 0)
        {
            unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
            memset(data, 0, d->m_baseFrameWidth * d->m_baseFrameHeight * 4);
            memcpy(data, d->m_baseFrame.get(), d->m_baseFrameWidth * d->m_baseFrameHeight * 4);
            //copyImageData(data, m_baseFrameWidth, 0, 0, m_baseFrameWidth, m_baseFrameHeight, m_baseFrame.get());
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
            //FIXME: null as pointer to rgba looks ugly
            d->m_region.texture->setRawData(d->m_baseFrameWidth, d->m_baseFrameHeight, TEXTURE_B8G8R8A8, 0, 4 * d->m_baseFrameWidth * d->m_baseFrameHeight);
        }
        else
        {
            d->drawFrame(d->m_currentFrame);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, d->m_baseFrameWidth, d->m_baseFrameHeight, GL_BGRA, GL_UNSIGNED_BYTE, 0);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        d->m_needNextFrame = false;
    }
    unlock();
}

void GAISpritePrivate::loadGIFrame5(const char * data, unsigned char * background, int startX, int startY, int finishX, int finishY)
{
    const char *buffer = data;
    GIFrameHeader image = *((GIFrameHeader*)data);

    if (image.type != 5)
        return;

    image.layers = new GILayerHeader[image.layerCount];

    if (finishX)
        image.finishX = finishX;

    if (finishY)
        image.finishY = finishY;

    image.startX -= startX;
    image.startY -= startY;
    image.finishX -= startX;
    image.finishY -= startY;

    for (int i = 0; i < image.layerCount; i++)
    {
        buffer = data + 64 + i * 32;
        image.layers[i] = *((GILayerHeader*)buffer);
        buffer = data + image.layers[i].seek;
        //FIXME: const unsigned char* -> unsigned char*
        image.layers[i].data = (unsigned char*)buffer;

        image.layers[i].startX -= startX;
        image.layers[i].startY -= startY;
        image.layers[i].finishX -= startX;
        image.layers[i].finishY -= startY;
    }

    int width;
    int height;


    width = image.finishX;
    height = image.finishY;

    if (image.layers[0].size)
        drawF5ToBGRA(background + (image.layers[0].startY * width + image.layers[0].startX) * 4, width * 4, (const unsigned char *)image.layers[0].data);

    delete image.layers;
}

void GAISpritePrivate::drawFrame(int i)
{
    RANGERS_Q(GAISprite);
    unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    loadGIFrame5((char *)m_gaiFrames.at(i).get(), data, m_gaiHeader.startX, m_gaiHeader.startY, m_gaiHeader.finishX, m_gaiHeader.finishY);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
}

void GAISprite::setFrame(int f)
{
    Log::warning() << "Cannot set frame on GAISprite";
}

void GAISpritePrivate::loadGAI(const char * data, int size, const GIFrame& baseFrame)
{
    RANGERS_Q(GAISprite);
    m_gaiHeader = loadGAIHeader(data);

    unsigned char *baseFrameData = new unsigned char[baseFrame.width * baseFrame.height * 4];
    memcpy(baseFrameData, baseFrame.data, baseFrame.width * baseFrame.height * 4);
    m_baseFrame = boost::shared_array<unsigned char>(baseFrameData);

    m_baseFrameWidth = baseFrame.width;
    m_baseFrameHeight = baseFrame.height;

    if (m_gaiHeader.haveBackground)
    {
        m_width = m_gaiHeader.finishX - m_gaiHeader.startX;
        m_height = m_gaiHeader.finishY - m_gaiHeader.startY;
        int width = m_gaiHeader.finishX - m_gaiHeader.startX;
        int height = m_gaiHeader.finishY - m_gaiHeader.startY;

        //Background as frame #0
        m_gaiFrames.push_back(boost::shared_array<char>(0));

        for (int i = 0; i < m_gaiHeader.frameCount; i++)
        {
            uint32_t giSeek , giSize;
            const char *p = data + sizeof(GAIHeader) - sizeof(GIFrame *) + i * 2 * sizeof(uint32_t);
            giSeek = *((uint32_t *)p);
            p += sizeof(uint32_t);
            giSize = *((uint32_t *)p);
            p += sizeof(uint32_t);

            if (giSeek && giSize)
            {
                uint32_t signature;
                p = data + giSeek;
                signature = *((uint32_t *)p);

                if (signature == 0x31304c5a)
                {
                    size_t outsize;
                    m_gaiFrames.push_back(boost::shared_array<char>((char*)unpackZL01((const unsigned char*)p, giSize, outsize)));
                }
                else
                {
                    char *data = new char[giSize];
                    memcpy(data, p, giSize);
                    m_gaiFrames.push_back(boost::shared_array<char>(data));
                }
            }
        }

        m_animationStarted = true;
    }
    q->setFrameRate(15);
    m_region = TextureRegion(boost::shared_ptr<Texture>(new Texture(m_width, m_height)));
}

void GAISprite::reset()
{
    RANGERS_D(GAISprite);
    if (!d->m_gaiFrames.size())
        return;
    lock();
    d->m_animationStarted = false;
    d->m_currentFrame = 0;
    d->m_animationTime = 0;
    markToUpdate();
    unlock();
}

}
