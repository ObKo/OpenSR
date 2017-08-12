/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2017 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/PlanetDrawer.h"

#include "OpenSR/Engine.h"
#include "OpenSR/ResourceManager.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImage>
#include <QImageReader>

namespace OpenSR
{
namespace
{
static const QString FRAGMENT_SHADER =
    "varying vec2 texCoord;\n"
    "uniform sampler2D surface, cloud0, cloud1;\n"
    "uniform bool hasCloud0, hasCloud1;\n"
    "uniform float pixelSize;\n"
    "uniform float phase, cloud0Phase, cloud1Phase;\n"
    "uniform vec4 atmosphere;\n"
    "uniform float angle;\n"

    "const float atmSize = 0.0625;\n"

    "vec3 mapTo3D(vec2 tex)\n"
    "{\n"
    "    vec3 pos;\n"
    "    pos.x = (texCoord.x - 0.5) / 0.5;\n"
    "    pos.y = (0.5 - texCoord.y) / 0.5;\n"
    "    pos.z = sqrt(1.0 - pos.x * pos.x - pos.y * pos.y);\n"
    "    return pos;\n"
    "}\n"

    "vec2 mapTexture(vec3 pos, float phase)\n"
    "{\n"
    "    vec2 coord;\n"
    "    coord.x = atan(pos.x, pos.z) / (2.0 * 3.141593) - phase;\n"
    "    coord.y = 0.5 - pos.y / 2.0;\n"
    "    return coord;\n"
    "}\n"

    "vec4 blend(vec4 dst, vec4 src)\n"
    "{\n"
    "    return src + dst * (1.0 - src.a);\n"
    "}\n"

    "vec4 getCloud(vec3 pos, float light, int cloud)\n"
    "{\n"
    "    vec2 coord = mapTexture(pos, (cloud == 0) ? cloud0Phase : cloud1Phase);\n"
    "    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n"
    "    if ((cloud == 0) && hasCloud0)\n"
    "        color = texture2D(cloud0, coord);\n"
    "    else if ((cloud == 1) && hasCloud1)\n"
    "        color = texture2D(cloud1, coord);\n"
    "    return vec4(color.rgb * light * color.a, color.a);\n"
    "}\n"

    "void main()\n"
    "{\n"
    "    vec3 pos = mapTo3D(texCoord);\n"

    "    vec3 l = normalize(vec3(cos(angle), sin(angle), 0.5));\n"
    "    vec3 n = normalize(pos);\n"

    "    vec3 surfacePos = pos * (1.0 + atmSize);\n"
    "    float surfaceR = sqrt(surfacePos.x * surfacePos.x + surfacePos.y * surfacePos.y);\n"
    "    float surfaceAA = clamp((1.0 - surfaceR) / (2.0 * pixelSize), 0.0, 1.0);\n"
    "    vec2 surfaceCoord = mapTexture(surfacePos, phase);\n"
    "    vec4 surfaceColor = texture2D(surface, surfaceCoord);\n"
    "    float surfaceLight = 0.75 * max(dot(l, n), 0.0) * surfaceColor.a + 0.25;\n"
    "    surfaceColor = vec4(surfaceColor.rgb * surfaceLight, 1.0);\n"

    "    float atmosphereLight = 0.75 * max(dot(l, n), 0.0) + 0.25;\n"
    "    float atmosphereAlpha = (1.0 - clamp(abs(surfaceR - 1.0) / (atmSize), 0.0, 1.0));\n"
    "    vec4 atmosphereColor = vec4(atmosphere.rgb * atmosphereLight * atmosphereAlpha, atmosphereAlpha);\n"

    "    surfaceColor = blend(surfaceColor, getCloud(pos, atmosphereLight, 0));\n"
    "    surfaceColor = blend(surfaceColor, getCloud(pos, atmosphereLight, 1));\n"

    "    surfaceColor = vec4(surfaceColor.rgb * surfaceAA, surfaceAA);\n"

    "    gl_FragColor = blend(surfaceColor, atmosphereColor);\n"
    "}\n";

static const QString VERTEX_SHADER =
    "attribute vec3 vPosition;\n"
    "attribute vec2 vTexCoord;\n"
    "varying vec2 texCoord;\n"

    "void main()\n"
    "{\n"
    "    gl_Position = vec4(vPosition, 1.0);\n"
    "    texCoord = vTexCoord;\n"
    "}\n";

class PlanetRenderer: public QQuickFramebufferObject::Renderer
{
    struct Vertex
    {
        QVector3D position;
        QVector2D texCoord;
    };

public:
    PlanetRenderer(): QQuickFramebufferObject::Renderer(),
        vbo(QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)),
        surfaceUpdated(false),
        surfaceTexture(nullptr),
        cloud0Updated(false),
        cloud0Texture(nullptr),
        cloud1Updated(false),
        cloud1Texture(nullptr)
    {
        vbo.create();
        vbo.bind();
        vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo.allocate(4 * sizeof(Vertex));

        vbo.bind();
        Vertex *v = (Vertex*)vbo.map(QOpenGLBuffer::WriteOnly);
        if (!v)
            return;

        v[0] = {QVector3D(-1.0, -1.0, 0.0), QVector2D(0.0, 0.0)};
        v[1] = {QVector3D(-1.0,  1.0, 0.0), QVector2D(0.0, 1.0)};
        v[2] = {QVector3D( 1.0,  1.0, 0.0), QVector2D(1.0, 1.0)};
        v[3] = {QVector3D( 1.0, -1.0, 0.0), QVector2D(1.0, 0.0)};
        vbo.unmap();

        shader.addShaderFromSourceCode(QOpenGLShader::Vertex, VERTEX_SHADER);
        shader.addShaderFromSourceCode(QOpenGLShader::Fragment, FRAGMENT_SHADER);
        shader.link();
        shader.bind();

        vPositionIndex = shader.attributeLocation("vPosition");
        vTexCoordIndex = shader.attributeLocation("vTexCoord");

        shader.enableAttributeArray(vPositionIndex);
        shader.enableAttributeArray(vTexCoordIndex);
        shader.setAttributeBuffer(vPositionIndex, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
        shader.setAttributeBuffer(vTexCoordIndex, GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

        shader.setUniformValue("surface", 0);
        shader.setUniformValue("cloud0", 1);
        shader.setUniformValue("cloud1", 2);
        shader.setUniformValue("hasCloud0", false);
        shader.setUniformValue("hasCloud1", false);

        shader.release();
        vbo.release();
    }

protected:
    virtual QOpenGLFramebufferObject *createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObject *fbo = Renderer::createFramebufferObject(size);
        return fbo;
    }

    virtual void render()
    {
        QOpenGLFunctions *g = QOpenGLContext::currentContext()->functions();
        g->glClearColor(0, 0, 0, 0);
        g->glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        vbo.bind();

        if (surfaceTexture)
            surfaceTexture->bind(0);

        if (cloud0Texture)
            cloud0Texture->bind(1);

        if (cloud1Texture)
            cloud1Texture->bind(2);

        shader.enableAttributeArray(vPositionIndex);
        shader.enableAttributeArray(vTexCoordIndex);
        shader.setAttributeBuffer(vPositionIndex, GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));
        shader.setAttributeBuffer(vTexCoordIndex, GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

        g->glDrawArrays(GL_QUADS, 0, 4);

        if (cloud1Texture)
            cloud1Texture->release(2);

        if (cloud0Texture)
            cloud0Texture->release(1);

        if (surfaceTexture)
            surfaceTexture->release(0);

        g->glActiveTexture(GL_TEXTURE0);

        vbo.release();
        shader.release();
    }

    virtual void synchronize(QQuickFramebufferObject *item)
    {
        PlanetDrawer *drawer = static_cast<PlanetDrawer*>(item);

        shader.bind();

        if (surfaceUpdated)
        {
            if (surfaceTexture)
                delete surfaceTexture;
            surfaceTexture = nullptr;

            if (!surfaceImage.isNull())
            {
                surfaceTexture = new QOpenGLTexture(surfaceImage, QOpenGLTexture::DontGenerateMipMaps);
                surfaceImage = QImage();
                surfaceTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
                surfaceTexture->setMinificationFilter(QOpenGLTexture::Nearest);
            }
            surfaceUpdated = false;
        }

        if (cloud0Updated)
        {
            if (cloud0Texture)
                delete cloud0Texture;
            cloud0Texture = nullptr;

            if (!cloud0Image.isNull())
            {
                cloud0Texture = new QOpenGLTexture(cloud0Image, QOpenGLTexture::DontGenerateMipMaps);
                cloud0Image = QImage();
                cloud0Texture->setMagnificationFilter(QOpenGLTexture::Nearest);
                cloud0Texture->setMinificationFilter(QOpenGLTexture::Nearest);
                shader.setUniformValue("hasCloud0", true);
            }
            else
                shader.setUniformValue("hasCloud0", false);

            cloud0Updated = false;
        }

        if (cloud1Updated)
        {
            if (cloud1Texture)
                delete cloud1Texture;
            cloud1Texture = nullptr;

            if (!cloud1Image.isNull())
            {
                cloud1Texture = new QOpenGLTexture(cloud1Image, QOpenGLTexture::DontGenerateMipMaps);
                cloud1Image = QImage();
                cloud1Texture->setMagnificationFilter(QOpenGLTexture::Nearest);
                cloud1Texture->setMinificationFilter(QOpenGLTexture::Nearest);
                shader.setUniformValue("hasCloud1", true);
            }
            else
                shader.setUniformValue("hasCloud1", false);

            cloud1Updated = false;
        }

        shader.setUniformValue("pixelSize", (GLfloat)(1.0f / drawer->width()));
        shader.setUniformValue("phase", drawer->phase());
        shader.setUniformValue("cloud0Phase", drawer->cloud0Phase());
        shader.setUniformValue("cloud1Phase", drawer->cloud1Phase());
        shader.setUniformValue("atmosphere", drawer->atmosphere());
        shader.setUniformValue("angle", drawer->lightAngle());
        shader.release();

        update();
    }

private:
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram shader;
    QImage surfaceImage;
    bool surfaceUpdated;
    QOpenGLTexture *surfaceTexture;

    QImage cloud0Image, cloud1Image;
    bool cloud0Updated, cloud1Updated;
    QOpenGLTexture *cloud0Texture, *cloud1Texture;

    int vPositionIndex, vTexCoordIndex;

    friend class OpenSR::PlanetDrawer;

    // !!! Called from GUI thread
    void updateSurface(const QUrl& surface)
    {
        surfaceImage = QImage();
        // TODO: make async
        QIODevice *imgDev = static_cast<Engine*>(qApp)->resources()->getIODevice(surface);
        if (imgDev)
        {
            QImageReader reader(imgDev);
            surfaceImage = reader.read();
            delete imgDev;
        }
        surfaceUpdated = true;
    }

    // !!! Called from GUI thread
    void updateCloud0(const QUrl cloud0)
    {
        cloud0Image = QImage();
        // TODO: make async
        QIODevice *imgDev = static_cast<Engine*>(qApp)->resources()->getIODevice(cloud0);
        if (imgDev)
        {
            QImageReader reader(imgDev);
            cloud0Image = reader.read();
            delete imgDev;
        }
        cloud0Updated = true;
    }

    // !!! Called from GUI thread
    void updateCloud1(const QUrl cloud1)
    {
        cloud1Image = QImage();
        // TODO: make async
        QIODevice *imgDev = static_cast<Engine*>(qApp)->resources()->getIODevice(cloud1);
        if (imgDev)
        {
            QImageReader reader(imgDev);
            cloud1Image = reader.read();
            delete imgDev;
        }
        cloud1Updated = true;
    }
};
}

class PlanetDrawer::PlanetDrawerPrivate
{
public:
    PlanetDrawerPrivate()
    {
        phase = 0.0f;
        cloud0Phase = 0.0f; cloud1Phase = 0.0f;
        lightAngle = 0.0f;
        renderer = nullptr;
    }

    QUrl surface;
    float phase;
    float cloud0Phase, cloud1Phase;
    QColor atmosphere;
    QUrl cloud0, cloud1;
    float lightAngle;
    mutable PlanetRenderer *renderer;
};

PlanetDrawer::PlanetDrawer(QQuickItem* parent): QQuickFramebufferObject(parent),
    d_osr_ptr(new PlanetDrawerPrivate())
{
    qApp;
    Q_D(PlanetDrawer);
}

PlanetDrawer::~PlanetDrawer()
{
    Q_D(PlanetDrawer);
}

QQuickFramebufferObject::Renderer* PlanetDrawer::createRenderer() const
{
    Q_D(const PlanetDrawer);
    d->renderer = new PlanetRenderer();
    d->renderer->updateSurface(d->surface);
    d->renderer->updateCloud0(d->cloud0);
    d->renderer->updateCloud1(d->cloud1);
    return d->renderer;
}

void PlanetDrawer::setSurface(const QUrl& surface)
{
    Q_D(PlanetDrawer);
    if (d->surface != surface)
    {
        d->surface = surface;
        if (d->renderer)
            d->renderer->updateSurface(d->surface);
        update();
        emit(surfaceChanged());
    }
}

QUrl PlanetDrawer::surface() const
{
    Q_D(const PlanetDrawer);
    return d->surface;
}

void PlanetDrawer::setPhase(float phase)
{
    Q_D(PlanetDrawer);
    if (d->phase != phase)
    {
        d->phase = phase;
        update();
        emit(phaseChanged());
    }
}

float PlanetDrawer::phase() const
{
    Q_D(const PlanetDrawer);
    return d->phase;
}


QColor PlanetDrawer::atmosphere() const
{
    Q_D(const PlanetDrawer);
    return d->atmosphere;
}

void PlanetDrawer::setAtmosphere(const QColor& atmosphere)
{
    Q_D(PlanetDrawer);
    if (d->atmosphere != atmosphere)
    {
        d->atmosphere = atmosphere;
        update();
        emit(atmosphereChanged());
    }
}

QUrl PlanetDrawer::cloud0() const
{
    Q_D(const PlanetDrawer);
    return d->cloud0;
}

void PlanetDrawer::setCloud0(const QUrl& cloud0)
{
    Q_D(PlanetDrawer);

    if (d->cloud0 != cloud0)
    {
        d->cloud0 = cloud0;
        if (d->renderer)
            d->renderer->updateCloud0(d->cloud0);
        update();
        emit(cloud0Changed());
    }
}

QUrl PlanetDrawer::cloud1() const
{
    Q_D(const PlanetDrawer);
    return d->cloud1;
}

void PlanetDrawer::setCloud1(const QUrl& cloud1)
{
    Q_D(PlanetDrawer);

    if (d->cloud1 != cloud1)
    {
        d->cloud1 = cloud1;
        if (d->renderer)
            d->renderer->updateCloud1(d->cloud1);
        update();
        emit(cloud1Changed());
    }
}

float PlanetDrawer::cloud0Phase() const
{
    Q_D(const PlanetDrawer);
    return d->cloud0Phase;
}

void PlanetDrawer::setCloud0Phase(float phase)
{
    Q_D(PlanetDrawer);

    if (d->cloud0Phase != phase)
    {
        d->cloud0Phase = phase;
        update();
        emit(cloud0PhaseChanged());
    }
}

float PlanetDrawer::cloud1Phase() const
{
    Q_D(const PlanetDrawer);
    return d->cloud1Phase;
}

void PlanetDrawer::setCloud1Phase(float phase)
{
    Q_D(PlanetDrawer);

    if (d->cloud1Phase != phase)
    {
        d->cloud1Phase = phase;
        update();
        emit(cloud1PhaseChanged());
    }
}

float PlanetDrawer::lightAngle() const
{
    Q_D(const PlanetDrawer);
    return d->lightAngle;
}

void PlanetDrawer::setLightAngle(float angle)
{
    Q_D(PlanetDrawer);

    if (d->lightAngle != angle)
    {
        d->lightAngle = angle;
        update();
        emit(lightAngleChanged());
    }
}
}
