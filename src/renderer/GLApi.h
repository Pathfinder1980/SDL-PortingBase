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

    inline constexpr GLbitfield GL_COLOR_BUFFER_BIT = 0x00004000;
    inline constexpr GLbitfield GL_DEPTH_BUFFER_BIT = 0x00000100;
    inline constexpr GLenum     GL_VERSION          = 0x1F02;

    
    struct GLApi
    {
        using GetProcFn = void* (*)(const char*);
        
        bool Load(GetProcFn getProc, const char** missingFunction = nullptr);

        void (PB_GL_CALL* ClearColor)(GLfloat r, GLfloat g, GLfloat b, GLfloat a) = nullptr;
        void (PB_GL_CALL* Clear)(GLbitfield mask) = nullptr;
        void (PB_GL_CALL* Viewport)(GLint x, GLint y, GLsizei width, GLsizei height) = nullptr;
        GLubyte* (PB_GL_CALL* GetString)(GLenum name) = nullptr;
    };
}