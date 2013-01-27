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
    needNextFrame = true;
    textureBuffer = 0;
    baseFrameHeight = 0;
    baseFrameWidth = 0;
}

GAISprite::GAISprite(Object *parent): AnimatedSprite(*(new GAISpritePrivate()), parent)
{
}

GAISprite::GAISprite(const std::wstring& name, Object *parent): AnimatedSprite(*(new GAISpritePrivate()), parent)
{
    RANGERS_D(GAISprite);

    std::wstring sfx = suffix(name);
    std::transform(sfx.begin(), sfx.end(), sfx.begin(), towlower);
    if (sfx == L"gai")
    {
        size_t s;
        boost::shared_ptr<std::istream> stream = ResourceManager::instance().getFileStream(name);

        if (stream)
        {
            GAIHeader header = loadGAIHeader(*stream);
            (*stream).seekg(0, std::ios_base::beg);

            if (!header.haveBackground)
            {
                Log::error() << "Unsupported gai format";
            }
            else
            {
                size_t size;
                boost::shared_ptr<std::istream> bgFrameStream = ResourceManager::instance().getFileStream(directory(name) + basename(name) + L".gi");
                if (bgFrameStream)
                {
                    GIFrame bgFrame = loadGIFile(*bgFrameStream);
                    d->loadGAI(stream, bgFrame);
                    delete[] bgFrame.data;
                }
            }
        }
    }
    else
        Log::error() << "Unknown animation format: " << sfx;

    markToUpdate();
}

GAISprite::GAISprite(GAISpritePrivate &p, Object *parent): AnimatedSprite(p, parent)
{
}

GAISprite::~GAISprite()
{
    RANGERS_D(GAISprite);
    if (d->textureBuffer)
        glDeleteBuffers(1, &d->textureBuffer);
}

void GAISprite::processLogic(int dt)
{
    RANGERS_D(GAISprite);

    if (!d->gaiFrames.size())
        return;

    lock();
    if (d->animationStarted && d->frameDuration)
    {
        if ((d->animationTime > d->frameDuration) && (!d->needNextFrame))
        {
            d->currentFrame = (d->currentFrame + 1) % d->gaiHeader.frameCount;
            d->animationTime -= d->frameDuration;
            d->needNextFrame = true;
            markToUpdate();
        }

        d->animationTime += dt;
    }
    unlock();
}

void GAISprite::draw() const
{
    Sprite::draw();
}

void GAISprite::processMain()
{
    //FIXME: baseFrameWidth
    AnimatedSprite::processMain();
    lock();
    RANGERS_D(GAISprite);

    if (!d->gaiFrames.size())
    {
        unlock();
        return;
    }

    if (!d->textureBuffer)
    {
        glGenBuffers(1, &d->textureBuffer);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, d->textureBuffer);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, d->width * d->height * 4, 0, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }

    if (d->needNextFrame)
    {
        glBindTexture(GL_TEXTURE_2D, d->region.texture->openGLTexture());
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, d->textureBuffer);
        if (d->currentFrame == 0)
        {
            unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
            memset(data, 0, d->baseFrameWidth * d->baseFrameHeight * 4);
            memcpy(data, d->baseFrame.get(), d->baseFrameWidth * d->baseFrameHeight * 4);
            //copyImageData(data, baseFrameWidth, 0, 0, baseFrameWidth, baseFrameHeight, baseFrame.get());
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
            //FIXME: null as pointer to rgba looks ugly
            d->region.texture->setRawData(d->baseFrameWidth, d->baseFrameHeight, TEXTURE_B8G8R8A8, 0, 4 * d->baseFrameWidth * d->baseFrameHeight);
        }
        else
        {
            d->drawFrame(d->currentFrame);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, d->baseFrameWidth, d->baseFrameHeight, GL_BGRA, GL_UNSIGNED_BYTE, 0);
        }
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        d->needNextFrame = false;
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

    delete[] image.layers;
}

void GAISpritePrivate::drawFrame(int i)
{
    RANGERS_Q(GAISprite);
    unsigned char* data = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    loadGIFrame5((char *)gaiFrames.at(i).get(), data, gaiHeader.startX, gaiHeader.startY, gaiHeader.finishX, gaiHeader.finishY);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
}

void GAISprite::setFrame(int f)
{
    Log::warning() << "Cannot set frame on GAISprite";
}

void GAISpritePrivate::loadGAI(boost::shared_ptr<std::istream> stream, const GIFrame& frame)
{
    RANGERS_Q(GAISprite);
    gaiHeader = loadGAIHeader(*stream);

    unsigned char *baseFrameData = new unsigned char[frame.width * frame.height * 4];
    memcpy(baseFrameData, frame.data, frame.width * frame.height * 4);
    baseFrame = boost::shared_array<unsigned char>(baseFrameData);

    baseFrameWidth = frame.width;
    baseFrameHeight = frame.height;

    if (gaiHeader.haveBackground)
    {
        width = gaiHeader.finishX - gaiHeader.startX;
        height = gaiHeader.finishY - gaiHeader.startY;
        int width = gaiHeader.finishX - gaiHeader.startX;
        int height = gaiHeader.finishY - gaiHeader.startY;

        //Background as frame #0
        gaiFrames.push_back(boost::shared_array<char>(0));

        for (int i = 0; i < gaiHeader.frameCount; i++)
        {
            uint32_t giSeek , giSize;
            (*stream).seekg(sizeof(GAIHeader) - sizeof(GIFrame *) + i * 2 * sizeof(uint32_t), std::ios_base::beg);
            (*stream).read((char *)&giSeek, 4);
            (*stream).read((char *)&giSize, 4);

            if (giSeek && giSize)
            {
                uint32_t signature;
                char *data = new char[giSize];

                (*stream).seekg(giSeek, std::ios_base::beg);
                (*stream).read(data, giSize);
                signature = *((uint32_t*)data);

                if (signature == 0x31304c5a)
                {
                    size_t outsize;
                    gaiFrames.push_back(boost::shared_array<char>((char*)unpackZL01((const unsigned char*)data, giSize, outsize)));
                    delete[] data;
                }
                else
                {
                    gaiFrames.push_back(boost::shared_array<char>(data));
                }
            }
        }

        animationStarted = true;
    }
    q->setFrameRate(15);
    region = TextureRegion(boost::shared_ptr<Texture>(new Texture(width, height)));
}

void GAISprite::reset()
{
    RANGERS_D(GAISprite);
    if (!d->gaiFrames.size())
        return;
    lock();
    d->animationStarted = false;
    d->currentFrame = 0;
    d->animationTime = 0;
    markToUpdate();
    unlock();
}

}
