#include "texture.h"

#include <QGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

Texture::Texture(TextureType type, const QImage& image)
    : m_type(type),
      m_image(QGLWidget::convertToGLFormat(image)),
      m_textureId(0),
      m_funcs(0)
{}

Texture::~Texture() {}

void Texture::load()
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->functions();
    m_funcs->initializeOpenGLFunctions();

    glGenTextures(1, &m_textureId);
    glBindTexture(m_type, m_textureId);
    glTexImage2D(m_type, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::destroy()
{
    if (m_textureId)
    {
        glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
    }
}

void Texture::bind(GLenum textureUnit)
{
    m_funcs->glActiveTexture(textureUnit);
    glBindTexture(m_type, m_textureId);
}
