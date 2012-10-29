#include "PlanetViewer.h"

#define GL_GLEXT_PROTOTYPES
#define _USE_MATH_DEFINES

#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <math.h>
#include <limits>

#include <GL/gl.h>
#include <GL/glu.h>

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

void generatePlanetImage(QImage &image, const QImage &texture, int radius, double phase);

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
    m_cloudEnabled = true;

    m_timer.setInterval(30);
    m_timer.setSingleShot(false);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(rotatePlanet()));
    m_timer.start();
}

void PlanetViewer::setCloudTexture(const QPixmap& cloud)
{
    m_cloudTexture = cloud;
    glDeleteTextures(1, &m_cloudTextureID);
    m_cloudTextureID = bindTexture(m_cloudTexture);
}

void PlanetViewer::setTexture(const QPixmap& texture)
{
    m_texture = texture;
    glDeleteTextures(1, &m_textureID);
    m_textureID = bindTexture(m_texture);
}

void setTexture(const QPixmap& cloud);

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

void PlanetViewer::initializeGL()
{
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
        qCritical() << "Error compiling fragment shader";

    glLinkProgram(m_shaderProgram);

    m_phaseArgument = glGetUniformLocation(m_shaderProgram, "phase");
    m_textureArgument = glGetUniformLocation(m_shaderProgram, "texture");
    m_cloudArgument = glGetUniformLocation(m_shaderProgram, "cloud");
    m_cloudPhaseArgument = glGetUniformLocation(m_shaderProgram, "cloudPhase");
    m_cloudEnableArgument = glGetUniformLocation(m_shaderProgram, "cloudEnabled");
    m_colorCorrectionArgument = glGetUniformLocation(m_shaderProgram, "colorCorrection");
    m_solarAngleArgument = glGetUniformLocation(m_shaderProgram, "solarAngle");
}

void PlanetViewer::updateSize()
{
    Vertex *vertices;

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    vertices[0].x = 0.0f;
    vertices[0].y = 0.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;
    vertices[1].x = m_planetSize;
    vertices[1].y = 0.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;
    vertices[2].x = m_planetSize;
    vertices[2].y = m_planetSize;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;
    vertices[3].x = 0.0f;
    vertices[3].y = m_planetSize;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void PlanetViewer::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glUseProgram(m_shaderProgram);

    glUniform1f(m_cloudPhaseArgument, (float)m_cloudPhase);
    glUniform1f(m_phaseArgument, (float)m_phase);
    glUniform1i(m_textureArgument, 0);
    glUniform1i(m_cloudArgument, 1);
    glUniform1i(m_cloudEnableArgument, m_cloudEnabled);
    glUniform1i(m_colorCorrectionArgument, m_colorCorrection);
    glUniform1f(m_solarAngleArgument, (float)m_solarAngle);

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
}

bool PlanetViewer::colorCorrection() const
{
    return m_colorCorrection;
}

void PlanetViewer::setColorCorrection(bool correction)
{
    m_colorCorrection = correction;
}

void PlanetViewer::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
}

/*void generatePlanetImage(QImage &image, const QImage &texture, int radius, double phase)
{
    image.fill(Qt::NoAlpha);
    for (int z = -radius; z < radius; z++)
    {
        for (int y = -radius; y < radius; y++)
        {
            double x = sqrt(radius * radius - z * z - y * y);
            double r = sqrt(x * x + y * y + z * z);
            double lambda = atan(double(y) / x) + phase;
            double theta = acos(double(z) / r);

            while (lambda > M_PI)
                lambda -= 2 * M_PI;

            while (lambda < -M_PI)
                lambda += 2 * M_PI;

            double u = texture.width() / 2 - ((texture.width() - 1) / (2.0 * M_PI) * lambda);
            double v = (texture.height() - 1) * theta / (M_PI);

            if ((u >= texture.width()) || (v >= texture.height()) || (u < 0) || (v < 0) || (v != v) || (u != u))
                continue;

            if ((r > radius - 0.5) && (r < radius + 0.5))
                image.setPixel(radius + y, radius + z, texture.pixel(round(u), round(v)));
        }
    }
}*/

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
