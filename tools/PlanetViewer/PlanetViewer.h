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
#include <GL/glut.h>
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
    bool colorCorrection() const;
    double solarAngle() const;
    QColor ambientColor() const;

    void setPlanetSpeed(double speed);
    void setCloudSpeed(double speed);
    void setPlanetSize(int size);
    void setCloudEnabled(bool enabled);
    void setColorCorrection(bool correction);
    void setSolarAngle(double angle);
    void setCloudTexture(const QPixmap& cloud);
    void setTexture(const QPixmap& cloud);
    void setAmbientColor(const QColor& color);

    static void generatePlanetImage(QImage &image, const QImage &texture, int size);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void updateSize();

    QPixmap m_texture;
    QPixmap m_cloudTexture;

    double m_phase;
    double m_cloudPhase;
    QTimer m_timer;

    float m_phaseSpeed;
    float m_cloudPhaseSpeed;
    bool m_cloudEnabled;
    bool m_colorCorrection;
    float m_solarAngle;

    int m_planetSize;
    int m_ambientColor;

    GLuint m_buffer;
    GLuint m_textureID;
    GLuint m_cloudTextureID;

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
