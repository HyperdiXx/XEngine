#pragma once

#include "../../cameras/camera.h"
#include "../../rendering/api/opengl/framebuffero.h"
#include "../../../xenpch.h"


namespace XEngine
{

    class SceneObjects
    {
    public:
        glm::vec3 lightPos;
        glm::vec3 lightColor;
        glm::vec3 lightDir;
        glm::vec3 fogColor;
        glm::mat4 projMatrix = glm::mat4(1.0f);
        Camera *cam;
        Rendering::FrameBuffer *sceneFBO;
        bool wireframe = false;
    public:
        static SceneObjects& getInstance()
        {
            static SceneObjects instance;
            return instance;
        }
    };

}