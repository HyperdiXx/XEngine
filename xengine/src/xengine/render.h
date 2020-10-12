#pragma once

#ifndef XENGINE_RENDERING_H
#define XENGINE_RENDERING_H

namespace ftgl
{
    struct texture_atlas_t;
    struct texture_font_t;
}

struct RenderPassData
{

};

struct RenderPass
{
    Framebuffer *active_framebuffer;
    RenderPassData data;
};


struct GraphicsState
{
    const Matrix4x4 IDENTITY_MATRIX = createMat4x4();
    
    Vec3  default_text_color = createVec3(1.0f, 1.0f, 1.0f);
    Vec3  default_cube_color = createVec3(0.0f, 1.0f, 0.0f);
    Vec4  default_line_color = createVec4(1.0f, 0.0f, 0.0f, 1.0f);

    Vec4 target_clear_color = createVec4(0.0f, 0.0f, 0.0f, 1.0f);

    uint32 default_line_width = 5;
    real32 default_text_scale = 1.0f;

    bool32 enable_shadows = false;

    std::unordered_map<const char*, Shader> shaders;
    std::unordered_map<const char*, Texture2D> textures;
    std::map<GLchar, Character> characters_map;

    VertexArray quad_vao;
    //Quad rect_vao;
    SphereMesh sphere_vao;
    CubeMesh cube_vao;
    LineMesh line_vao;

    Skybox *skybox_obj = nullptr;

    GraphicsDevice *graphics_device = nullptr;

    RenderPass *active_render_pass = nullptr;
    Framebuffer *active_framebuffer = nullptr;
};

struct RenderClearTarget
{
    real32 x, y, z, a;

    RenderClearTarget() {};
    RenderClearTarget(real32 x_x, real32 y_y, real32 z_z, real32 a_a) : x(x_x), y(y_y), z(z_z), a(a_a) {};
};

struct RenderCommandMesh
{

};

struct RenderCommandLine
{

};

struct RenderCommandQuad
{

};

enum CommandType
{
    LINE_COMMAND,
    QUAD_COMMAND,
    MESH_COMMAND
};

struct RenderCommand
{
    CommandType type;

    union
    {
        RenderClearTarget clear_command;
        RenderCommandMesh mesh_command;
        RenderCommandQuad quad_command;
        RenderCommandLine line_command;
    };
};

class RenderCommandQueue
{
public:
    typedef void(*RenderCommand)(void*);
    RenderCommandQueue();
    ~RenderCommandQueue();

    void* submit(RenderCommand func_ptr, uint32 size);
    void  executeQueue();

private:
    uint8 *command_buffer_ptr_base;
    uint8 *command_buffer_ptr;
    uint32 command_count = 0;
};

class Render
{
public:

    enum class ApiType
    {
        OPENGL,
        VULKAN,
        DX11
    };

    global void init(ApiType type = ApiType::OPENGL);
    global bool32 initRenderGL();
    global bool32 initRenderDX11();

    global void initGui();

    global bool32 initCommonGpuResources();
    //global bool32 loadFont(const char *path);
    global bool32 loadShaders();
    global bool32 loadFreeTextures();

    global void clear(uint32 flags);
    global void clearColor(real32 x, real32 y, real32 z, real32 a);

    global void shutdown();

    global void setDevice(GraphicsDevice *device);
    global void setRenderPass(RenderPass *pass);
    global void setActiveFramebuffer(Framebuffer *fbo);

    global Framebuffer *getActiveFramebuffer();
    global GraphicsDevice* getDevice();
     
    global Shader *getShader(const char* name);
    global Texture2D *getTexture2DResource(const char* name);

    global Camera2DComponent& getCamera2D();
    global Camera3DComponent& getCamera3D();

    global bool32 createQuad();

    global bool32 createFullquad();
    global bool32 createSkybox(Skybox *sky);
    global bool32 createCubemap(std::vector<const char*> paths, Cubemap *cube);
    global bool32 createCubemap(Cubemap *cube);
    global bool32 createShadowMaps(ShadowMap *shadow);
    global bool32 createSphere(SphereMesh *sphre);
    global bool32 createCube(CubeMesh *cube);
    //bool32 createMesh(xe_assets::Mesh *meh, xe_graphics::Vertex *vertex_type, bool32 calculate_tspace);

    global bool32 createLineMesh(const Vec3 &start, const Vec3 &end, LineMesh *line_com);
    global bool32 createLineMesh(LineMesh *line_com);

    global bool32 createLinesBuffer();
    global bool32 createQuadBuffer();

    global bool32 createRenderPass(const RenderPassData &data, RenderPass *rp);

    global void drawFullquad();

    global void drawQuad(const Vec2 &pos, const Vec2 &size, const Color4RGBA &color);
    global void drawQuad(const Vec3 &pos, const Vec2 &size, const Color4RGBA &color);
   
    global void drawQuad(real32 x, real32 y, real32 w, real32 h, const Color4RGBA &color);
    global void drawQuad(real32 x, real32 t, real32 w, real32 h, Texture2D *texture);

    global void drawQuad(Shader *shd, Texture2D *texture, const Matrix4x4 &mod);

    //void drawModel(Model *mod, Shader *shd, const glm::mat4 &transform = glm::mat4(1.0f));
    //void drawModel(AnimModel *mod, Shader *shd, const glm::mat4 &transform = glm::mat4(1.0f));

    //void drawModel(Model *mod, const Matrix4x4 &transform = createMat4x4(1.0f));
    //void drawModel(AnimModel *mod, const Matrix4x4 &transform = createMat4x44(1.0f));

    //void drawMesh(Mesh *mshs, Shader *shd);

    global void drawSphere(Texture2D *texture_diff);
    global void drawSphere();

    global void drawCube(Texture2D *texture_diff);
    global void drawCube();

    global void drawLine(Entity *ent);

    global void drawLine(real32 x, real32 y, real32 x_end, real32 y_end);
    global void drawLine(real32 x, real32 y, real32 z, real32 x_end, real32 y_end, real32 z_end);

    global void drawLine(real32 x, real32 y, real32 z, real32 x_end, real32 y_end, real32 z_end, Color4RGBA color);
    global void drawLine(real32 x, real32 y, real32 z, real32 x_end, real32 y_end, real32 z_end, Color3RGB color);

    global void drawLine(real32 x, real32 y, real32 x_end, real32 y_end, Color4RGBA color);
    global void drawLine(real32 x, real32 y, real32 x_end, real32 y_end, Color3RGB color);

    global void drawLines();
    global void drawQuads();

    global void drawEnt(Entity *ent);
    global void drawEntStatic(Entity *ent);
    global void drawEntWithShader(Entity *ent, Shader *shd);
    global void drawEntPrimitive(Entity *ent);

    global void drawText(const std::string &text, Vec2& pos, Vec3& color);
    global void drawText(const std::string &text, Vec2& pos);
    global void drawText(const std::string &text, real32 x, real32 y);
    global void drawText(const std::string &text, real32 x, real32 y, real32 scale);
    global void drawText(const std::string &text, real32 x, real32 y, Vec3 &color, real32 scale);

    global void drawWaterPlane(Entity *ent);

    global void drawSkybox();
    global void drawShadowMaps();

    global void drawAABB(const AABB &bb, const Matrix4x4 &matrix);

    global void applyShadowMap(ShadowMap *shadow);

    global void applyTransform(TransformComponent *transform, Shader *shd);
    global void applyDirLight(Shader *shd, DirLightComponent *directional_light, TransformComponent *transform);
    global void applySpotLight(Shader *shd, SpotLightComponent *directional_light, TransformComponent *transform);
    global void applyPointLight(Shader *shd, PointLightComponent *directional_light, TransformComponent *transform);

    global void beginFrame(bool32 shouldClearScreen);

    global void setupRenderCommand(CommandType type);

    global void executeCommands();
    global void executeCommand(CommandType type);

    global void endFrame();

    global void addShader(const char *ac_name, Shader shr);
    global void addTexture(const char *ac_name, Texture2D tex);

    template<typename T>
    global void pushCommand(T&& func)
    {
        auto render_cmd = [](void* ptr) {
            auto func_ptr = (T*)ptr;
            (*func_ptr)();

            func_ptr->~T();
        };

        auto storageBuffer = GetRenderCommandQueue().submit(render_cmd, sizeof(func));
        new (storageBuffer) T(std::forward<T>(func));
    }

private:

    global RenderCommandQueue& GetRenderCommandQueue();
};
#endif // !XENGINE_RENDERING_H
