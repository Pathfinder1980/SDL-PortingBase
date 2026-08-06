#pragma once

#include <memory>
#include <string>
#include "renderer/GLApi.h"


namespace porting_base
{
    class Renderer
    {
    public:
        ~Renderer();
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        static std::unique_ptr<Renderer> Create(GLApi::GetProcFn getProc, std::string &errorMessage);

        const std::string& GetVersionString() const;
        void Clear(float r, float g, float b, float a);

        void Draw();

    private:
        Renderer() = default;

        GLuint CompileStage(GLenum type, const char* source, std::string& outError);
        
        bool InitShaderProgram(std::string& outError);
        bool InitGeometry(std::string& outError);    

        std::string m_VersionString;
        GLuint m_Program { 0 };
        GLuint m_Vao { 0 };
        GLuint m_Vbo { 0 };

        GLApi m_GLApi;
    };

}