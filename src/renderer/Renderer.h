#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include "renderer/GLApi.h"


namespace porting_base
{
    class Renderer
    {
    public:
        ~Renderer();
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        static std::unique_ptr<Renderer> Create(GLApi::GetProcFn getProc, const std::filesystem::path& shaderPath, std::string& errorMessage);

        const std::string& GetVersionString() const;
        void Clear(float r, float g, float b, float a);

        void Draw(int drawableWidth, int drawableHeight, float angleX, float angleY);

    private:
        Renderer() = default;

        bool InitShaderProgram(const std::filesystem::path& shaderPath, std::string& outError);
        std::string LoadShaderSourceFile(const std::string& fileName, const std::filesystem::path& shaderPath, std::string& outError);
        GLuint CompileStage(GLenum type, const char* source, const std::string& label, std::string& outError);
        bool InitGeometry(std::string& outError);
        bool InitTexture(std::string& outError);

        std::string m_VersionString;
        GLint m_MvpLocation { -1 };
        GLint m_TexLocation { -1 };
        GLuint m_Program { 0 };
        GLuint m_Vao { 0 };
        GLuint m_Vbo { 0 };
        GLuint m_Ebo { 0 };
        GLuint m_Tex { 0 };

        GLApi m_GLApi;
    };

}