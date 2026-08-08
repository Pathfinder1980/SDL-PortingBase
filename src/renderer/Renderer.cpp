#include "Renderer.h"
#include <format>
#include <fstream>
#include <sstream>
#include <iterator>


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
    }


    Renderer::~Renderer()
    {
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
        
        renderer->m_GLApi.Enable(GL_DEPTH_TEST);

        if (!renderer->InitShaderProgram(shaderPath, errorMessage))
        {
            return nullptr;
        }
        if (!renderer->InitGeometry(errorMessage))
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

    void Renderer::Draw()
    {
        m_GLApi.UseProgram(m_Program);
        m_GLApi.BindVertexArray(m_Vao);
        m_GLApi.DrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(kCubeIndices)), GL_UNSIGNED_SHORT, nullptr);
    }

    

    bool Renderer::InitShaderProgram(const std::filesystem::path& shaderPath, std::string& outError)
    {
        std::string vertexShaderLabel = "triangle.vert";
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

        std::string fragmentShaderLabel = "triangle.frag";
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

        const GLenum error = m_GLApi.GetError();
        if (error != 0)
        {
            outError = std::format("GL error during geometry init: 0x{:X}", error);
            return false;
        }

        return true;
    }
}
