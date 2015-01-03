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

#ifndef OPENSR_GAIANIMATEDIMAGE_H
#define OPENSR_GAIANIMATEDIMAGE_H

#include <QQuickItem>
#include <OpenSR/libRangerQt.h>
#include <QTimer>
#include <QList>

namespace OpenSR
{
class GAITexture;
class GAIAnimatedImage: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<QUrl> sources READ sources WRITE setSources NOTIFY sourcesChanged)
    Q_PROPERTY(int framesCount READ framesCount NOTIFY framesCountChanged)
    Q_PROPERTY(int currentFrame READ currentFrame NOTIFY currentFrameChanged)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)
public:
    GAIAnimatedImage(QQuickItem * parent = 0);

    QList<QUrl> sources() const;
    int framesCount() const;
    int currentFrame() const;
    bool paused() const;
    bool playing() const;
    float speed() const;

    void setSources(const QList<QUrl>& urls);
    void setPaused(bool paused);
    void setPlaying(bool playing);
    void setSpeed(float speed);

Q_SIGNALS:
    void sourcesChanged();
    void framesCountChanged();
    void currentFrameChanged();
    void pausedChanged();
    void playingChanged();
    void speedChanged();

protected:
    virtual QSGNode *updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData);

private Q_SLOTS:
    void nextFrame();

private:
    void loadGAI(const QUrl& source);
    void loadGIFrame(QIODevice* dev, int i, const GAIHeader &header,
                     QVector<QByteArray>& frames, QVector<QPoint>& offsets);

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
}

#endif // OPENSR_GAIANIMATEDIMAGE_H
