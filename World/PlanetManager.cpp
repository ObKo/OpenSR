/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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
#include <OpenSR/libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/Shader.h>
#include <OpenSR/ShaderProgram.h>
#include <OpenSR/Engine.h>
#include <cmath>

namespace
{
const uint32_t PLANET_MANAGER_SIGNATURE = *((uint32_t*)"SRPM");
static const std::string PLANET_VERTEX_SHADER = " \
varying vec2 texCoord; \
void main() \
{ \
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \
    texCoord = vec2(gl_MultiTexCoord0); \
}";
}

namespace Rangers
{
namespace World
{
PlanetManager::PlanetManager(): m_vertexBuffer(0), m_framebuffer(0)
{
    m_useShader = Rangers::Engine::instance().properties()->get<bool>("graphics.useShaders", true);
}

PlanetManager::~PlanetManager()
{
    if (m_vertexBuffer)
        glDeleteBuffers(1, &m_vertexBuffer);
    if (m_framebuffer)
        glDeleteFramebuffers(1, &m_framebuffer);
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


boost::shared_ptr<Texture> PlanetManager::renderHardware(int size, boost::shared_ptr<PlanetStyle> style)
{
    if (!style)
        return boost::shared_ptr<Texture>();

    if (!m_vertexShader || !m_fragmentShader || !m_shader ||
            (!m_shader->isLinked() && !m_shader->isInvalid()))
    {
        std::string fragShaderFile = Engine::instance().properties()->get<std::string>("world.planetShader", "World/Planet.frag");
        m_vertexShader = boost::shared_ptr<Shader>(new Shader(Shader::VERTEX_SHADER, PLANET_VERTEX_SHADER));
        m_fragmentShader = boost::shared_ptr<Shader>(new Shader(fragShaderFile));
        m_shader = boost::shared_ptr<ShaderProgram>(new ShaderProgram());

        m_shader->addShader(m_vertexShader);
        m_shader->addShader(m_fragmentShader);
        m_shader->link();

        if (m_shader->isInvalid())
        {
            m_useShader = false;
        }
        else
        {
            m_phaseLocation = m_shader->getUniformLocation("phase");
            m_cloudPhaseLocation = m_shader->getUniformLocation("cloudPhase");
            m_cloudEnabledLocation = m_shader->getUniformLocation("cloudEnabled");
            m_solarAngleLocation = m_shader->getUniformLocation("solarAngle");
            m_ambientColorLocation = m_shader->getUniformLocation("ambientColor");
            m_textureLocation = m_shader->getUniformLocation("texture");
            m_cloudLocation = m_shader->getUniformLocation("cloud");
            m_texPixelSizeLocation = m_shader->getUniformLocation("texPixelSize");
            m_pixelSizeLocation = m_shader->getUniformLocation("pixelSize");
        }
    }
    Vertex *vertices;
    if (!m_vertexBuffer)
    {
        vertices = new Vertex[4];
        vertices[0].u = 0.0f;
        vertices[0].v = 1.0f;
        vertices[0].x = 0.0;
        vertices[0].y = 0.0;
        vertices[1].u = 1.0f;
        vertices[1].v = 1.0f;
        vertices[1].x = 1.0;
        vertices[1].y = 0.0;
        vertices[2].u = 1.0f;
        vertices[2].v = 0.0f;
        vertices[2].x = 1.0;
        vertices[2].y = 1.0;
        vertices[3].u = 0.0f;
        vertices[3].v = 0.0f;
        vertices[3].x = 0.0;
        vertices[3].y = 1.0;
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);
        delete[] vertices;
    }

    if (!m_framebuffer)
    {
        glGenFramebuffers(1, &m_framebuffer);
    }

    boost::shared_ptr<Texture> texture;
    boost::shared_ptr<Texture> cloud;

    texture = ResourceManager::instance().loadTexture(style->texture);

    if (!texture)
        return boost::shared_ptr<Texture>();

    if (style->hasCloud)
        cloud = ResourceManager::instance().loadTexture(style->cloud);

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    boost::shared_ptr<Texture> result = boost::shared_ptr<Texture>(new Texture(size, size, TEXTURE_R8G8B8A8, 0));

    glBindTexture(GL_TEXTURE_2D, result->openGLTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, result->openGLTexture(), 0);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return boost::shared_ptr<Texture>();

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, size, size);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, 1.0, 1.0, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    if (texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->openGLTexture());
    }

    if (cloud)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cloud->openGLTexture());
    }
    glUseProgram(m_shader->handle());
    m_shader->setUniform(m_textureLocation, 0);
    m_shader->setUniform(m_cloudLocation, 1);
    m_shader->setUniform(m_phaseLocation, 0);
    m_shader->setUniform(m_cloudPhaseLocation, 0);
    m_shader->setUniform(m_solarAngleLocation, (float)M_PI);
    m_shader->setUniform(m_ambientColorLocation, style->ambientColor);
    m_shader->setUniform(m_texPixelSizeLocation, Vector(1.0f / texture->width(), 1.0f / texture->height()));
    m_shader->setUniform(m_pixelSizeLocation, 1.0f / size);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

    glDrawArrays(GL_QUADS, 0, 4);

    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return result;
}

boost::shared_ptr<Texture> PlanetManager::renderSoftware(int size, boost::shared_ptr<PlanetStyle> style)
{
    if (!style)
        return boost::shared_ptr<Texture>();

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

                r *= 1.3f + style->ambientColor.r * 0.5f;
                g *= 1.3f + style->ambientColor.g * 0.5f;
                b *= 1.3f + style->ambientColor.b * 0.5f;

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

    return result;
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

    boost::shared_ptr<Texture> result;

    if (m_useShader)
        result = renderHardware(size, style);
    else
        result = renderSoftware(size, style);

    m_imageCache[cacheID] = result;

    return result;
}
}
}
