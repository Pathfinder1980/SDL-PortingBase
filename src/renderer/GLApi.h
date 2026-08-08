#pragma once

#include <cstdint>
#include <cstddef>

#if defined (_WIN32)
    #define PB_GL_CALL __stdcall
#else
    #define PB_GL_CALL
#endif


namespace porting_base
{
    using GLenum = std::uint32_t;
    using GLuint = std::uint32_t;
    using GLbitfield = std::uint32_t;
    using GLint = std::int32_t;
    using GLsizei = std::int32_t;
    using GLfloat = float;
    using GLubyte = std::uint8_t;
    using GLchar = char;
    using GLboolean = std::uint8_t;
    using GLsizeiptr = std::ptrdiff_t;
    using GLintptr = std::ptrdiff_t;

    inline constexpr GLenum GL_TRIANGLES       = 0x0004;
    inline constexpr GLenum GL_FLOAT           = 0x1406;
    inline constexpr GLenum GL_ARRAY_BUFFER    = 0x8892;
    inline constexpr GLenum GL_STATIC_DRAW     = 0x88E4;
    inline constexpr GLenum GL_VERTEX_SHADER   = 0x8B31;
    inline constexpr GLenum GL_FRAGMENT_SHADER = 0x8B30;
    inline constexpr GLenum GL_COMPILE_STATUS  = 0x8B81;
    inline constexpr GLenum GL_LINK_STATUS     = 0x8B82;

    inline constexpr GLbitfield GL_COLOR_BUFFER_BIT = 0x00004000;
    inline constexpr GLbitfield GL_DEPTH_BUFFER_BIT = 0x00000100;
    inline constexpr GLenum     GL_VERSION          = 0x1F02;

    inline constexpr GLenum GL_DEPTH_TEST           = 0x0B71;
    inline constexpr GLenum GL_UNSIGNED_SHORT       = 0x1403;
    inline constexpr GLenum GL_ELEMENT_ARRAY_BUFFER = 0x8893;

    
    struct GLApi
    {
        using GetProcFn = void* (*)(const char*);
        
        bool Load(GetProcFn getProc, const char** missingFunction = nullptr);

        void (PB_GL_CALL* ClearColor)(GLfloat r, GLfloat g, GLfloat b, GLfloat a) = nullptr;
        void (PB_GL_CALL* Clear)(GLbitfield mask) = nullptr;
        void (PB_GL_CALL* Viewport)(GLint x, GLint y, GLsizei width, GLsizei height) = nullptr;
        GLubyte* (PB_GL_CALL* GetString)(GLenum name) = nullptr;

        void (PB_GL_CALL* GenVertexArrays)(GLsizei n, GLuint* arrays) = nullptr;
        void (PB_GL_CALL* BindVertexArray)(GLuint array) = nullptr;
        void (PB_GL_CALL* GenBuffers)(GLsizei n, GLuint* buffers) = nullptr;
        void (PB_GL_CALL* BindBuffer)(GLenum target, GLuint buffer) = nullptr;
        void (PB_GL_CALL* BufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage) = nullptr;
        void (PB_GL_CALL* EnableVertexAttribArray)(GLuint index) = nullptr;
        void (PB_GL_CALL* VertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized,
                                                GLsizei stride, const void* pointer) = nullptr;
        void (PB_GL_CALL* DrawArrays)(GLenum mode, GLint first, GLsizei count) = nullptr;

        GLuint (PB_GL_CALL* CreateShader)(GLenum type) = nullptr;
        void (PB_GL_CALL* ShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string,
                                        const GLint* length) = nullptr;
        void (PB_GL_CALL* CompileShader)(GLuint shader) = nullptr;
        void (PB_GL_CALL* GetShaderiv)(GLuint shader, GLenum pname, GLint* params) = nullptr;
        void (PB_GL_CALL* GetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = nullptr;
        void (PB_GL_CALL* DeleteShader)(GLuint shader) = nullptr;

        GLuint (PB_GL_CALL* CreateProgram)() = nullptr;
        void (PB_GL_CALL* AttachShader)(GLuint program, GLuint shader) = nullptr;
        void (PB_GL_CALL* LinkProgram)(GLuint program) = nullptr;
        void (PB_GL_CALL* GetProgramiv)(GLuint program, GLenum pname, GLint* params) = nullptr;
        void (PB_GL_CALL* GetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = nullptr;
        void (PB_GL_CALL* UseProgram)(GLuint program) = nullptr;
        void (PB_GL_CALL* DeleteProgram)(GLuint program) = nullptr;

        void (PB_GL_CALL* DeleteVertexArrays)(GLsizei n, const GLuint* arrays) = nullptr;
        void (PB_GL_CALL* DeleteBuffers)(GLsizei n, const GLuint* buffers) = nullptr;

        void  (PB_GL_CALL* Enable)(GLenum cap) = nullptr;
        void  (PB_GL_CALL* DrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices) = nullptr;
        GLint (PB_GL_CALL* GetUniformLocation)(GLuint program, const GLchar* name) = nullptr;
        void  (PB_GL_CALL* UniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) = nullptr;

        GLenum (PB_GL_CALL* GetError)() = nullptr;
    };
}