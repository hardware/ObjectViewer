#include "opengluniformbuffer.h"

#include <QAtomicInt>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

class OpenGLUniformBufferPrivate
{

public:
    OpenGLUniformBufferPrivate()
        : m_bufferId(0),
          m_ref(1),
          m_usagePattern(OpenGLUniformBuffer::DynamicDraw),
          m_funcs(nullptr)
    {}

    GLuint m_bufferId;
    QAtomicInt m_ref;
    OpenGLUniformBuffer::UsagePattern m_usagePattern;
    QOpenGLFunctions_4_3_Core* m_funcs;

};

OpenGLUniformBuffer::OpenGLUniformBuffer()
    : d_ptr(new OpenGLUniformBufferPrivate())
{}

OpenGLUniformBuffer::OpenGLUniformBuffer(const OpenGLUniformBuffer& other)
    : d_ptr(other.d_ptr)
{
    d_ptr->m_ref.ref();
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
    if( ! d_ptr->m_ref.deref() )
    {
        destroy();
        delete d_ptr;
        d_ptr = nullptr;
    }
}

OpenGLUniformBuffer& OpenGLUniformBuffer::operator=(const OpenGLUniformBuffer& other)
{
    if(d_ptr != other.d_ptr)
    {
        other.d_ptr->m_ref.ref();

        if( ! d_ptr->m_ref.deref() )
        {
            destroy();
            delete d_ptr;
            d_ptr = nullptr;
        }

        d_ptr = other.d_ptr;
    }

    return *this;
}

OpenGLUniformBuffer::UsagePattern OpenGLUniformBuffer::usagePattern() const
{
    Q_D(const OpenGLUniformBuffer);

    return d->m_usagePattern;
}

void OpenGLUniformBuffer::setUsagePattern(OpenGLUniformBuffer::UsagePattern value)
{
    Q_D(OpenGLUniformBuffer);

    d->m_usagePattern = value;
}

bool OpenGLUniformBuffer::create()
{
    Q_D(OpenGLUniformBuffer);

    QOpenGLContext* context = QOpenGLContext::currentContext();

    if(context)
    {
        d->m_funcs = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
        d->m_funcs->initializeOpenGLFunctions();

        d->m_funcs->glGenBuffers(1, &d->m_bufferId);

        if(d->m_bufferId) return true;
    }

    return false;
}

bool OpenGLUniformBuffer::isCreated() const
{
    Q_D(const OpenGLUniformBuffer);

    return ( d->m_bufferId != 0 );
}

bool OpenGLUniformBuffer::bind()
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::bind() : buffer not created");

    Q_D(const OpenGLUniformBuffer);

    if(d->m_bufferId)
    {
        d->m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, d->m_bufferId);
        return true;
    }

    return false;
}

bool OpenGLUniformBuffer::bind(int bindingPoint)
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::bind() : buffer not created");

    Q_D(const OpenGLUniformBuffer);

    if(d->m_bufferId)
    {
        d->m_funcs->glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, d->m_bufferId);
        return true;
    }

    return false;
}

void OpenGLUniformBuffer::destroy()
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::destroy() : buffer not created");

    Q_D(OpenGLUniformBuffer);

    if(d->m_bufferId)
    {
        d->m_funcs->glDeleteBuffers(1, &d->m_bufferId);
        d->m_bufferId = 0;
    }

    delete d->m_funcs;
    d->m_funcs = nullptr;
}

void OpenGLUniformBuffer::release()
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::release() : buffer not created");

    Q_D(const OpenGLUniformBuffer);

    if(d->m_bufferId)
        d->m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint OpenGLUniformBuffer::bufferId() const
{
    Q_D(const OpenGLUniformBuffer);

    return ( (d->m_bufferId) ? d->m_bufferId : 0 );
}

int OpenGLUniformBuffer::size() const
{
    Q_D(const OpenGLUniformBuffer);

    if ( ! d->m_bufferId )
        return -1;

    GLint value = -1;
    d->m_funcs->glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &value);

    return value;
}

bool OpenGLUniformBuffer::read(int offset, int size, void* data)
{
    Q_D(OpenGLUniformBuffer);

    while(glGetError() != GL_NO_ERROR);
    d->m_funcs->glGetBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    return glGetError() == GL_NO_ERROR;
}

void OpenGLUniformBuffer::write(int offset, int size, const void* data)
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::write() : buffer not created");

    Q_D(OpenGLUniformBuffer);

    if(d->m_bufferId)
        d->m_funcs->glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void OpenGLUniformBuffer::allocate(int size)
{
    allocate(size, nullptr);
}

void OpenGLUniformBuffer::allocate(int size, const void* data)
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::write() : buffer not created");

    Q_D(OpenGLUniformBuffer);

    if(d->m_bufferId)
        d->m_funcs->glBufferData(GL_UNIFORM_BUFFER, size, data, d->m_usagePattern);
}

void* OpenGLUniformBuffer::map(int offset, int size, int access)
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::map() : buffer not created");

    Q_D(OpenGLUniformBuffer);

    if( ! d->m_bufferId )
        return nullptr;

    return d->m_funcs->glMapBufferRange(GL_UNIFORM_BUFFER, offset, size, access);
}

bool OpenGLUniformBuffer::unmap()
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::unmap() : buffer not created");

    Q_D(OpenGLUniformBuffer);

    if( ! d->m_bufferId )
        return false;

    return d->m_funcs->glUnmapBuffer(GL_UNIFORM_BUFFER) == GL_TRUE;
}
