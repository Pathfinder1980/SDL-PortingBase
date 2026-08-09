#include "Renderer.h"
#include <format>
#include <fstream>
#include <sstream>
#include <iterator>
#include <array>
#include <cstdint>
#include <numbers>
#include <cstdio>
#include <string_view>


#include "renderer/Math.h"


namespace porting_base
{
    namespace 
    {
        constexpr float kCubeVertices[] = {
            // +Z
            -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
            // -Z
             0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
            // +X
             0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            // -X
            -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            // +Y
            -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
            // -Y
            -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        };

        constexpr unsigned short kCubeIndices[] = {
            0,  1,  2,   0,  2,  3,
            4,  5,  6,   4,  6,  7,
            8,  9, 10,   8, 10, 11,
            12, 13, 14,  12, 14, 15,
            16, 17, 18,  16, 18, 19,
            20, 21, 22,  20, 22, 23,
        };

        constexpr float kFovYRadians = 60.0f * std::numbers::pi_v<float> / 180.0f;

        constexpr std::array<std::uint8_t, 8 * 8 * 4> MakeCheckerTexture()
        {
            std::array<std::uint8_t, 8 * 8 * 4> texels {};
            for (int y = 0; y < 8; ++y)
            {
                for (int x = 0; x < 8; ++x)
                {
                    const bool light = (x + y) % 2 == 0;
                    const std::uint8_t value = light ? 0xE0 : 0x40;
                    const int i = (y * 8 + x) * 4;
                    texels[i + 0] = value;
                    texels[i + 1] = value;
                    texels[i + 2] = value;
                    texels[i + 3] = 0xFF;
                }
            }
            return texels;
        }

        constexpr std::array<std::uint8_t, 8 * 8 * 4> kCheckTexture { MakeCheckerTexture() };

        void PB_GL_CALL DebugCallback([[maybe_unused]]GLenum source, [[maybe_unused]]GLenum type, [[maybe_unused]]GLuint id, GLenum severity, 
            [[maybe_unused]]GLsizei length, const GLchar* message, [[maybe_unused]]const void* userParam)
        {
            if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
            {
                return;
            }
            std::fprintf(stderr, "[GL] %s\n", message);

            if (severity == GL_DEBUG_SEVERITY_HIGH)
            {
                PB_DEBUG_BREAK();
            }
        }
    }


    Renderer::~Renderer()
    {
        if (m_Tex != 0)
        {
            m_GLApi.DeleteTextures(1, &m_Tex);
        }

        if (m_Ebo != 0)
        {
            m_GLApi.DeleteBuffers(1, &m_Ebo);
        }
        if (m_Vbo != 0)
        {
            m_GLApi.DeleteBuffers(1, &m_Vbo);
        }
        if (m_Vao != 0)
        {
            m_GLApi.DeleteVertexArrays(1, &m_Vao);
        }

        if (m_Program != 0)
        {
            m_GLApi.DeleteProgram(m_Program);
        }
    }

    std::unique_ptr<Renderer> Renderer::Create(GLApi::GetProcFn getProc, const std::filesystem::path& shaderPath, std::string& errorMessage)
    {
        std::unique_ptr<Renderer> renderer { new Renderer() };
        
        const char* missingFunction;
        if (!renderer->m_GLApi.Load(getProc, &missingFunction))
        {
            errorMessage = std::format("Failed to load GL function: {}", missingFunction);
            return nullptr;
        }
        
        const GLubyte* version = renderer->m_GLApi.GetString(GL_VERSION);
        renderer->m_VersionString = version ? reinterpret_cast<const char*>(version) : "unknown";

#ifndef NDEBUG
        GLint extensionCount = 0;
        renderer->m_GLApi.GetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
        bool hasKhrDebug = false;
        for (GLint i = 0; i < extensionCount; ++i)
        {
            const GLubyte* ext = renderer->m_GLApi.GetStringi(GL_EXTENSIONS, static_cast<GLuint>(i));
            if (ext != nullptr && std::string_view { reinterpret_cast<const char*>(ext) } == "GL_KHR_debug")
            {
                hasKhrDebug = true;
                break;
            }
        }
        if (hasKhrDebug && renderer->m_GLApi.DebugMessageCallback != nullptr)
        {
            renderer->m_GLApi.Enable(GL_DEBUG_OUTPUT);
            renderer->m_GLApi.Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            renderer->m_GLApi.DebugMessageCallback(&DebugCallback, nullptr);
        }
#endif

        
        renderer->m_GLApi.Enable(GL_DEPTH_TEST);

        if (!renderer->InitShaderProgram(shaderPath, errorMessage))
        {
            return nullptr;
        }
        if (!renderer->InitGeometry(errorMessage))
        {
            return nullptr;
        }
        if (!renderer->InitTexture(errorMessage))
        {
            return nullptr;
        }

        return renderer;
    }

    const std::string& Renderer::GetVersionString() const
    {
        return m_VersionString;
    }

    void Renderer::Clear(float r, float g, float b, float a)
    {
        m_GLApi.ClearColor(r, g, b, a);        
        m_GLApi.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Draw(int drawableWidth, int drawableHeight, float angleX, float angleY)
    {
        const int w = drawableWidth  > 0 ? drawableWidth  : 1;
        const int h = drawableHeight > 0 ? drawableHeight : 1;
        m_GLApi.Viewport(0, 0, w, h);

        const float aspect = static_cast<float>(w) / static_cast<float>(h);

        const Mat4 model      = Mat4::RotateY(angleY) * Mat4::RotateX(angleX);
        const Mat4 view       = Mat4::Translation(0.0f, 0.0f, -3.0f);
        const Mat4 projection = Mat4::Perspective(kFovYRadians, aspect, 0.1f, 100.0f);
        const Mat4 mvp        = projection * view * model;

        m_GLApi.UseProgram(m_Program);
        m_GLApi.UniformMatrix4fv(m_MvpLocation, 1, 0, mvp.m);
        m_GLApi.Uniform1i(m_TexLocation, 0);
        m_GLApi.ActiveTexture(GL_TEXTURE0);
        m_GLApi.BindTexture(GL_TEXTURE_2D, m_Tex);
        m_GLApi.BindVertexArray(m_Vao);
        m_GLApi.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(kCubeIndices)), GL_UNSIGNED_SHORT, nullptr);
    }    

    bool Renderer::InitShaderProgram(const std::filesystem::path& shaderPath, std::string& outError)
    {
        std::string vertexShaderLabel = "cube.vert";
        std::string vertexShaderSource = LoadShaderSourceFile(vertexShaderLabel, shaderPath, outError);
        if (vertexShaderSource.empty())
        {
            return false;
        }
        GLuint vertexShaderId = CompileStage(GL_VERTEX_SHADER, vertexShaderSource.c_str(), vertexShaderLabel, outError);
        if (vertexShaderId == 0)
        {
            return false;
        }

        std::string fragmentShaderLabel = "cube.frag";
        std::string fragmentShaderSource = LoadShaderSourceFile(fragmentShaderLabel, shaderPath, outError);
        if (fragmentShaderSource.empty())
        {
            return false;
        }
        GLuint fragmentId = CompileStage(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str(), fragmentShaderLabel, outError);
        if (fragmentId == 0)
        {
            m_GLApi.DeleteShader(vertexShaderId);
            return false;
        }

        GLuint program = m_GLApi.CreateProgram();
        m_GLApi.AttachShader(program, vertexShaderId);
        m_GLApi.AttachShader(program, fragmentId);
        m_GLApi.LinkProgram(program);

        GLint status;
        m_GLApi.GetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == 0)
        {
            char log[1024];
            GLsizei len = 0;
            m_GLApi.GetProgramInfoLog(program, sizeof(log), &len, log);
            outError = std::format("Failed to link shader program: {}", log);

            m_GLApi.DeleteShader(fragmentId);
            m_GLApi.DeleteShader(vertexShaderId);
            m_GLApi.DeleteProgram(program);            
            return false;
        }
        
        m_GLApi.DeleteShader(vertexShaderId);
        m_GLApi.DeleteShader(fragmentId);
        m_Program = program;

        m_MvpLocation =  m_GLApi.GetUniformLocation(program, "uMVP");
        if (m_MvpLocation == -1)
        {
            outError = std::format("Failed to get UniformLocation uMVP");
            return false;            
        }

        m_TexLocation = m_GLApi.GetUniformLocation(program, "uTexture");
        if (m_TexLocation == -1)
        {
            outError = std::format("Failed to get UniformLocation uTexture");
            return false;            
        }

        return true;
    }

    std::string Renderer::LoadShaderSourceFile(const std::string& fileName, const std::filesystem::path& shaderPath, std::string& outError)
    {
        std::filesystem::path fullPath = shaderPath / fileName;
        std::ifstream filePathStream(fullPath, std::ios::binary);
        if (!filePathStream)
        {
            outError = std::format("Failed to open shader file: {}", fullPath.string());
            return {};
        }

        std::ostringstream contents;
        contents << filePathStream.rdbuf();
        return contents.str();
    }

    GLuint Renderer::CompileStage(GLenum type, const char* source, const std::string& label, std::string& outError)
    {
        GLuint id = m_GLApi.CreateShader(type);
        m_GLApi.ShaderSource(id, 1, &source, nullptr);
        m_GLApi.CompileShader(id);

        GLint status;
        m_GLApi.GetShaderiv(id, GL_COMPILE_STATUS, &status);
        if (status == 0)
        {
            char log[1024]; 
            GLsizei len = 0; 
            m_GLApi.GetShaderInfoLog(id, sizeof(log), &len, log);
            outError = std::format("{} compile failed: {}", label, log);
            m_GLApi.DeleteShader(id);
            return 0;
        }

        return id;
    }

    bool Renderer::InitGeometry(std::string& outError)
    {
        m_GLApi.GenVertexArrays(1, &m_Vao);
        m_GLApi.BindVertexArray(m_Vao);

        m_GLApi.GenBuffers(1, &m_Vbo);
        m_GLApi.BindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        m_GLApi.BufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);

        m_GLApi.GenBuffers(1, &m_Ebo);
        m_GLApi.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
        m_GLApi.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kCubeIndices), kCubeIndices, GL_STATIC_DRAW);

        m_GLApi.EnableVertexAttribArray(0);
        m_GLApi.VertexAttribPointer(0, 3, GL_FLOAT, 0, 8 * sizeof(float), nullptr);

        m_GLApi.EnableVertexAttribArray(1);
        m_GLApi.VertexAttribPointer(1, 3, GL_FLOAT, 0, 8 * sizeof(float), reinterpret_cast<const void*>(3 * sizeof(float)));

        m_GLApi.EnableVertexAttribArray(2);
        m_GLApi.VertexAttribPointer(2, 2, GL_FLOAT, 0, 8 * sizeof(float), reinterpret_cast<const void*>(6 * sizeof(float)));

        const GLenum error = m_GLApi.GetError();
        if (error != 0)
        {
            outError = std::format("GL error during geometry init: 0x{:X}", error);
            return false;
        }

        return true;
    }

    bool Renderer::InitTexture(std::string &outError)
    {
        m_GLApi.GenTextures(1, &m_Tex);
        m_GLApi.BindTexture(GL_TEXTURE_2D, m_Tex);
        m_GLApi.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, kCheckTexture.data());

        m_GLApi.GenerateMipmap(GL_TEXTURE_2D);
        m_GLApi.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        m_GLApi.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        m_GLApi.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_GLApi.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        const GLenum error = m_GLApi.GetError();
        if (error != 0)
        {
            outError = std::format("GL error during texture init: 0x{:X}", error);
            return false;
        }
        return true;
    }
}
