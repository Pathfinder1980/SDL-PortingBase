#pragma once

#include <memory>
#include <string>
#include "renderer/GLApi.h"


namespace porting_base
{
    class Renderer
    {
    public:
        static std::unique_ptr<Renderer> Create(GLApi::GetProcFn getProc, std::string& errorMessage);

        const std::string& GetVersionString() const;
        void Clear(float r, float g, float b, float a);


    private:
        Renderer() = default;

        std::string m_VersionString;

        GLApi m_GLApi;
    };

}