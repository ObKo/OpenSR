/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef PLANETVIEWER_H
#define PLANETVIEWER_H

#include <QPaintEvent>
#include <QImage>
#include <QTimer>
#include <QColor>

#ifdef Q_OS_WIN32
#include <GL/glew.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <QGLWidget>

class PlanetViewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit PlanetViewer(QWidget *parent = 0);

    double planetSpeed() const;
    double cloudSpeed() const;
    int planetSize() const;
    bool cloudEnabled() const;
    bool ringEnabled() const;
    bool colorCorrection() const;
    double solarAngle() const;
    QColor ambientColor() const;
    bool ringBackgroundEnabled() const;

    void setPlanetSpeed(double speed);
    void setCloudSpeed(double speed);
    void setPlanetSize(int size);
    void setCloudEnabled(bool enabled);
    void setRingEnabled(bool enabled);
    void setRingBackgroundEnabled(bool enabled);
    void setColorCorrection(bool correction);
    void setSolarAngle(double angle);
    void setCloudTexture(const QString& cloud);
    void setTexture(const QString& texture);
    void setRingTexture(const QString& ring);
    void setAmbientColor(const QColor& color);
    void setRingBackground(const QString& ring);
    void setRingOffset(float x, float y);
    void setRingBackgroundOffset(float x, float y);

    static void generatePlanetImage(QImage &image, const QImage &texture, int size);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void updateSize();

    QImage loadImage(const QString& file);

    QImage m_texture;
    QImage m_cloudTexture;
    QImage m_ringTexture;
    QImage m_ringBackgroundTexture;

    double m_phase;
    double m_cloudPhase;
    QTimer m_timer;

    float m_phaseSpeed;
    float m_cloudPhaseSpeed;
    bool m_cloudEnabled;
    bool m_colorCorrection;
    bool m_ringEnabled;
    bool m_ringBackgroundEnabled;
    float m_solarAngle;

    int m_planetSize;
    int m_ambientColor;

    float m_ringOffsetX, m_ringOffsetY;
    float m_ringBackgroundOffsetX, m_ringBackgroundOffsetY;

    GLuint m_buffer;
    GLuint m_ringBuffer;
    GLuint m_ringBackgroundBuffer;
    GLuint m_textureID;
    GLuint m_cloudTextureID;
    GLuint m_ringTextureID;
    GLuint m_ringBackgroundTextureID;

    GLuint m_shaderProgram;
    GLuint m_phaseArgument;
    GLuint m_cloudPhaseArgument;
    GLuint m_textureArgument;
    GLuint m_cloudArgument;
    GLuint m_cloudEnableArgument;
    GLuint m_colorCorrectionArgument;
    GLuint m_solarAngleArgument;
    GLuint m_ambientColorArgument;

private Q_SLOTS:
    void rotatePlanet();
};

#endif // PLANETVIEWER_H
