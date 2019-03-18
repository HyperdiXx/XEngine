#pragma once



#include "../graphicsapi/openglwnd.h"
#include "../core/rendering/pipeline/shader.h"
#include "../core/rendering/texture.h"
#include "../core/geometry/model.h"
#include "../core/imguibase.h"
#include "../core/cameras/camera.h"
#include "../xenpch.h"

namespace XEngine
{
    struct Loop
    {
        Shader basicShader;
        Shader lightShader;
        Shader cubeMap;
        Shader floorShader;
        real32 deltaTime = 0.0f;
        real32 lastFrame = 0.0f;
    };

    struct Letter
    {
        GLuint id;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint     Advance;
    };


   

    void OpenGLRunEngineWin32();
    void OpenGLUpdateLoopWin32();
    void InitStats();
    void RenderQ(Shader *shader);
}
