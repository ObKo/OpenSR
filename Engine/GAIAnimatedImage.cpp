/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/GAIAnimatedImage.h"

#include <OpenSR/GAITexture.h>
#include <QSGSimpleMaterialShader>
#include <QDir>
#include <QDebug>
#include <QBuffer>
#include <QSGSimpleRectNode>

namespace OpenSR
{
namespace
{
static const char gaiFragmentShader[] =
    "varying highp vec2 qt_TexCoord; \n"
    "uniform sampler2D qt_Texture; \n"
    "uniform lowp float qt_Opacity; \n"
    "void main() { \n"
    " vec4 color = texture2D(qt_Texture, qt_TexCoord);\n"
    " color.rgb *= color.a;\n"
    " color.bgr = color.rgb;\n"
    " gl_FragColor = color * qt_Opacity; \n"
    "}";

static const char gaiVertexShader[] =
    "uniform highp mat4 qt_Matrix; \n"
    "attribute highp vec4 qt_VertexPosition; \n"
    "attribute highp vec2 qt_VertexTexCoord; \n"
    "varying highp vec2 qt_TexCoord; \n"
    "void main() { \n"
    " qt_TexCoord = qt_VertexTexCoord; \n"
    " gl_Position = qt_Matrix * qt_VertexPosition; \n"
    "}";

struct GAIShaderState
{
    GAITexture *t;

    int compare(const GAIShaderState *other) const
    {

        if (t == other->t)
        {
            return 0;
        }
        else if (t < other->t)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
};

class GAIShader : public QSGSimpleMaterialShader<GAIShaderState>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(GAIShader, GAIShaderState);

public:
    const char *vertexShader() const
    {
        return gaiVertexShader;
    }

    const char *fragmentShader() const
    {
        return gaiFragmentShader;
    }

    QList<QByteArray> attributes() const
    {
        return QList<QByteArray>() << "qt_VertexPosition" << "qt_VertexTexCoord";
    }

    void resolveUniforms()
    {
        m_texture_id = program()->uniformLocation("qt_Texture");
    }

    void updateState(const GAIShaderState *state, const GAIShaderState *oldState)
    {
        program()->setUniformValue(m_texture_id, 0);
        if (state->t)
            state->t->bind();
    }

private:
    int m_texture_id;
};

class GAINode : public QSGGeometryNode
{
public:
    GAINode()
        : m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    {
        setGeometry(&m_geometry);

        QSGSimpleMaterial<GAIShaderState> *material = GAIShader::createMaterial();
        material->setFlag(QSGMaterial::Blending);
        setMaterial(material);
        setFlag(OwnsMaterial);
    }

    QSGGeometry m_geometry;
};

}

GAIAnimatedImage::GAIAnimatedImage(QQuickItem* parent): QQuickItem(parent),
    m_sourceChanged(false), m_loaded(false), m_currentFrame(0), m_currentFile(0),
    m_fileChanged(false), m_playing(true), m_speed(1.0f)
{
    setFlag(QQuickItem::ItemHasContents);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
}

void GAIAnimatedImage::setSources(const QList<QUrl>& url)
{
    m_sourceChanged = true;
    m_fileChanged = true;
    m_currentFile = 0;
    m_currentFrame = 0;

    m_gaiFrames.clear();
    m_gaiTimes.clear();
    m_gaiOffsets.clear();
    m_bgs.clear();
    m_headers.clear();
    m_timer.stop();
    m_loaded = false;

    m_sources = url;

    for (const QUrl& u : m_sources)
        loadGAI(u);

    if (!m_headers.isEmpty())
    {
        setImplicitWidth(m_headers[0].finishX - m_headers[0].startX);
        setImplicitHeight(m_headers[0].finishY - m_headers[0].startY);
    }
    emit(sourcesChanged());
    emit(currentFrameChanged());
    emit(framesCountChanged());
}

QList<QUrl> GAIAnimatedImage::sources() const
{
    return m_sources;
}

QSGNode* GAIAnimatedImage::updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* updatePaintNodeData)
{
    GAINode *n = dynamic_cast<GAINode*>(oldNode);

    if (!n)
        n = new GAINode();

    if (m_sourceChanged)
    {
        if (!m_loaded)
        {
            m_sourceChanged = false;
            return 0;
        }

        for (QSGTexture *t : m_textures)
        {
            delete t;
        }
        m_textures.clear();

        for (int i = 0; i < m_headers.size(); i++)
        {
            GAITexture *texture = new GAITexture(m_headers[i], m_bgs[i]);
            texture->setFiltering(QSGTexture::Linear);
            m_textures.push_back(texture);
        }
    }

    if (m_fileChanged)
        static_cast<QSGSimpleMaterial<GAIShaderState>*>(n->material())->state()->t = m_textures[m_currentFile];

    QSGGeometry::updateTexturedRectGeometry(n->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
    n->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    m_sourceChanged = false;
    m_fileChanged = false;

    return n;
}

void GAIAnimatedImage::loadGAI(const QUrl& source)
{

    if (!source.isLocalFile() && source.scheme() != "qrc")
    {
        qWarning() << "GAIAnimatedImage: Non-local files is unsupported.";
        return;
    }

    QString path;
    if (source.scheme() == "qrc")
        path = ":/" + source.path();
    else
        path = source.toLocalFile();


    QFile gaiDev(path);
    gaiDev.open(QIODevice::ReadOnly);
    if (!checkGAIHeader(&gaiDev))
    {
        gaiDev.close();
        return;
    }

    GAIHeader header = readGAIHeader(&gaiDev);

    QVector<int> times = loadGAITimes(&gaiDev, header);

    QFileInfo gaiFile(path);
    QString bgPath = gaiFile.dir().canonicalPath() + QDir::separator() + gaiFile.baseName() + ".gi";
    if (!QFileInfo(bgPath).exists())
        bgPath = gaiFile.dir().canonicalPath() + QDir::separator() + gaiFile.baseName() + ".GI";

    QImage background = QImage(bgPath);

    QVector<QByteArray> frames(header.frameCount);
    QVector<QPoint> offsets(header.frameCount);

    quint32 seekSizes[header.frameCount * 2];

    gaiDev.seek(sizeof(GAIHeader));
    gaiDev.read((char*)seekSizes, header.frameCount * 2 * sizeof(uint32_t));

    for (int i = 0; i < header.frameCount; i++)
    {
        quint32 giSeek = seekSizes[i * 2], giSize = seekSizes[i * 2 + 1];
        if (giSeek && giSize)
        {
            qint64 giOffset = giSeek;
            uint32_t signature;
            gaiDev.seek(giOffset);
            gaiDev.peek((char*)&signature, sizeof(uint32_t));

            if (signature == ZL01_SIGNATURE || signature == ZL02_SIGNATURE)
            {
                QByteArray zlibData = gaiDev.read(giSize);
                QByteArray data = unpackZL(zlibData);
                QBuffer bf(&data);
                bf.open(QIODevice::ReadOnly);
                loadGIFrame(&bf, i, header, frames, offsets);
            }
            else
            {
                loadGIFrame(&gaiDev, i, header, frames, offsets);
            }
        }
        else
        {
            frames[i] = QByteArray();
            offsets[i] = QPoint();
        }
    }

    m_headers.push_back(header);
    m_gaiTimes.push_back(times);
    m_bgs.push_back(background);
    m_gaiFrames.push_back(frames);
    m_gaiOffsets.push_back(offsets);

    m_timer.setSingleShot(true);
    m_timer.setInterval(m_gaiTimes[m_currentFile][m_currentFrame] / m_speed);
    m_timer.start();

    m_loaded = true;
}

void GAIAnimatedImage::loadGIFrame(QIODevice* dev, int i, const GAIHeader &header,
                                   QVector<QByteArray>& frames, QVector<QPoint>& offsets)
{
    GIFrameHeader image;

    if (!checkGIHeader(dev))
    {
        frames[i] = QByteArray();
        offsets[i] = QPoint();
        return;
    }

    qint64 offset = dev->pos();
    dev->read((char*)&image, sizeof(GIFrameHeader));

    if (image.type != 5)
    {
        frames[i] = QByteArray();
        offsets[i] = QPoint();
        return;
    }

    GILayerHeader layers[image.layerCount];

    for (int i = 0; i < image.layerCount; i++)
    {
        dev->read((char*)&layers[i], sizeof(GILayerHeader));

        layers[i].startX -= header.startX;
        layers[i].startY -= header.startY;
        layers[i].finishX -= header.startX;
        layers[i].finishY -= header.startY;
    }

    offsets[i] = QPoint(layers[0].startX, layers[0].startY);

    if (layers[0].size)
    {
        dev->seek(offset + layers[0].seek);
        frames[i] = dev->read(layers[0].size);
    }
    else
    {
        frames[i] = QByteArray();
    }
}

int GAIAnimatedImage::currentFrame() const
{
    //FIXME: Quite ugly
    int f = 0;
    for (int i = 0; i < m_currentFile; i++)
        f += m_headers[i].frameCount;

    return f + m_currentFrame;
}

int GAIAnimatedImage::framesCount() const
{
    //FIXME: Quite ugly
    int f = 0;
    for (int i = 0; i < m_headers.count(); i++)
        f += m_headers[i].frameCount;

    return f;

}

bool GAIAnimatedImage::paused() const
{
    return !m_playing;
}

bool GAIAnimatedImage::playing() const
{
    return m_playing;
}

void GAIAnimatedImage::setPlaying(bool playing)
{
    if (!m_playing && playing)
        m_timer.start();

    if (m_playing && !playing)
        m_timer.stop();

    m_playing = playing;
    emit(playingChanged());
    emit(pausedChanged());
}

void GAIAnimatedImage::setPaused(bool paused)
{
    if (m_playing && paused)
        m_timer.stop();

    if (!m_playing && !paused)
        m_timer.start();

    m_playing = !paused;
    emit(playingChanged());
    emit(pausedChanged());
}

void GAIAnimatedImage::setSpeed(float speed)
{
    m_speed = speed;
    emit(speedChanged());
}

float GAIAnimatedImage::speed() const
{
    return m_speed;
}

void GAIAnimatedImage::nextFrame()
{
    if (!m_loaded)
        return;

    if (m_currentFile >= m_headers.count())
        return;

    if ((m_currentFile >= m_textures.count()) || (!m_textures[m_currentFile]))
    {
        m_timer.setInterval(m_gaiTimes[m_currentFile][m_currentFrame] / m_speed);
        m_timer.start();
        return;
    }

    m_currentFrame = m_currentFrame + 1;
    if (m_currentFrame >= m_headers[m_currentFile].frameCount)
    {
        m_currentFrame = 0;
        m_currentFile = (m_currentFile + 1) % m_headers.count();
        setImplicitWidth(m_headers[m_currentFile].finishX - m_headers[m_currentFile].startX);
        setImplicitHeight(m_headers[m_currentFile].finishY - m_headers[m_currentFile].startY);
        m_fileChanged = true;
    }

    m_timer.setInterval(m_gaiTimes[m_currentFile][m_currentFrame] / m_speed);

    if (m_textures[m_currentFile])
        m_textures[m_currentFile]->drawNextFrame(m_gaiFrames[m_currentFile][m_currentFrame],
                m_gaiOffsets[m_currentFile][m_currentFrame]);

    m_timer.start();
    emit(currentFrameChanged());
}
}
