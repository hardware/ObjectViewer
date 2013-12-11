#ifndef OPENGLUNIFORMBUFFER_H
#define OPENGLUNIFORMBUFFER_H

#include <string>
#include <qopengl.h>

using namespace std;

class OpenGLUniformBufferPrivate;

class OpenGLUniformBuffer
{

public:

    OpenGLUniformBuffer();
    OpenGLUniformBuffer(GLuint programHandle, const string& blockName, GLsizei numMembers);
    OpenGLUniformBuffer(const OpenGLUniformBuffer& other);
    ~OpenGLUniformBuffer();

    OpenGLUniformBuffer& operator=(const OpenGLUniformBuffer& other);

    enum BufferLayout { Standard, Shared, Packed };

    enum UsagePattern
    {
        StreamDraw  = 0x88E0, // GL_STREAM_DRAW
        StreamRead  = 0x88E1, // GL_STREAM_READ
        StreamCopy  = 0x88E2, // GL_STREAM_COPY
        StaticDraw  = 0x88E4, // GL_STATIC_DRAW
        StaticRead  = 0x88E5, // GL_STATIC_READ
        StaticCopy  = 0x88E6, // GL_STATIC_COPY
        DynamicDraw = 0x88E8, // GL_DYNAMIC_DRAW
        DynamicRead = 0x88E9, // GL_DYNAMIC_READ
        DynamicCopy = 0x88EA  // GL_DYNAMIC_COPY
    };

    enum Access
    {
        Read             = 0x0001, // GL_MAP_READ_BIT
        Write            = 0x0002, // GL_MAP_WRITE_BIT
        InvalidateRange  = 0x0004, // GL_MAP_INVALIDATE_RANGE_BIT
        InvalidateBuffer = 0x0008, // GL_MAP_INVALIDATE_BUFFER_BIT
        FlushExplicit    = 0x0010, // GL_MAP_FLUSH_EXPLICIT_BIT
        Unsynchronized   = 0x0020  // GL_MAP_UNSYNCHRONIZED_BIT
    };

    OpenGLUniformBuffer::UsagePattern usagePattern() const;
    void setUsagePattern(OpenGLUniformBuffer::UsagePattern value);

    bool create();
    bool isCreated() const;
    bool bind();
    bool bind(int bindingPoint);
    void destroy();
    void release();

    bool read(int offset, int size, void* data);
    void write(int offset, int size, const void* data);
    void allocate(int size);
    void allocate(int size, const void* data);
    void* map(int offset, int size, int access);
    bool unmap();

    enum MembersDataType
    {
        Type         = 0x8A37, // GL_UNIFORM_TYPE
        Size         = 0x8A38, // GL_UNIFORM_SIZE
        NameLength   = 0x8A39, // GL_UNIFORM_NAME_LENGTH
        BlockIndex   = 0x8A3A, // GL_UNIFORM_BLOCK_INDEX
        Offset       = 0x8A3B, // GL_UNIFORM_OFFSET
        ArrayStride  = 0x8A3C, // GL_UNIFORM_ARRAY_STRIDE
        MatrixStride = 0x8A3D, // GL_UNIFORM_MATRIX_STRIDE
        IsRowMajor   = 0x8A3E  // GL_UNIFORM_IS_ROW_MAJOR
    };

    GLuint getBufferId() const;
    GLuint getIndex() const;
    GLint  getSize() const;

    void getBlockMembersData(const GLchar** uniformNames,
                             OpenGLUniformBuffer::MembersDataType type,
                             GLint* membersData);

    void assignBindingPoint(GLuint bindingPoint);

private:
    OpenGLUniformBufferPrivate* d_ptr;

    Q_DECLARE_PRIVATE( OpenGLUniformBuffer )

};

#endif // OPENGLUNIFORMBUFFER_H
