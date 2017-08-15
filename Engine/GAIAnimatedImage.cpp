/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 - 2017 Kosyak <ObKo@mail.ru>

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
#include <QTimer>
#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/libRangerQt.h>
#include <QImageReader>

namespace OpenSR
{
namespace
{
//TODO: Make loading async.
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

class GAIAnimatedImage::GAIAnimatedImagePrivate
{
    GAIAnimatedImage *q_ptr;
    Q_DECLARE_PUBLIC(GAIAnimatedImage)

    GAIAnimatedImagePrivate(GAIAnimatedImage *q);

    void loadGAI(const QUrl& source);
    void loadGIFrame(QIODevice* dev, int i, const GAIHeader &header, QVector<QByteArray>& frames, QVector<QPoint>& offsets);

    QList<QUrl> m_sources;
    bool m_sourceChanged;

    int m_currentFrame;

    bool m_loaded;

    QList<QImage> m_bgs;
    QList<GAIHeader> m_headers;

    QList<QVector<int> > m_gaiTimes;
    QList<QVector<QByteArray> > m_gaiFrames;
    QList<QVector<QPoint> > m_gaiOffsets;
    QTimer m_timer;

    QList<GAITexture*> m_textures;
    int m_currentFile;
    bool m_fileChanged;
    bool m_playing;
    float m_speed;
};

GAIAnimatedImage::GAIAnimatedImagePrivate::GAIAnimatedImagePrivate(GAIAnimatedImage *q):
    m_sourceChanged(false), m_loaded(false), m_currentFrame(0), m_currentFile(0),
    m_fileChanged(false), m_playing(true), m_speed(1.0f)
{
    q_ptr = q;
}

GAIAnimatedImage::GAIAnimatedImage(QQuickItem* parent): QQuickItem(parent),
    d_osr_ptr(new GAIAnimatedImagePrivate(this))
{
    Q_D(GAIAnimatedImage);
    setFlag(QQuickItem::ItemHasContents);
    connect(&d->m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
}

GAIAnimatedImage::~GAIAnimatedImage()
{
    Q_D(GAIAnimatedImage);
    for (QSGTexture *t : d->m_textures)
        delete t;
}

void GAIAnimatedImage::setSources(const QList<QUrl>& url)
{
    Q_D(GAIAnimatedImage);

    d->m_sourceChanged = true;
    d->m_fileChanged = true;
    d->m_currentFile = 0;
    d->m_currentFrame = 0;

    d->m_gaiFrames.clear();
    d->m_gaiTimes.clear();
    d->m_gaiOffsets.clear();
    d->m_bgs.clear();
    d->m_headers.clear();
    d->m_timer.stop();
    d->m_loaded = false;

    d->m_sources = url;

    for (const QUrl& u : d->m_sources)
        d->loadGAI(u);

    if (!d->m_headers.isEmpty())
    {
        setImplicitWidth(d->m_headers[0].finishX - d->m_headers[0].startX);
        setImplicitHeight(d->m_headers[0].finishY - d->m_headers[0].startY);
    }
    emit(sourcesChanged());
    emit(currentFrameChanged());
    emit(framesCountChanged());
}

QList<QUrl> GAIAnimatedImage::sources() const
{
    Q_D(const GAIAnimatedImage);
    return d->m_sources;
}

QSGNode* GAIAnimatedImage::updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* updatePaintNodeData)
{
    Q_D(GAIAnimatedImage);

    GAINode *n = dynamic_cast<GAINode*>(oldNode);

    if (!n)
        n = new GAINode();

    if (d->m_sourceChanged)
    {
        if (!d->m_loaded)
        {
            d->m_sourceChanged = false;
            return 0;
        }

        for (QSGTexture *t : d->m_textures)
            delete t;
        d->m_textures.clear();

        for (int i = 0; i < d->m_headers.size(); i++)
        {
            GAITexture *texture = new GAITexture(d->m_headers[i], d->m_bgs[i]);
            texture->setFiltering(QSGTexture::Linear);
            d->m_textures.push_back(texture);
        }
    }

    if (d->m_fileChanged)
        static_cast<QSGSimpleMaterial<GAIShaderState>*>(n->material())->state()->t = d->m_textures[d->m_currentFile];

    QSGGeometry::updateTexturedRectGeometry(n->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
    n->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    d->m_sourceChanged = false;
    d->m_fileChanged = false;

    return n;
}

void GAIAnimatedImage::GAIAnimatedImagePrivate::loadGAI(const QUrl& source)
{
    if (!source.isLocalFile() && source.scheme() != "qrc" && source.scheme() != "res")
    {
        qWarning() << "GAIAnimatedImage: Non-local files is unsupported.";
        return;
    }

    QString path;
    if (source.scheme() == "res")
        path = source.path();
    else if (source.scheme() == "qrc")
        path = ":/" + source.path();
    else
        path = source.toLocalFile();

    QIODevice *dev = 0;

    if (source.isLocalFile() || source.scheme() == "qrc")
    {
        dev = new QFile(path);
        dev->open(QIODevice::ReadOnly);
    }
    else
    {
        dev = ((Engine*)qApp)->resources()->getIODevice(path);
    }
    if (!dev)
        return;

    if (!checkGAIHeader(dev))
    {
        dev->close();
        return;
    }

    GAIHeader header = readGAIHeader(dev);

    QVector<int> times = loadGAITimes(dev, header);

    QFileInfo gaiFile(path);
    QString bgPath = gaiFile.dir().path() + '/' + gaiFile.baseName() + ".gi";

    QImage background;
    if (source.isLocalFile() || source.scheme() == "qrc")
        background = QImage(bgPath);
    else
    {
        QIODevice *bgDev = ((Engine*)qApp)->resources()->getIODevice(bgPath);
        if (bgDev)
        {
            background = QImageReader(bgDev).read();
            bgDev->close();
            delete bgDev;
        }
    }

    QVector<QByteArray> frames(header.frameCount);
    QVector<QPoint> offsets(header.frameCount);

    quint32 *seekSizes = new quint32[header.frameCount * 2];

    dev->seek(sizeof(GAIHeader));
    dev->read((char*)seekSizes, header.frameCount * 2 * sizeof(uint32_t));

    for (int i = 0; i < header.frameCount; i++)
    {
        quint32 giSeek = seekSizes[i * 2], giSize = seekSizes[i * 2 + 1];
        if (giSeek && giSize)
        {
            qint64 giOffset = giSeek;
            uint32_t signature;
            dev->seek(giOffset);
            dev->peek((char*)&signature, sizeof(uint32_t));

            if (signature == ZL01_SIGNATURE || signature == ZL02_SIGNATURE)
            {
                QByteArray zlibData = dev->read(giSize);
                QByteArray data = unpackZL(zlibData);
                QBuffer bf(&data);
                bf.open(QIODevice::ReadOnly);
                loadGIFrame(&bf, i, header, frames, offsets);
            }
            else
            {
                loadGIFrame(dev, i, header, frames, offsets);
            }
        }
        else
        {
            frames[i] = QByteArray();
            offsets[i] = QPoint();
        }
    }

    delete[] seekSizes;
    delete dev;

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

void GAIAnimatedImage::GAIAnimatedImagePrivate::loadGIFrame(QIODevice* dev, int i, const GAIHeader &header,
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

    GILayerHeader *layers = new GILayerHeader[image.layerCount];

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
    delete[] layers;
}

int GAIAnimatedImage::currentFrame() const
{
    Q_D(const GAIAnimatedImage);
    //FIXME: Quite ugly
    int f = 0;
    for (int i = 0; i < d->m_currentFile; i++)
        f += d->m_headers[i].frameCount;

    return f + d->m_currentFrame;
}

int GAIAnimatedImage::framesCount() const
{
    Q_D(const GAIAnimatedImage);
    //FIXME: Quite ugly
    int f = 0;
    for (int i = 0; i < d->m_headers.count(); i++)
        f += d->m_headers[i].frameCount;

    return f;

}

bool GAIAnimatedImage::paused() const
{
    Q_D(const GAIAnimatedImage);
    return !d->m_playing;
}

bool GAIAnimatedImage::playing() const
{
    Q_D(const GAIAnimatedImage);
    return d->m_playing;
}

void GAIAnimatedImage::setPlaying(bool playing)
{
    Q_D(GAIAnimatedImage);
    if (!d->m_playing && playing)
        d->m_timer.start();

    if (d->m_playing && !playing)
        d->m_timer.stop();

    d->m_playing = playing;
    emit(playingChanged());
    emit(pausedChanged());
}

void GAIAnimatedImage::setPaused(bool paused)
{
    Q_D(GAIAnimatedImage);
    if (d->m_playing && paused)
        d->m_timer.stop();

    if (!d->m_playing && !paused)
        d->m_timer.start();

    d->m_playing = !paused;
    emit(playingChanged());
    emit(pausedChanged());
}

void GAIAnimatedImage::setSpeed(float speed)
{
    Q_D(GAIAnimatedImage);
    d->m_speed = speed;
    emit(speedChanged());
}

float GAIAnimatedImage::speed() const
{
    Q_D(const GAIAnimatedImage);
    return d->m_speed;
}

void GAIAnimatedImage::nextFrame()
{
    Q_D(GAIAnimatedImage);

    if (!d->m_loaded)
        return;

    if (d->m_currentFile >= d->m_headers.count())
        return;

    if ((d->m_currentFile >= d->m_textures.count()) || (!d->m_textures[d->m_currentFile]))
    {
        d->m_timer.setInterval(d->m_gaiTimes[d->m_currentFile][d->m_currentFrame] / d->m_speed);
        d->m_timer.start();
        return;
    }

    d->m_currentFrame = d->m_currentFrame + 1;
    if (d->m_currentFrame >= d->m_headers[d->m_currentFile].frameCount)
    {
        d->m_currentFrame = 0;
        d->m_currentFile = (d->m_currentFile + 1) % d->m_headers.count();
        setImplicitWidth(d->m_headers[d->m_currentFile].finishX - d->m_headers[d->m_currentFile].startX);
        setImplicitHeight(d->m_headers[d->m_currentFile].finishY - d->m_headers[d->m_currentFile].startY);
        d->m_fileChanged = true;
    }

    d->m_timer.setInterval(d->m_gaiTimes[d->m_currentFile][d->m_currentFrame] / d->m_speed);

    if (d->m_textures[d->m_currentFile])
        d->m_textures[d->m_currentFile]->drawNextFrame(d->m_gaiFrames[d->m_currentFile][d->m_currentFrame],
                d->m_gaiOffsets[d->m_currentFile][d->m_currentFrame]);

    d->m_timer.start();
    emit(currentFrameChanged());
}
}
