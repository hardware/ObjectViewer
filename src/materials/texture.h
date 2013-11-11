#ifndef TEXTURE_H
#define TEXTURE_H

#include <qopengl.h>
#include <QImage>

class QOpenGLFunctions;

class Texture
{

public:
    enum TextureType
    {
        Texture1D      = GL_TEXTURE_1D,
        Texture2D      = GL_TEXTURE_2D,
        Texture3D      = GL_TEXTURE_3D,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP
    };

    Texture(const QImage& image, TextureType type = Texture2D);
    ~Texture();

    TextureType type() const { return m_type; }
    GLuint textureId() const { return m_textureId; }

    void load();
    void destroy();
    void bind(GLenum textureUnit);
    void release();

private:
    QImage      m_image;
    TextureType m_type;
    GLuint      m_textureId;

    QOpenGLFunctions* m_funcs;

};

#endif // TEXTURE_H
