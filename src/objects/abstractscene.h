#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include <QObject>

class QOpenGLContext;

/**
 * @class AbstractScene
 * @brief Classe abtraite representant une scène
 *
 * Une scène permet le rendu et la gestion d'objets avec OpenGL
 */
class AbstractScene : public QObject
{
public:
    AbstractScene(QObject* parent = 0);

    /**
     * Chargement des shaders, des VBOs, IBOs, des textures...etc
     */
    virtual void initialize() = 0;

    /**
     * Permet de mettre à jour la scène
     */
    virtual void update(float t) = 0;

    /**
     * Permet de redessiner la scène
     */
    virtual void render(double currentTime) = 0;

    /**
     * Permet de redimensionner la scène
     */
    virtual void resize(int width, int height) = 0;

    /**
     * Méthodes d'accès au contexte OpenGL de la scène
     */
    inline void setContext(QOpenGLContext* context) { m_context = context; }
    inline QOpenGLContext* context() const { return m_context; }

protected:
    QOpenGLContext* m_context;

};

#endif // ABSTRACTSCENE_H
