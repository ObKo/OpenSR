#ifndef PLANETVIEWER_H
#define PLANETVIEWER_H

#include <QGLWidget>
#include <QPaintEvent>
#include <QImage>
#include <QTimer>

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

    void setPlanetSpeed(double speed);
    void setCloudSpeed(double speed);
    void setPlanetSize(int size);
    void setCloudEnabled(bool enabled);
    void setColorCorrection(bool correction);
    void setSolarAngle(double angle);
    void setCloudTexture(const QPixmap& cloud);
    void setTexture(const QPixmap& cloud);

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

private Q_SLOTS:
    void rotatePlanet();
};

#endif // PLANETVIEWER_H
