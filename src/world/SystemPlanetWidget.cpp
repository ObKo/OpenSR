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

#include "SystemPlanetWidget.h"
#include "PlanetManager.h"
#include "Planet.h"
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Texture.h>
#include <OpenSR/Sprite.h>
#include <OpenSR/Engine.h>
#define _USE_MATH_DEFINES
#include <cmath>

namespace
{
static const std::string PLANET_FRAGMENT_SHADER = "\
varying vec2 tex_coord; \
uniform sampler2D cloud; \
uniform sampler2D texture; \
uniform float phase; \
uniform float cloudPhase; \
uniform bool cloudEnabled; \
uniform float solarAngle; \
uniform vec3 ambientColor; \
\
float PI = 3.14159265358979323846264; \
float tr = (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) + (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f); \
\
void getColor(in sampler2D tex, in float phase, in float radius, in bool cloud, out vec4 color) \
{ \
    float x = sqrt(radius * radius - (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) - (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f)); \
    float r = sqrt(x * x + (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) + (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f)); \
    float lambda = atan((tex_coord.x - 0.5f) / x) + phase; \
    float theta = acos((1.0f - tex_coord.y - 0.5f) / r); \
    vec2 result; \
    result.x = 0.5f + (1.0f / (2.0f * PI) * lambda); \
    result.y = 0.5f - cos(theta) / 2.0f; \
\
    if(tr <= radius * radius) \
    { \
        color = texture2D(tex, result); \
\
        if (!cloud) \
        { \
            float y = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b; \
            float u = -0.14713 * color.r - 0.28886 * color.g + 0.436 * color.b; \
            float v = 0.615 * color.r - 0.51499 * color.g - 0.10001 * color.b; \
            y = y * (0.5f + color.a / 2.0f); \
            color.r = y + 1.13983 * v; \
            color.g = y - 0.39465 * u - 0.58060 * v; \
            color.b = y + 2.03211 * u; \
            color.a = 1.0f; \
        } \
\
        if (!cloud) \
            color.a = 1.0f; \
    } \
    else \
        color = vec4(0.0f, 0.0f, 0.0f, 0.0f); \
} \
\
void main() \
{ \
    vec4 t0, t1; \
    if (cloudEnabled) \
    { \
        getColor(texture, phase, 0.5f, false, t0); \
        getColor(cloud, cloudPhase, 0.5f, true, t1); \
        gl_FragColor = mix(t0, t1, t1.a); \
\
        if (tr >= 0.23) \
            gl_FragColor.a = (0.25f - tr) * 50.0f; \
        else \
            gl_FragColor.a = 1.0f; \
    } \
    else \
    { \
        getColor(texture, phase, 0.5f, false, t0); \
        gl_FragColor = t0; \
\
        if (tr >= 0.23) \
            gl_FragColor.a = (0.25f - tr) * 50.0f; \
        else \
            gl_FragColor.a = 1.0f; \
    } \
    float x = sqrt(0.5f * 0.5f - (tex_coord.x - 0.5f) * (tex_coord.x - 0.5f) - (tex_coord.y - 0.5f) * (tex_coord.y - 0.5f)); \
    vec3 normal = normalize(vec3(x, tex_coord.x - 0.5f, tex_coord.y - 0.5f)); \
    float diffuse = max(dot(normal, vec3(0.5f, cos(solarAngle), sin(solarAngle))), 0.0); \
    gl_FragColor.rgb = gl_FragColor.rgb * 0.3 * ambientColor + gl_FragColor.rgb * diffuse * vec3(1.0, 1.0f, 1.0f); \
}";

static const std::string PLANET_VERTEX_SHADER = " \
varying vec2 tex_coord; \
void main() \
{ \
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \
    tex_coord = vec2(gl_MultiTexCoord0); \
}";
}

namespace Rangers
{
namespace World
{
Shader SystemPlanetWidget::m_vertexShader(Shader::VERTEX_SHADER, PLANET_VERTEX_SHADER);
Shader SystemPlanetWidget::m_fragmentShader(Shader::FRAGMENT_SHADER, PLANET_FRAGMENT_SHADER);
ShaderProgram SystemPlanetWidget::m_shader;

GLint SystemPlanetWidget::m_phaseLocation;
GLint SystemPlanetWidget::m_cloudPhaseLocation;
GLint SystemPlanetWidget::m_solarAngleLocation;
GLint SystemPlanetWidget::m_ambientColorLocation;
GLint SystemPlanetWidget::m_cloudEnabledLocation;
GLint SystemPlanetWidget::m_textureLocation;
GLint SystemPlanetWidget::m_cloudLocation;

SystemPlanetWidget::SystemPlanetWidget(boost::shared_ptr<Planet> planet, Widget* parent): Widget(parent), m_planet(planet), m_useShader(true), m_vertexBuffer(0),
    m_phase(0.0f), m_cloudPhase(0.0f), m_solarAngle(0.0f), m_ringSprite(0), m_ringBgSprite(0), m_staticSprite(0)
{
    m_useShader = Rangers::Engine::instance().properties()->get<bool>("graphics.useShaders", true);
    if (planet)
    {
        boost::shared_ptr<PlanetStyle> style = PlanetManager::instance().style(planet->style());
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
                m_ringSprite = new Sprite(style->ring, this);
                m_ringSprite->setOrigin(POSITION_X_CENTER, POSITION_Y_CENTER);
                m_ringSprite->setPosition(style->ringOffsetX + m_size / 2.0f, style->ringOffsetY + m_size / 2.0f);
            }
            m_hasRingBackground = style->hasRingBackground;
            if (m_hasRingBackground)
            {
                m_ringBgSprite = new Sprite(style->ringBackground, this);
                m_ringBgSprite->setOrigin(POSITION_X_CENTER, POSITION_Y_CENTER);
                m_ringBgSprite->setPosition(style->ringBgOffsetX + m_size / 2.0f, style->ringBgOffsetY + m_size / 2.0f);
            }
        }
        updatePosition();
    }
    setWidth(m_size);
    setHeight(m_size);
    markToUpdate();
}

void SystemPlanetWidget::updatePosition()
{
    if (!m_planet)
        return;

    setPosition(m_planet->position().x - m_size / 2.0f, m_planet->position().y - m_size / 2.0f);
}

SystemPlanetWidget::~SystemPlanetWidget()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    if (m_ringBgSprite)
        delete m_ringBgSprite;
    if (m_ringSprite)
        delete m_ringSprite;
    if (m_staticSprite)
        delete m_staticSprite;
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
        glUseProgram(m_shader.handle());
        glUniform1i(m_textureLocation, 0);
        glUniform1i(m_cloudLocation, 1);
        glUniform1f(m_phaseLocation, m_phase);
        glUniform1f(m_cloudPhaseLocation, m_cloudPhase);
        glUniform1f(m_solarAngleLocation, m_solarAngle);
        glUniform3f(m_ambientColorLocation, ((m_ambientColor >> 16) & 0xFF) / 255.0f, ((m_ambientColor >> 8) & 0xFF) / 255.0f, ((m_ambientColor) & 0xFF) / 255.0f);
        glUniform1i(m_cloudEnabledLocation, m_hasCloud);

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
        if (m_staticSprite)
            m_staticSprite->draw();
    }

    if (m_hasRing && m_ringSprite)
        m_ringSprite->draw();

    endDraw();
}

void SystemPlanetWidget::processMain()
{
    if (m_useShader && !m_shader.isLinked() && !m_shader.isInvalid())
    {
        m_shader.addShader(m_vertexShader);
        m_shader.addShader(m_fragmentShader);
        m_shader.link();

        if (m_shader.isInvalid())
        {
            m_useShader = false;
        }
        else
        {
            m_phaseLocation = m_shader.getUniformLocation("phase");
            m_cloudPhaseLocation = m_shader.getUniformLocation("cloudPhase");
            m_cloudEnabledLocation = m_shader.getUniformLocation("cloudEnabled");
            m_solarAngleLocation = m_shader.getUniformLocation("solarAngle");
            m_ambientColorLocation = m_shader.getUniformLocation("ambientColor");
            m_textureLocation = m_shader.getUniformLocation("texture");
            m_cloudLocation = m_shader.getUniformLocation("cloud");
        }
    }
    if (!m_useShader && !m_staticSprite)
    {
        m_staticSprite = new Sprite(PlanetManager::instance().getPlanetImage(PlanetManager::instance().style(m_planet->style()), (int)m_size), this);
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
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    vertices[0].x = 0.0f;
    vertices[0].y = 0.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;
    vertices[1].x = m_size;
    vertices[1].y = 0.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;
    vertices[2].x = m_size;
    vertices[2].y = m_size;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;
    vertices[3].x = 0.0f;
    vertices[3].y = m_size;
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void SystemPlanetWidget::processLogic(int dt)
{
    updatePosition();
    if (m_planet)
        m_solarAngle = m_planet->angle() + M_PI;

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
    return m_planet;
}
}
}