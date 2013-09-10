#ifndef WINDOW_H
#define WINDOW_H

#include <QWindow>
#include <QElapsedTimer>

class AbstractScene;
class QOpenGLContext;
class Scene;

/**
 * @class Window
 * @brief Classe representant la zone de rendu OpenGL
 *
 * Cette classe permet d'initialiser le contexte OpenGL et de
 * manipuler la scène de rendu.
 */
class Window : public QWindow
{
    Q_OBJECT

public:
    Window(QScreen *screen = 0);

    Scene* getScene();

protected:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    void initializeGL();

public slots:
    void checkAnimate(int state);
    void setCameraSpeed(double speed);
    void setCameraSensitivity(double sensitivity);

protected slots:
    void resizeGL();
    void paintGL();
    void updateScene();

signals:
    void updateFramerate();

private:
    QElapsedTimer    m_renderTimer, m_updateTimer;
    QOpenGLContext * m_context;
    AbstractScene  * m_scene;

    QPoint m_prevPos;
    QPoint m_pos;

    bool m_leftButtonPressed;

    double m_cameraSpeed;
    double m_cameraSensitivity;
};

#endif // WINDOW_H
