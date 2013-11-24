#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <qopengl.h>
#include <QImage>

#include "ImageMagick/Magick++.h"

using namespace std;

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


    Texture(const string& fileName, TextureType type = Texture2D);
    Texture(const QImage& image, TextureType type = Texture2D);
    ~Texture();

    TextureType type() const { return m_type; }
    GLuint textureId() const { return m_textureId; }

    void init();
    bool load();
    void destroy();
    void bind(GLenum textureUnit);
    void release();

private:
    Magick::Image m_image;
    Magick::Blob  m_blob;

    string m_fileName;

    TextureType m_type;
    GLuint      m_textureId;

    QImage m_qimage;
    QOpenGLFunctions* m_funcs;

};

#endif // TEXTURE_H
