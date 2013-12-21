#include "opengluniformbuffer.h"

#include <QDebug>

#include <QAtomicInt>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

class OpenGLUniformBufferPrivate
{

public:
    OpenGLUniformBufferPrivate(GLuint programHandle = 0, const string& blockName = NULL, GLsizei numMembers = 0)
        : m_bufferId(0),
          m_programHandle(programHandle),
          m_blockName(blockName),
          m_numMembers(numMembers),
          m_ref(1),
          m_usagePattern(OpenGLUniformBuffer::DynamicDraw),
          m_funcs(nullptr)
    {}

    GLuint  m_bufferId;
    GLuint  m_programHandle;
    GLuint  m_blockIndex;
    string  m_blockName;
    GLsizei m_numMembers;

    QAtomicInt m_ref;
    OpenGLUniformBuffer::UsagePattern m_usagePattern;
    QOpenGLFunctions_4_3_Core* m_funcs;

};

OpenGLUniformBuffer::OpenGLUniformBuffer()
    : d_ptr(new OpenGLUniformBufferPrivate())
{}

OpenGLUniformBuffer::OpenGLUniformBuffer(GLuint programHandle, const string& blockName, GLsizei numMembers)
    : d_ptr(new OpenGLUniformBufferPrivate(programHandle, blockName, numMembers))
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
        d->m_blockIndex = d->m_funcs->glGetUniformBlockIndex(d->m_programHandle, d->m_blockName.data());

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
}

void OpenGLUniformBuffer::release()
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::release() : buffer not created");

    Q_D(const OpenGLUniformBuffer);

    if(d->m_bufferId)
        d->m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

    return ( d->m_funcs->glUnmapBuffer(GL_UNIFORM_BUFFER) == GL_TRUE );
}

GLuint OpenGLUniformBuffer::getBufferId() const
{
    Q_D(const OpenGLUniformBuffer);

    return ( (d->m_bufferId) ? d->m_bufferId : 0 );
}

GLuint OpenGLUniformBuffer::getIndex() const
{
    Q_D(const OpenGLUniformBuffer);

    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::getIndex() : buffer not created");

    return d->m_blockIndex;
}

GLint OpenGLUniformBuffer::getSize() const
{
    Q_D(const OpenGLUniformBuffer);

    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::getSize() : buffer not created");

    GLint blockSize;
    d->m_funcs->glGetActiveUniformBlockiv(
        d->m_programHandle,
        d->m_blockIndex,
        GL_UNIFORM_BLOCK_DATA_SIZE,
        &blockSize
   );

    return blockSize;
}

void OpenGLUniformBuffer::getBlockMembersData(const GLchar** uniformNames,
                                              OpenGLUniformBuffer::MembersDataType type,
                                              GLint* membersData)
{
    Q_D(OpenGLUniformBuffer);

    GLuint* membersIndices = new GLuint[d->m_numMembers];

    d->m_funcs->glGetUniformIndices(d->m_programHandle, d->m_numMembers, uniformNames, membersIndices);
    d->m_funcs->glGetActiveUniformsiv(d->m_programHandle, d->m_numMembers, membersIndices, type, membersData);

    delete[] membersIndices;
}

void OpenGLUniformBuffer::assignBindingPoint(GLuint bindingPoint)
{
    if( ! isCreated() )
        qWarning("OpenGLUniformBuffer::assignBindingPoint() : buffer not created");

    Q_D(OpenGLUniformBuffer);

    d->m_funcs->glUniformBlockBinding(d->m_programHandle, d->m_blockIndex, bindingPoint);
}






























