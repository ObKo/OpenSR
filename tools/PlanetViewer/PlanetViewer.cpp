/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#include "PlanetViewer.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <math.h>
#include <limits>
#include <QFileInfo>

#include <GL/gl.h>
#include <GL/glu.h>

namespace OpenSR
{
struct Vertex
{
    float x, y;
    float u, v;
};

namespace
{
const char *vertexShader = ""
                           "varying vec2 tex_coord;"
                           "void main()"
                           "{"
                           "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
                           "    tex_coord = vec2(gl_MultiTexCoord0);"
                           "}";
}

PlanetViewer::PlanetViewer(QWidget *parent) :
    QGLWidget(parent)
{
    m_textureID = 0;
    m_cloudTextureID = 0;
    m_cloudPhase = 0.0f;
    m_phase = 0.0f;
    m_phaseSpeed = 15.0f;
    m_cloudPhaseSpeed = 10.0f;
    m_planetSize = 256;
    m_cloudEnabled = false;
    m_ringEnabled = false;
    m_ringBackgroundEnabled = false;
    m_ambientColor = 0xffffffff;
    m_ringBuffer = 0;
    m_ringTextureID = 0;
    m_ringBackgroundBuffer = 0;
    m_ringBackgroundTextureID = 0;
    m_ringBackgroundOffsetX = 0.0f;
    m_ringBackgroundOffsetY = 0.0f;
    m_ringOffsetX = 0.0f;
    m_ringOffsetY = 0.0f;

    m_timer.setInterval(30);
    m_timer.setSingleShot(false);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(rotatePlanet()));
    m_timer.start();
}

void PlanetViewer::setCloudTexture(const QString& cloud)
{
    m_cloudTexture = loadImage(cloud);
    deleteTexture(m_cloudTextureID);
    m_cloudTextureID = bindTexture(m_cloudTexture);
}

void PlanetViewer::setTexture(const QString& texture)
{
    m_texture = loadImage(texture);
    deleteTexture(m_textureID);
    m_textureID = bindTexture(m_texture);
}

void PlanetViewer::setRingTexture(const QString& ring)
{
    m_ringTexture = loadImage(ring);
    deleteTexture(m_ringTextureID);
    m_ringTextureID = bindTexture(m_ringTexture);
    updateSize();
}

bool PlanetViewer::ringBackgroundEnabled() const
{
    return m_ringBackgroundEnabled;
}

void PlanetViewer::setRingOffset(float x, float y)
{
    m_ringOffsetX = x;
    m_ringOffsetY = y;
}

void PlanetViewer::setRingBackgroundOffset(float x, float y)
{
    m_ringBackgroundOffsetX = x;
    m_ringBackgroundOffsetY = y;
}

void PlanetViewer::setRingBackground(const QString& ring)
{
    m_ringBackgroundTexture = loadImage(ring);
    deleteTexture(m_ringBackgroundTextureID);
    m_ringBackgroundTextureID = bindTexture(m_ringBackgroundTexture);
    updateSize();
}

void PlanetViewer::setRingBackgroundEnabled(bool enabled)
{
    m_ringBackgroundEnabled = enabled;
}

bool PlanetViewer::ringEnabled() const
{
    return m_ringEnabled;
}

void PlanetViewer::setRingEnabled(bool enabled)
{
    m_ringEnabled = enabled;
}

bool PlanetViewer::cloudEnabled() const
{
    return m_cloudEnabled;
}

void PlanetViewer::setCloudEnabled(bool enabled)
{
    m_cloudEnabled = enabled;
}

double PlanetViewer::planetSpeed() const
{
    return m_phaseSpeed;
}

double PlanetViewer::cloudSpeed() const
{
    return m_cloudPhaseSpeed;
}

void PlanetViewer::setPlanetSpeed(double speed)
{
    m_phaseSpeed = speed;
}

void PlanetViewer::setCloudSpeed(double speed)
{
    m_cloudPhaseSpeed = speed;
}

double PlanetViewer::solarAngle() const
{
    return m_solarAngle;
}

void PlanetViewer::setSolarAngle(double angle)
{
    m_solarAngle = angle;
}

QColor PlanetViewer::ambientColor() const
{
    return QColor::fromRgb(m_ambientColor);
}

void PlanetViewer::setAmbientColor(const QColor& color)
{
    m_ambientColor = color.rgb();
}

void PlanetViewer::initializeGL()
{
#ifdef Q_OS_WIN32
    glewInit();
#endif
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_ARB_texture_non_power_of_two);
    glEnable(GL_EXT_texture_filter_anisotropic);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Vertex *vertices = new Vertex[4];

    memset(vertices, 0, 4 * sizeof(Vertex));

    glGenBuffers(1, &m_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &m_ringBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_ringBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &m_ringBackgroundBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_ringBackgroundBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_DYNAMIC_DRAW);
    delete[] vertices;

    updateSize();

    GLuint vShader;
    GLuint fShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vShader);
    glAttachShader(m_shaderProgram, fShader);

    QFile fShaderFile(":/planetFragmentShader.frag");
    fShaderFile.open(QIODevice::ReadOnly);

    QByteArray data = fShaderFile.readAll();

    char *fShaderData = new char[data.size() + 1];
    memcpy(fShaderData, data.data(), data.size() + 1);

    glShaderSource(vShader, 1, &vertexShader, NULL);
    glShaderSource(fShader, 1, (const char**)&fShaderData, NULL);

    glCompileShader(vShader);
    glCompileShader(fShader);

    fShaderFile.close();

    GLint compiled;

    glGetObjectParameterivARB(vShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
        qCritical() << "Error compiling vertex shader";
    glGetObjectParameterivARB(fShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        int logLength;
        char *log;
        glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLength);
        log = new char[logLength];
        glGetShaderInfoLog(fShader, logLength, 0, log);
        qCritical() << "Error compiling fragment shader:" << log;
        delete[] log;
    }

    glLinkProgram(m_shaderProgram);

    m_phaseArgument = glGetUniformLocation(m_shaderProgram, "phase");
    m_textureArgument = glGetUniformLocation(m_shaderProgram, "texture");
    m_cloudArgument = glGetUniformLocation(m_shaderProgram, "cloud");
    m_cloudPhaseArgument = glGetUniformLocation(m_shaderProgram, "cloudPhase");
    m_cloudEnableArgument = glGetUniformLocation(m_shaderProgram, "cloudEnabled");
    m_bumpMappingArgument = glGetUniformLocation(m_shaderProgram, "bumpMapping");
    m_solarAngleArgument = glGetUniformLocation(m_shaderProgram, "solarAngle");
    m_ambientColorArgument = glGetUniformLocation(m_shaderProgram, "ambientColor");
    m_pixelSizeArgument = glGetUniformLocation(m_shaderProgram, "pixelSize");
    m_texPixelSizeArgument = glGetUniformLocation(m_shaderProgram, "texPixelSize");
}

void PlanetViewer::updateSize()
{
    Vertex *vertices;

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vertices[0].x = - m_planetSize / 2.0f;
    vertices[0].y = - m_planetSize / 2.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    vertices[1].x = m_planetSize / 2.0f;
    vertices[1].y = - m_planetSize / 2.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;
    vertices[2].x = m_planetSize / 2.0f;
    vertices[2].y = m_planetSize / 2.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;
    vertices[3].x = - m_planetSize / 2.0f;
    vertices[3].y = m_planetSize / 2.0f;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_ringBuffer);
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vertices[0].x = - m_ringTexture.width() / 2.0f;
    vertices[0].y = - m_ringTexture.height() / 2.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 1.0f;
    vertices[1].x = m_ringTexture.width() / 2.0f;
    vertices[1].y = - m_ringTexture.height() / 2.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 1.0f;
    vertices[2].x = m_ringTexture.width() / 2.0f;
    vertices[2].y = m_ringTexture.height() / 2.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 0.0f;
    vertices[3].x = - m_ringTexture.width() / 2.0f;
    vertices[3].y = m_ringTexture.height() / 2.0f;
    vertices[3].u = 0.0f;
    vertices[3].v = 0.0f;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_ringBackgroundBuffer);
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vertices[0].x = - m_ringBackgroundTexture.width() / 2.0f;
    vertices[0].y = - m_ringBackgroundTexture.height() / 2.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 1.0f;
    vertices[1].x = m_ringBackgroundTexture.width() / 2.0f;
    vertices[1].y = - m_ringBackgroundTexture.height() / 2.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 1.0f;
    vertices[2].x = m_ringBackgroundTexture.width() / 2.0f;
    vertices[2].y = m_ringBackgroundTexture.height() / 2.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 0.0f;
    vertices[3].x = - m_ringBackgroundTexture.width() / 2.0f;
    vertices[3].y = m_ringBackgroundTexture.height() / 2.0f;
    vertices[3].u = 0.0f;
    vertices[3].v = 0.0f;

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void PlanetViewer::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (m_ringBackgroundEnabled)
    {
        glPushMatrix();
        glTranslatef(m_ringBackgroundOffsetX, m_ringBackgroundOffsetY, 0.0f);
        glUseProgram(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ringBackgroundTextureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, m_ringBackgroundBuffer);

        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (char*)0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (char*)0 + sizeof(float) * 2);

        glDrawArrays(GL_QUADS, 0, 4);

        glDisableClientState(GL_ARRAY_BUFFER);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }

    glUseProgram(m_shaderProgram);

    glUniform1f(m_cloudPhaseArgument, (float)m_cloudPhase);
    glUniform1f(m_phaseArgument, (float)m_phase);
    glUniform1i(m_textureArgument, 0);
    glUniform1i(m_cloudArgument, 1);
    glUniform1i(m_cloudEnableArgument, m_cloudEnabled);
    glUniform1i(m_bumpMappingArgument, m_bumpMapping);
    glUniform1f(m_solarAngleArgument, (float)m_solarAngle);

    glUniform3f(m_ambientColorArgument, ((m_ambientColor >> 16) & 0xff) / 255.0f,
                ((m_ambientColor >> 8) & 0xff) / 255.0f, ((m_ambientColor) & 0xff) / 255.0f);
    glUniform1f(m_pixelSizeArgument, 1.0 / m_planetSize);
    glUniform2f(m_texPixelSizeArgument, 1.0 / m_texture.width(), 1.0 / m_texture.height());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_cloudTextureID);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (char*)0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (char*)0 + sizeof(float) * 2);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_ARRAY_BUFFER);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (m_ringEnabled)
    {
        glPushMatrix();
        glTranslatef(m_ringOffsetX, m_ringOffsetY, 0.0f);
        glUseProgram(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ringTextureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, m_ringBuffer);

        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (char*)0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (char*)0 + sizeof(float) * 2);

        glDrawArrays(GL_QUADS, 0, 4);

        glDisableClientState(GL_ARRAY_BUFFER);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
}

QImage PlanetViewer::loadImage(const QString& file)
{
    return QImage(file);
}

bool PlanetViewer::bumpMapping() const
{
    return m_bumpMapping;
}

void PlanetViewer::setBumpMapping(bool bump)
{
    m_bumpMapping = bump;
}

void PlanetViewer::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width / 2.0f, width / 2.0f, height / 2.0f, -height / 2.0f, -1.0f, 1.0f);
}

QRgb getColor(const QImage &texture, int px, int py, int size, float radius, bool cloud)
{
    float x = sqrt(radius * radius - (px - size / 2.0f) * (px - size / 2.0f) - (py - size / 2.0f) * (py - size / 2.0f));
    float r = sqrt(x * x + (px - size / 2.0f) * (px - size / 2.0f) + (py - size / 2.0f) * (py - size / 2.0f));
    float lambda = atan((px - size / 2.0f) / x);
    float theta = acos((py - size / 2.0f) / r);
    float rx, ry;
    rx = 0.5f + (1.0f / (2.0f * M_PI) * lambda);
    ry = 0.5f + cos(theta) / 2.0f;

    QRgb color;

    if ((r > radius - 0.001f) && (r <= radius + 0.001f) && (rx == rx) && (ry == ry))
    {
        color = texture.pixel(rx * texture.width(), ry * texture.height());

        if (!cloud)
        {
            float y = 0.299 * qRed(color) + 0.587 * qGreen(color) + 0.114 * qBlue(color);
            float u = -0.14713 * qRed(color) - 0.28886 * qGreen(color) + 0.436 * qBlue(color);
            float v = 0.615 * qRed(color) - 0.51499 * qGreen(color) - 0.10001 * qBlue(color);
            y = y * (127.0 + qAlpha(color) / 2.0f) / 255.0f;
            y = y > 255.0f ? 255.0f : (y < 0.0 ? 0.0f : y);
            u = u > 255.0f ? 255.0f : (u < 0.0 ? 0.0f : u);
            v = v > 255.0f ? 255.0f : (v < 0.0 ? 0.0f : v);
            color = qRgb((y + 1.13983 * v), (y - 0.39465 * u - 0.58060 * v), (y + 2.03211 * u));
        }
        color = qRgb(qRed(color), qGreen(color), qBlue(color));
    }
    else
        color = 0;

    return color;
}

void PlanetViewer::generatePlanetImage(QImage &image, const QImage &texture, int size)
{
    image.fill(Qt::NoAlpha);
    QRgb t0, t1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            float tr = sqrt((i - size / 2) * (i - size / 2) + (j - size / 2) * (j - size / 2));
            t0 = getColor(texture, i, j, size, size / 2.0f, false);

            if ((tr > sqrt(size / 2 * size / 2) - 2) && (tr <= sqrt(size / 2 * size / 2)))
                t0 = (t0 & 0xffffff) | (((int)round((sqrt(size / 2 * size / 2) - tr) * 64) << 24) & 0xff000000);

            image.setPixel(i, j, t0);
        }
    }
};

void PlanetViewer::rotatePlanet()
{
    m_phase += m_phaseSpeed / 180000.0f * m_timer.interval() * M_PI;
    m_cloudPhase += m_cloudPhaseSpeed / 180000.0f * m_timer.interval() * M_PI;
    while (m_phase > M_PI)
        m_phase -= 2 * M_PI;
    while (m_phase < -M_PI)
        m_phase += 2 * M_PI;
    while (m_cloudPhase > M_PI)
        m_cloudPhase -= 2 * M_PI;
    while (m_cloudPhase < -M_PI)
        m_cloudPhase += 2 * M_PI;
    repaint();
}

int PlanetViewer::planetSize() const
{
    return m_planetSize;
}

void PlanetViewer::setPlanetSize(int size)
{
    m_planetSize = size;
    updateSize();
}

/*void PlanetViewer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(0, 0, m_rendered);
}*/
}
