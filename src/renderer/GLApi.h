#pragma once

#include <cstdint>
#include <cstddef>

#if defined (_WIN32)
    #define PB_GL_CALL __stdcall
#else
    #define PB_GL_CALL
#endif

#if defined(_MSC_VER)
    #define PB_DEBUG_BREAK() __debugbreak()
#elif defined(__clang__)
    #define PB_DEBUG_BREAK() __builtin_debugtrap()
#else
    #include <csignal>
    #define PB_DEBUG_BREAK() raise(SIGTRAP)
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

    inline constexpr GLenum GL_TEXTURE_2D               = 0x0DE1;
    inline constexpr GLenum GL_TEXTURE0                 = 0x84C0;
    inline constexpr GLenum GL_RGBA                     = 0x1908;
    inline constexpr GLint  GL_RGBA8                    = 0x8058;
    inline constexpr GLenum GL_UNSIGNED_BYTE            = 0x1401;
    inline constexpr GLenum GL_TEXTURE_MAG_FILTER       = 0x2800;
    inline constexpr GLenum GL_TEXTURE_MIN_FILTER       = 0x2801;
    inline constexpr GLint  GL_NEAREST                  = 0x2600;
    inline constexpr GLint  GL_NEAREST_MIPMAP_LINEAR    = 0x2702;
    inline constexpr GLenum GL_TEXTURE_WRAP_S           = 0x2802;
    inline constexpr GLenum GL_TEXTURE_WRAP_T           = 0x2803;
    inline constexpr GLint  GL_REPEAT                   = 0x2901;
    inline constexpr GLenum GL_UNPACK_ALIGNMENT         = 0x0CF5;

    inline constexpr GLenum GL_DEBUG_OUTPUT                 = 0x92E0;
    inline constexpr GLenum GL_DEBUG_OUTPUT_SYNCHRONOUS     = 0x8242;
    inline constexpr GLenum GL_DEBUG_SEVERITY_NOTIFICATION  = 0x826B;
    inline constexpr GLenum GL_DEBUG_SEVERITY_HIGH          = 0x9146;

    inline constexpr GLenum GL_NUM_EXTENSIONS               = 0x821D;
    inline constexpr GLenum GL_EXTENSIONS                   = 0x1F03;

    
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

        void (PB_GL_CALL* GenTextures)(GLsizei n, GLuint* textures) = nullptr;
        void (PB_GL_CALL* BindTexture)(GLenum target, GLuint texture) = nullptr;
        void (PB_GL_CALL* TexImage2D)(GLenum target, GLint level, GLint internalFormat,
                                    GLsizei width, GLsizei height, GLint border,
                                    GLenum format, GLenum type, const void* pixels) = nullptr;
        void (PB_GL_CALL* TexParameteri)(GLenum target, GLenum pname, GLint param) = nullptr;
        void (PB_GL_CALL* GenerateMipmap)(GLenum target) = nullptr;
        void (PB_GL_CALL* ActiveTexture)(GLenum texture) = nullptr;
        void (PB_GL_CALL* Uniform1i)(GLint location, GLint v0) = nullptr;
        void (PB_GL_CALL* DeleteTextures)(GLsizei n, const GLuint* textures) = nullptr;
        void (PB_GL_CALL* PixelStorei)(GLenum pname, GLint param) = nullptr;

        GLenum (PB_GL_CALL* GetError)() = nullptr;

        using GLDebugProc = void (PB_GL_CALL*)(GLenum source, GLenum type, GLuint id, 
            GLenum severity, GLsizei length, 
            const GLchar* message, const void* userParam);
        void (PB_GL_CALL* DebugMessageCallback)(GLDebugProc callback, const void* userParam) = nullptr;
        void (PB_GL_CALL* GetIntegerv)(GLenum pname, GLint* data) = nullptr;
        const GLubyte* (PB_GL_CALL* GetStringi)(GLenum name, GLuint index) = nullptr;
    };
}