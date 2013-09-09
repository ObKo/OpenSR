/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include "PlanetManager.h"
#include "Types.h"
#include "WorldManager.h"
#include <libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <cmath>

namespace
{
const uint32_t PLANET_MANAGER_SIGNATURE = *((uint32_t*)"SRPM");
}

namespace Rangers
{
namespace World
{
PlanetManager::PlanetManager()
{

}

PlanetManager::PlanetManager(const PlanetManager& other)
{

}

bool PlanetManager::deserialize(std::istream& stream)
{
    return true;
}

bool PlanetManager::serialize(std::ostream& stream) const
{
    return true;
}

void PlanetManager::dropImageCache()
{
    m_imageCache.clear();
}

boost::shared_ptr< Texture > PlanetManager::getPlanetImage(uint32_t style, int size)
{
    boost::shared_ptr<PlanetStyle> s = WorldManager::instance().styleManager().planetStyle(style);
    return getPlanetImage(s, size);
}

//TODO: Planet rings
/*! This function must be run in OpenGL thread (main thread) !*/
boost::shared_ptr<Texture> PlanetManager::getPlanetImage(boost::shared_ptr<PlanetStyle> style, int size)
{
    if (!style)
        return boost::shared_ptr<Texture>();

    uint64_t cacheID = (uint64_t(textHash32(style->id)) << 32) | (size & 0xffffffff);

    std::map<uint64_t, boost::shared_ptr<Texture> >::const_iterator cache = m_imageCache.find(cacheID);
    if (cache != m_imageCache.end())
        return cache->second;

    boost::shared_ptr<Texture> texture;
    boost::shared_ptr<Texture> cloud;

    texture = ResourceManager::instance().loadTexture(style->texture);

    if (!texture)
        return boost::shared_ptr<Texture>();

    if (style->hasCloud)
        cloud = ResourceManager::instance().loadTexture(style->cloud);

    unsigned char *tex = new unsigned char[texture->width() * texture->height() * 4];
    unsigned char *cld = 0;
    unsigned char *img = new unsigned char[size * size * 4];

    if (style->hasCloud && cloud)
    {
        cld = new unsigned char[cloud->width() * cloud->height() * 4];
        glBindTexture(GL_TEXTURE_2D, cloud->openGLTexture());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_BYTE, cld);
    }


    glBindTexture(GL_TEXTURE_2D, texture->openGLTexture());
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_BYTE, tex);


    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float tx = j / (float(size) - 1.0f) - 0.5f;
            float ty = i / (float(size) - 1.0f) - 0.5f;
            float tr = tx * tx + ty * ty;

            float x = sqrt(0.25f - tx * tx - ty * ty);
            float r = sqrt(x * x + tx * tx + ty * ty);
            float lambda = atan(tx / x);
            float theta = acos(-ty / r);

            int tex_x = (0.5f + (1.0f / (2.0f * M_PI) * lambda)) * (texture->width() - 1);
            int tex_y = (0.5f - cos(theta) / 2.0f) * (texture->height() - 1);
            if (tr <= 0.25 && tex_x < texture->width() && tex_y < texture->height() && tex_x >= 0 && tex_y >= 0)
            {
                float a = tex[(tex_y * texture->width() + tex_x) * 4 + 3] / 255.0f;
                float b = tex[(tex_y * texture->width() + tex_x) * 4 + 2] / 255.0f;
                float g = tex[(tex_y * texture->width() + tex_x) * 4 + 1] / 255.0f;
                float r = tex[(tex_y * texture->width() + tex_x) * 4] / 255.0f;
                float y = 0.299f * r + 0.587f * g + 0.114f * b;
                float u = -0.14713f * r - 0.28886f * g + 0.436f * b;
                float v = 0.615f * r - 0.51499f * g - 0.10001f * b;
                y = y * (0.5f + a / 2.0f);
                r = y + 1.13983f * v;
                g = y - 0.39465f * u - 0.58060f * v;
                b = y + 2.03211f * u;
                a = 1.0f;

                if (style->hasCloud && cloud)
                {
                    int cld_x = tex_x * (float(cloud->width()) / texture->width());
                    int cld_y = tex_y * (float(cloud->height()) / texture->height());
                    float ca = cld[(cld_y * cloud->width() + cld_x) * 4 + 3] / 255.0f;
                    float cb = cld[(cld_y * cloud->width() + cld_x) * 4 + 2] / 255.0f;
                    float cg = cld[(cld_y * cloud->width() + cld_x) * 4 + 1] / 255.0f;
                    float cr = cld[(cld_y * cloud->width() + cld_x) * 4] / 255.0f;

                    float outa = ca + a * (1.0f - ca);
                    float outr = (cr * ca + r * a * (1 - ca));
                    float outg = (cg * ca + g * a * (1 - ca));
                    float outb = (cb * ca + b * a * (1 - ca));

                    r = outr;
                    g = outg;
                    b = outb;
                    a = outa;
                }

                r *= 1.3f + ((style->ambientColor >> 16) & 0xff) / 255.0f * 0.5f;
                g *= 1.3f + ((style->ambientColor >> 8) & 0xff) / 255.0f * 0.5f;
                b *= 1.3f + (style->ambientColor & 0xff) / 255.0f * 0.5f;

                if (tr >= 0.23)
                    a = (0.25f - tr) * 50.0f;
                else
                    a = 1.0f;

                r = r > 1.0f ? 1.0f : r < 0.0f ? 0.0f : r;
                g = g > 1.0f ? 1.0f : g < 0.0f ? 0.0f : g;
                b = b > 1.0f ? 1.0f : b < 0.0f ? 0.0f : b;
                ((uint32_t*)img)[i * size + j] = ((char(a * 255.0) << 24) & 0xff000000) | ((char(b * 255.0) << 16) & 0xff0000) |
                                                 ((char(g * 255.0) << 8) & 0xff00) | (char(r * 255.0) & 0xff);
            }
            else
                ((uint32_t*)img)[i * size + j] = 0;
        }
    }
    boost::shared_ptr<Texture> result = boost::shared_ptr<Texture>(new Texture(size, size, TEXTURE_R8G8B8A8, img));
    delete[] img;
    delete[] tex;
    if (cld)
        delete[] cld;

    m_imageCache[cacheID] = result;

    return result;
}
}
}
