#include "Renderer.h"
#include <format>


namespace porting_base
{
    std::unique_ptr<Renderer> Renderer::Create(GLApi::GetProcFn getProc, std::string &errorMessage)
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
    
}
