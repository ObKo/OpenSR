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

#ifndef OPENSR_GAIANIMATEDIMAGE_H
#define OPENSR_GAIANIMATEDIMAGE_H

#include <OpenSR/OpenSR.h>
#include <QQuickItem>
#include <QList>

namespace OpenSR
{
class GAIAnimatedImagePrivate;
class ENGINE_API GAIAnimatedImage: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<QUrl> sources READ sources WRITE setSources NOTIFY sourcesChanged)
    Q_PROPERTY(int framesCount READ framesCount NOTIFY framesCountChanged)
    Q_PROPERTY(int currentFrame READ currentFrame NOTIFY currentFrameChanged)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)

    OPENSR_DECLARE_PRIVATE(GAIAnimatedImage)

public:
    GAIAnimatedImage(QQuickItem * parent = 0);
    virtual ~GAIAnimatedImage();

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
    OPENSR_DECLARE_DPOINTER(GAIAnimatedImage);
    virtual QSGNode *updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData);

public Q_SLOTS:
    void nextFrame();
};
}

#endif // OPENSR_GAIANIMATEDIMAGE_H
