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

#include "SystemPlanetWidget.h"
#include "PlanetManager.h"
#include "Planet.h"
#include "WorldManager.h"
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/Sprite.h>
#include <OpenSR/Engine.h>
#include <OpenSR/libRanger.h>
#include <cmath>

namespace
{
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
boost::shared_ptr<Shader> SystemPlanetWidget::m_vertexShader;
boost::shared_ptr<Shader> SystemPlanetWidget::m_fragmentShader;
boost::shared_ptr<ShaderProgram> SystemPlanetWidget::m_shader;

GLint SystemPlanetWidget::m_phaseLocation;
GLint SystemPlanetWidget::m_cloudPhaseLocation;
GLint SystemPlanetWidget::m_solarAngleLocation;
GLint SystemPlanetWidget::m_ambientColorLocation;
GLint SystemPlanetWidget::m_cloudEnabledLocation;
GLint SystemPlanetWidget::m_textureLocation;
GLint SystemPlanetWidget::m_cloudLocation;
GLint SystemPlanetWidget::m_texPixelSizeLocation;
GLint SystemPlanetWidget::m_pixelSizeLocation;

SystemPlanetWidget::SystemPlanetWidget(boost::shared_ptr<Planet> planet): SpaceObjectWidget(planet), m_useShader(true), m_vertexBuffer(0),
    m_phase(0.0f), m_cloudPhase(0.0f), m_solarAngle(0.0f), m_hasRingBackground(false), m_hasCloud(false), m_hasRing(false)
{
    m_useShader = Rangers::Engine::instance().properties()->get<bool>("graphics.useShaders", true);
    if (m_useShader && (!m_vertexShader || !m_fragmentShader || !m_shader))
    {
        std::string fragShaderFile = Engine::instance().properties()->get<std::string>("world.planetShader", "World/Planet.frag");
        m_vertexShader = boost::shared_ptr<Shader>(new Shader(Shader::VERTEX_SHADER, PLANET_VERTEX_SHADER));
        m_fragmentShader = boost::shared_ptr<Shader>(new Shader(fragShaderFile));
        m_shader = boost::shared_ptr<ShaderProgram>(new ShaderProgram());
    }
    if (planet)
    {
        boost::shared_ptr<PlanetStyle> style = WorldManager::instance().styleManager().planetStyle(planet->style());
        if (style)
        {
            m_texture = ResourceManager::instance().loadTexture(style->texture);
            m_hasCloud = style->hasCloud;
            m_ambientColor = style->ambientColor;
            m_speed = style->speed;
            m_cloudSpeed = style->cloudSpeed;
            m_size = planet->radius() *  2.0f;
            if (m_hasCloud)
                m_cloud = ResourceManager::instance().loadTexture(style->cloud);
            m_hasRing = style->hasRing;
            if (m_hasRing)
            {
                m_ringSprite = boost::shared_ptr<Sprite>(new Sprite(style->ring));
                m_ringSprite->setOrigin(POSITION_X_CENTER, POSITION_Y_CENTER);
                m_ringSprite->setPosition(style->ringOffsetX, style->ringOffsetY);
                addChild(m_ringSprite);
            }
            m_hasRingBackground = style->hasRingBackground;
            if (m_hasRingBackground)
            {
                m_ringBgSprite = boost::shared_ptr<Sprite>(new Sprite(style->ringBackground));
                m_ringBgSprite->setOrigin(POSITION_X_CENTER, POSITION_Y_CENTER);
                m_ringBgSprite->setPosition(style->ringBgOffsetX, style->ringBgOffsetY);
                addChild(m_ringBgSprite);
            }
        }
        updatePosition();
    }
    setWidth(m_size);
    setHeight(m_size);
    markToUpdate();
}

SystemPlanetWidget::~SystemPlanetWidget()
{
    if (m_vertexBuffer)
        glDeleteBuffers(1, &m_vertexBuffer);
}

void SystemPlanetWidget::draw() const
{
    if (!m_vertexBuffer)
        return;
    if (!prepareDraw())
        return;

    if (m_hasRingBackground && m_ringBgSprite)
        m_ringBgSprite->draw();

    if (m_useShader)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        if (m_texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture->openGLTexture());
        }

        if (m_cloud)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_cloud->openGLTexture());
        }
        glUseProgram(m_shader->handle());
        m_shader->setUniform(m_textureLocation, 0);
        m_shader->setUniform(m_cloudLocation, 1);
        m_shader->setUniform(m_phaseLocation, m_phase);
        m_shader->setUniform(m_cloudPhaseLocation, m_cloudPhase);
        m_shader->setUniform(m_solarAngleLocation, m_solarAngle);
        m_shader->setUniform(m_ambientColorLocation, m_ambientColor);
        m_shader->setUniform(m_cloudEnabledLocation, m_hasCloud);
        m_shader->setUniform(m_texPixelSizeLocation, Vector(1.0f / m_texture->width(), 1.0f / m_texture->height()));
        m_shader->setUniform(m_pixelSizeLocation, 1.0f / m_size);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), 0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), OPENGL_BUFFER_OFFSET(sizeof(float) * 2));

        glDrawArrays(GL_QUADS, 0, 4);

        glUseProgram(0);
        glActiveTexture(GL_TEXTURE0);


        glDisableClientState(GL_ARRAY_BUFFER);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else
    {
        boost::shared_ptr<Sprite> s = sprite();
        if (s)
            s->draw();
    }

    if (m_hasRing && m_ringSprite)
        m_ringSprite->draw();

    endDraw();
}

bool SystemPlanetWidget::containsPoint(const Vector& p) const
{
    return ((p.x) * (p.x) + (p.y) * (p.y)) < (m_size * m_size / 4.0f);
}

void SystemPlanetWidget::processMain()
{
    boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(m_object);

    if (m_useShader && !m_shader->isLinked() && !m_shader->isInvalid())
    {
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

    boost::shared_ptr<Sprite> s = sprite();
    if (!m_useShader && !s)
    {
        s = boost::shared_ptr<Sprite>(new Sprite(WorldManager::instance().planetManager().getPlanetImage(WorldManager::instance().styleManager().planetStyle(planet->style()), (int)m_size)));
        s->setPosition(int(- m_size / 2.0f), int(- m_size / 2.0f));
        setSprite(s);
    }
    Vertex *vertices;
    if (!m_vertexBuffer)
    {
        vertices = new Vertex[4];
        glGenBuffers(1, &m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_DYNAMIC_DRAW);
        delete[] vertices;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    float a = int(m_size / 2.0f);
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    vertices[0].x = -a;
    vertices[0].y = -a;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;
    vertices[1].x = a;
    vertices[1].y = -a;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;
    vertices[2].x = a;
    vertices[2].y = a;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;
    vertices[3].x = -a;
    vertices[3].y = a;
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void SystemPlanetWidget::processLogic(int dt)
{
    updatePosition();
    boost::shared_ptr<Planet> planet = boost::dynamic_pointer_cast<Planet>(m_object);

    if (planet)
        m_solarAngle = planet->angle() + M_PI;

    m_phase += m_speed / 180000.0f * dt * M_PI;
    while (m_phase > M_PI)
        m_phase -= 2 * M_PI;
    while (m_phase < -M_PI)
        m_phase += 2 * M_PI;

    m_cloudPhase += m_cloudSpeed / 180000.0f * dt * M_PI;
    while (m_cloudPhase > M_PI)
        m_cloudPhase -= 2 * M_PI;
    while (m_cloudPhase < -M_PI)
        m_cloudPhase += 2 * M_PI;
}

boost::shared_ptr<Planet> SystemPlanetWidget::planet() const
{
    return boost::dynamic_pointer_cast<Planet>(m_object);
}
}
}