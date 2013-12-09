#include "texture.h"

#include <QDebug>
#include <QGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Texture::Texture(const string& fileName, TextureType type)
    : m_qimage(),
      m_image(),
      m_fileName(fileName),
      m_type(type),
      m_textureId(0),
      m_funcs(nullptr)
{
    init();
    load();
}

Texture::Texture(const QImage& image, TextureType type)
    : m_qimage(QGLWidget::convertToGLFormat(image)),
      m_image(),
      m_fileName(""),
      m_type(type),
      m_textureId(0),
      m_funcs(nullptr)
{
    init();
    load();
}

Texture::~Texture()
{
    destroy();
}

void Texture::init()
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    m_funcs->initializeOpenGLFunctions();
}

bool Texture::load()
{
    m_funcs->glGenTextures(1, &m_textureId);
    m_funcs->glBindTexture(m_type, m_textureId);

    if( ! m_fileName.empty() )
    {
        try
        {
            m_image.read(m_fileName);
            m_image.magick("RGBA");
            m_image.write(&m_blob);
        }
        catch (Magick::Error& e)
        {
            qDebug() << "Error loading texture '" << m_fileName.c_str() << "': " << e.what();
            destroy();
            return false;
        }

        m_funcs->glTexImage2D(m_type,
            0,
            GL_RGBA,
            static_cast<GLsizei>(m_image.columns()),
            static_cast<GLsizei>(m_image.rows()),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            m_blob.data()
        );
    }
    else
    {
        m_funcs->glTexImage2D(m_type,
            0,
            GL_RGBA,
            m_qimage.width(),
            m_qimage.height(),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            m_qimage.bits()
        );
    }

    m_funcs->glTexParameterf(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_funcs->glTexParameterf(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::destroy()
{
    if(m_textureId)
    {
        m_funcs->glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }
}

void Texture::bind(GLenum textureUnit)
{
    m_funcs->glActiveTexture(textureUnit);
    m_funcs->glBindTexture(m_type, m_textureId);
}

void Texture::release()
{
    m_funcs->glBindTexture(m_type, 0);
}
