#include "renderer/GLApi.h"

namespace porting_base
{
    #define PB_GL_LOAD(name)                                                \
    do {                                                                    \
        name = reinterpret_cast<decltype(name)>(getProc("gl" #name));       \
        if (name == nullptr)                                                \
        {                                                                   \
            if (missingFunction != nullptr)                                 \
            {                                                               \
                *missingFunction = "gl" #name;                              \
            }                                                               \
            return false;                                                   \
        }                                                                   \
    } while (false)

    bool GLApi::Load(GetProcFn getProc, const char** missingFunction)
    {
        PB_GL_LOAD(ClearColor);
        PB_GL_LOAD(Clear);
        PB_GL_LOAD(Viewport);
        PB_GL_LOAD(GetString);

        PB_GL_LOAD(GenVertexArrays);
        PB_GL_LOAD(BindVertexArray);
        PB_GL_LOAD(GenBuffers); 
        PB_GL_LOAD(BindBuffer);
        PB_GL_LOAD(BufferData);
        PB_GL_LOAD(EnableVertexAttribArray);
        PB_GL_LOAD(VertexAttribPointer);
        PB_GL_LOAD(DrawArrays);
        PB_GL_LOAD(CreateShader);
        PB_GL_LOAD(ShaderSource);
            
        PB_GL_LOAD(CompileShader);
        PB_GL_LOAD(GetShaderiv);
        PB_GL_LOAD(GetShaderInfoLog);
        PB_GL_LOAD(DeleteShader);
        PB_GL_LOAD(CreateProgram);
        PB_GL_LOAD(AttachShader);
        PB_GL_LOAD(LinkProgram);
        PB_GL_LOAD(GetProgramiv);
        PB_GL_LOAD(GetProgramInfoLog);
        PB_GL_LOAD(UseProgram);
        PB_GL_LOAD(DeleteProgram);

        PB_GL_LOAD(DeleteVertexArrays);
        PB_GL_LOAD(DeleteBuffers);
        
        PB_GL_LOAD(Enable);
        PB_GL_LOAD(DrawElements);
        PB_GL_LOAD(GetUniformLocation);
        PB_GL_LOAD(UniformMatrix4fv);

        PB_GL_LOAD(GenTextures);
        PB_GL_LOAD(BindTexture);
        PB_GL_LOAD(TexImage2D);
        PB_GL_LOAD(TexParameteri);
        PB_GL_LOAD(GenerateMipmap);
        PB_GL_LOAD(ActiveTexture);
        PB_GL_LOAD(Uniform1i);
        PB_GL_LOAD(DeleteTextures);
        PB_GL_LOAD(PixelStorei);

        PB_GL_LOAD(GetError);

        return true;
    }

    #undef PB_GL_LOAD
}