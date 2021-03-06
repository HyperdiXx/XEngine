
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include FT_PARAMETER_TAGS_H
//#include FT_MODULE_H

global GraphicsState graphics_state = {};
global RenderCommandQueue render_queue = {};
global MeshFactory mesh_factory = {};
global GPUResourceManager gpu_data = {};
 
global GraphicsDevice *g_device = 0;
global GraphicsContext *g_context = 0;

GraphicsDevice* Render::getDevice()
{
    return graphics_state.graphics_device;
}

GraphicsContext* Render::getContext()
{
    return graphics_state.graphics_context;
}

void Render::setDevice(GraphicsDevice *device)
{
    graphics_state.graphics_device = device;
    g_device = device;
}

void Render::setContext(GraphicsContext *context)
{
    graphics_state.graphics_context = context;
    g_context = context;
}

void Render::setRenderPass(RenderPass *pass)
{
    graphics_state.active_render_pass = pass;
}

void Render::setActiveFramebuffer(Framebuffer *fbo)
{
    graphics_state.active_framebuffer = fbo;
}

Framebuffer *Render::getActiveFramebuffer()
{
    return graphics_state.active_framebuffer;
}

Shader *Render::getShader(const char *name)
{
    return &graphics_state.gpu_resources.shaders[name];
}

Material *Render::getMaterial(const char* name)
{
    return &graphics_state.materials[name];
}

Texture2D *Render::getTexture2DResource(const char *name)
{
    return &graphics_state.gpu_resources.textures[name];
}

internal Camera2D& getCamera2D()
{
    global Camera2D camera_2d = {};

    if (!camera_2d.is_inited)
    {
        Viewport vp = Render::getContext()->getViewport();
        camera_2d.width = 1280;
        camera_2d.height = 720;
        camera_2d.updateCamera();
    }

    return camera_2d;
}

internal Camera3D& getCamera3D()
{
    global Camera3D camera_3d = {};

    if (!camera_3d.is_inited)
    {
        Viewport vp = Render::getContext()->getViewport();
        //camera_3d.aspect_ratio = vp.width / vp.height;
        camera_3d.aspect_ratio = real32(1280.0f / 720.0f);
    }

    return camera_3d;
}

internal
void fillVersions()
{
    graphics_state.gpu_version.vendor   = (char*)glGetString(GL_VENDOR);
    graphics_state.gpu_version.renderer = (char*)glGetString(GL_RENDERER);
    graphics_state.gpu_version.version  = (char*)glGetString(GL_VERSION);
}

internal
void initGPUResourceHandler()
{
    gpu_data.vb_handler.reserve(64);
    gpu_data.ib_handler.reserve(64);
    gpu_data.va_handler.reserve(64);
    gpu_data.tex2d_handler.reserve(64);
}

internal 
void addVBHandler(const VertexBuffer vb)
{
    gpu_data.vb_handler.push_back(vb);
}

internal
void addIBHandler(const IndexBuffer ib)
{
    gpu_data.ib_handler.push_back(ib);
}

internal
void addVAHandler(const VertexArray va)
{
    gpu_data.va_handler.push_back(va);
}

internal
Vec3 convertToVec3(ColRGB color)
{
    return createVec3(color.x, color.y, color.z);
}

internal
Vec4 convertToVec4(ColRGBA color)
{
    return createVec4(color.x, color.y, color.z, color.a);
}

internal
Vec4 convertToVec4(ColRGB color)
{
    return createVec4(color.x, color.y, color.z, 1.0f);
}

void MeshFactory::addCubePrimitive()
{
    global real32 cube_primitive_vertices[] =
    {
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
    };

    uint32 count = ArrayCount(cube_primitive_vertices);
    Array<real32> cube_data = createArray(cube_primitive_vertices, count);

    primitive_vertices[1] = cube_data;
}

Array<real32> *MeshFactory::createArrayOfVertices(GeometryType type)
{
    if (!is_init)
    {        
        addCubePrimitive();

        is_init = true;
    }

    Array<real32> *data = nullptr;

    data = &primitive_vertices[type];

    return data;
}

internal 
void setupCameras()
{
    Camera2D &camera2D = getCamera2D();
    Camera3D &camera3D = getCamera3D();

    
        
}

internal 
void setupMaterials()
{
    Shader *model3d = Render::getShader("base3d");
    Shader *def = Render::getShader("deffered");
    Shader *pp = Render::getShader("post_proc");
    Shader *light_pass = Render::getShader("lightpass");

    Texture2D *wood_diffuse = Render::getTexture2DResource("wood");
    Texture2D *water = Render::getTexture2DResource("water");

    Material deffered_shading(def);
    Material baseWater(model3d);
    Material baseCube(model3d);
    Material lightPass(light_pass);

    int sampler0 = 0;
    int sampler1 = 1;
    int sampler2 = 2;
   
    lightPass.set("albedoSpec", ShaderUniformType::Sampler2D, &sampler0);
    lightPass.set("normal_texture", ShaderUniformType::Sampler2D, &sampler1);
    lightPass.set("depth_texture", ShaderUniformType::Sampler2D, &sampler2);

    baseWater.set("tex_diff1", ShaderUniformType::Sampler2D, &sampler0);
    baseWater.addMaterialFlag(MaterialFlag::Depth);
    baseWater.addTexture2D(wood_diffuse);
    
    baseCube.set("tex_diff1", ShaderUniformType::Sampler2D, &sampler0);
    baseCube.addMaterialFlag(MaterialFlag::Depth);
    baseCube.addTexture2D(water);
       
    deffered_shading.set("tex_diff", ShaderUniformType::Sampler2D, &sampler0);
    deffered_shading.set("tex_norm", ShaderUniformType::Sampler2D, &sampler1);
    deffered_shading.set("tex_spec", ShaderUniformType::Sampler2D, &sampler2);
    deffered_shading.addMaterialFlag(MaterialFlag::Depth);
    deffered_shading.addTexture2D(wood_diffuse);

    Shader *rend2d = Render::getShader("render2d");
    Material r2d(rend2d);

    Material postProc(pp);

    postProc.set("tex_diff", ShaderUniformType::Sampler2D, &sampler0);

    Render::addMaterial("lightP", lightPass);
    Render::addMaterial("base", baseWater);
    Render::addMaterial("baseCube", baseCube);
    Render::addMaterial("render2d", r2d);
    Render::addMaterial("deffered", deffered_shading);
    Render::addMaterial("post_proc", postProc);
}

internal 
void setupBuffers()
{
    Render::createQuadBuffer();
    Render::createLinesBuffer();
}

internal 
void setupRenderPasses()
{
    RenderPassData default_pass = {};

    default_pass.name = "default";

}

internal 
void setupDefferedRender()
{
    Viewport &vp = g_context->getViewport();
    
    FramebufferObject &fb_obj = graphics_state.deffered_buffer;
    Framebuffer &fb = fb_obj.fb;

    FramebufferSpecs specs = {};

    specs.width = vp.width;
    specs.height = vp.height;

    TextureSampler samplerColor1 = {};

    samplerColor1.pxl_sampling_min = TEXTURE_SAMPLING::NEAREST;
    samplerColor1.pxl_sampling_mag = TEXTURE_SAMPLING::NEAREST;

    samplerColor1.wrapping_s = TEXTURE_WRAPPING::TEXTURE_ADDRESS_REPEAT;
    samplerColor1.wrapping_t = TEXTURE_WRAPPING::TEXTURE_ADDRESS_REPEAT;

    TextureDesc color1(vp.width, vp.height, TEXTURE_DIMENSION::TEXTURE_2D, TEXTURE_TYPE::COLOR, PIXEL_FORMAT::RGBA16F, PIXEL_INTERNAL_FORMAT::IFRGBA, PIXEL_TYPE::PTFLOAT, 0, samplerColor1);
    TextureDesc color2(vp.width, vp.height, TEXTURE_DIMENSION::TEXTURE_2D, TEXTURE_TYPE::COLOR, PIXEL_FORMAT::RGBA16F, PIXEL_INTERNAL_FORMAT::IFRGBA, PIXEL_TYPE::PTFLOAT, 0, samplerColor1);
    TextureDesc color3(vp.width, vp.height, TEXTURE_DIMENSION::TEXTURE_2D, TEXTURE_TYPE::COLOR, PIXEL_FORMAT::RGBA16F, PIXEL_INTERNAL_FORMAT::IFRGBA, PIXEL_TYPE::PTFLOAT, 0, samplerColor1);
    TextureDesc color4(vp.width, vp.height, TEXTURE_DIMENSION::TEXTURE_2D, TEXTURE_TYPE::COLOR, PIXEL_FORMAT::RGBA16F, PIXEL_INTERNAL_FORMAT::IFRGBA, PIXEL_TYPE::PTFLOAT, 0, samplerColor1);
    
    //depthStencil = Texture2D::create(specs.width, specs.height, TEXTURE_TYPE::DEPTH_STENCIL, PIXEL_FORMAT::DepthStencil32, PIXEL_INTERNAL_FORMAT::IFDEPTHSTENCIL, PIXEL_TYPE::PTFLOAT, 0);
    TextureDesc depth(vp.width, vp.height, TEXTURE_DIMENSION::TEXTURE_2D, TEXTURE_TYPE::COLOR, PIXEL_FORMAT::Depth32, PIXEL_INTERNAL_FORMAT::IFDEPTH, PIXEL_TYPE::PTFLOAT, 0, samplerColor1);
    
    fb_obj.desc.attachments[RENDER_TARGET_TYPE::RTColor0] = color1;
    fb_obj.desc.attachments[RENDER_TARGET_TYPE::RTColor1] = color2;
    fb_obj.desc.attachments[RENDER_TARGET_TYPE::RTColor2] = color3;
    fb_obj.desc.attachments[RENDER_TARGET_TYPE::RTColor3] = color4;

    fb_obj.desc.ds = depth;

    bool32 isSetuped = g_device->createFramebuffer(fb_obj.desc, specs, &fb);

    RenderPass deffered = {};

    if (Render::createRenderPass("deffered", true, true, &fb, &deffered))
    {
        Render::addRenderPass("deffered", deffered);
    }    
}

void Render::init(API_TYPE type)
{
    GraphicsDevice *device = getDevice();
    GraphicsContext *context = getContext();

    switch (type)
    {
    case API_TYPE::OPENGL:
    {
        LoadAllOpenGLProcedures();
        
        device = alloc_mem GraphicsDeviceGL();
        device->setAPI();
        
        context = alloc_mem GraphicsContextGL();

        setDevice(device);
        setContext(context);

    } break;
    default:
        break;
    }

    viewport(1280.0f, 720.0f);

    fillVersions();
    
    loadShaders();
    loadFreeTextures();
    
    setupMaterials();
    setupCameras();
    setupBuffers();
    setupRenderPasses();
    createFullquad();

    setupDefferedRender();

    Texture2D ts = Texture2D::create(512, 512, PIXEL_FORMAT::RGBA8, PIXEL_INTERNAL_FORMAT::IFRGB);
};

/*bool32 Render::loadFont(const char *path)
{
    FT_Library lib = {};
    FT_Face face = {};
 
    if (FT_Init_FreeType(&lib))
    {
        print_error("FREETYPE: Couldnt init free type lib");
    }

    if (FT_New_Face(lib, path, 0, &face))
    {
        print_error("FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (int c = 0; c < 128; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            print_error("FREETYPE: Failed to load char");
            continue;
        }

        Texture2D texture_symbol = {};

        graphics_state.graphics_device->createTexture(&texture_symbol);
        graphics_state.graphics_device->bindTexture(TEXTURE_TYPE::COLOR, &texture_symbol);

        graphics_state.graphics_device->loadTextureGpu(TEXTURE_TYPE::COLOR, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_RED, (void**)face->glyph->bitmap.buffer);

        graphics_state.graphics_device->setTextureWrapping(TEXTURE_TYPE::COLOR, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
        graphics_state.graphics_device->setTextureWrapping(TEXTURE_TYPE::COLOR, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

        graphics_state.graphics_device->setTextureSampling(TEXTURE_TYPE::COLOR, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR);
        graphics_state.graphics_device->setTextureSampling(TEXTURE_TYPE::COLOR, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

        Character chacter =
        {
            texture_symbol.id,
            createVec2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            createVec2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        characters_map.insert(std::pair<GLchar, Character>(c, chacter));
    }

    graphics_state.graphics_device->unbindTexture(TEXTURE_TYPE::COLOR);
    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    VertexArray *va = new VertexArray();
    VertexBuffer *vb = new VertexBuffer();
    graphics_state.graphics_device->createVertexArray(va);
    graphics_state.graphics_device->bindVertexArray(va);

    graphics_state.graphics_device->createVertexBuffer(NULL, 24 * sizeof(real32), DRAW_TYPE::DYNAMIC, vb);

    BufferLayout buffer_layout = {};

    std::initializer_list<BufferElement> init_list =
    {
        { "aPos",    ElementType::Float4, },
    };

    graphics_state.graphics_device->createBufferLayout(init_list, &buffer_layout);
    graphics_state.graphics_device->setVertexBufferLayout(vb, &buffer_layout);
    graphics_state.graphics_device->addVertexBuffer(va, vb);

    graphics_state.graphics_device->unbindBuffer(BUFFER_TYPE::VERTEX);
    graphics_state.graphics_device->unbindVertexArray();

    Matrix4x4 ortho = createMat4x4();

    //ortho = glm::ortho(0.0f, float(WINDOWS_WIDTH), 0.0f, float(WINDOWS_HEIGHT), -1.0f, 1.0f);

    Shader *text_shader = &shaders["text"];

    graphics_state.graphics_device->bindShader(text_shader);
    graphics_state.graphics_device->setMat4("projection", ortho, text_shader);

    return true;
}*/

void Render::addShader(const std::string &ac_name, Shader shr)
{
    shr.setName(ac_name);

    graphics_state.gpu_resources.shaders[ac_name] = shr;
}

void Render::addTexture(const std::string &ac_name, Texture2D tex)
{
    tex.name = ac_name;
    graphics_state.gpu_resources.textures[ac_name] = tex;
}

void Render::addMaterial(const std::string &mat_name, Material mat)
{
    graphics_state.materials[mat_name] = mat;
}

void Render::addRenderPass(const std::string &pass_name, RenderPass pass)
{
    graphics_state.render_passes[pass_name] = pass;
}

bool32 Render::loadShaders()
{
    // ???
    std::string resolved_path = "shaders/glsl/";

    Shader simple_3dcolor = Shader::create("shaders/glsl/3dcolor.glsl");
    Shader anim_model = Shader::create("shaders/glsl/anim_model3d.glsl");
    Shader model_shader = Shader::create("shaders/glsl/model3d.glsl");
    Shader d2shader = Shader::create("shaders/glsl/2dshader.glsl");
    Shader simple_shader = Shader::create("shaders/glsl/simple2d.glsl");
    Shader simple_texture = Shader::create("shaders/glsl/simple_model.glsl");
    Shader water = Shader::create("shaders/glsl/water.glsl");
    Shader gamma_correction_shader = Shader::create("shaders/glsl/gamma_correction.glsl");
    Shader text_shader = Shader::create("shaders/glsl/text.glsl");
    Shader cubemap_shader = Shader::create("shaders/glsl/cube_map.glsl");
    Shader shadow_map_shader = Shader::create("shaders/glsl/shadow_map.glsl");
    Shader shadow_map_depth_shader = Shader::create("shaders/glsl/shadow_map_extract.glsl");
    Shader post_proc_shader = Shader::create("shaders/glsl/fullquad.glsl");

    Shader pbr = Shader::create("shaders/glsl/pbr/pbr.glsl");
    Shader background_shader = Shader::create("shaders/glsl/pbr/background.glsl");
    Shader brdf_shader = Shader::create("shaders/glsl/pbr/brdf.glsl");
    Shader prefilter_shader = Shader::create("shaders/glsl/pbr/pref.glsl");
    Shader equirectangular_cubemap = Shader::create("shaders/glsl/pbr/equ_to_cubemap.glsl");
    Shader irradiance_shader = Shader::create("shaders/glsl/pbr/irradiance.glsl");
    Shader triangle = Shader::create("shaders/glsl/triangle.glsl");
    Shader terrain = Shader::create("shaders/glsl/terrain.glsl");

    Shader color_shader = Shader::create("shaders/glsl/color.glsl");
    
    Shader deffered = Shader::create("shaders/glsl/def_shade.glsl");
    Shader light_pass = Shader::create("shaders/glsl/light_pass.glsl");

    addShader("deffered", deffered);
    addShader("lightpass", light_pass);
    addShader("render2d", d2shader);
    addShader("3dcolor", simple_3dcolor);
    addShader("simple2d", simple_shader);
    addShader("simple_tex", simple_texture);
    addShader("base3d", model_shader);
    addShader("gc", gamma_correction_shader);
    addShader("text", text_shader);
    addShader("cubemap", cubemap_shader);
    addShader("color", color_shader);
    addShader("shadow_map", shadow_map_shader);
    addShader("shadow_depth", shadow_map_depth_shader);
    addShader("post_proc", post_proc_shader);
    addShader("pbr", pbr);
    addShader("background", background_shader);
    addShader("brdf", brdf_shader);
    addShader("prefilter", prefilter_shader);
    addShader("equirect", equirectangular_cubemap);
    addShader("irradiance", irradiance_shader);
    addShader("water", water);
    addShader("skeletal_animation", anim_model);
    addShader("triangle", triangle);

    print_info("Shaders loaded!!!");
    return true;
}

bool32 Render::loadFreeTextures()
{
    // pbr

    Texture2D albedo_iron = {};
    Texture2D normal_iron = {};
    Texture2D metallic_iron = {};
    Texture2D roughness_iron = {};
    Texture2D ao_iron = {};

    Texture2D albedo_plastic = {};
    Texture2D normal_plastic = {};
    Texture2D metallic_plastic = {};
    Texture2D roughness_plastic = {};
    Texture2D ao_plastic = {};

    Texture2D albedo_grass = {};
    Texture2D normal_grass = {};
    Texture2D metallic_grass = {};
    Texture2D roughness_grass = {};
    Texture2D ao_grass = {};

    Texture2D albedo_gold = {};
    Texture2D normal_gold = {};
    Texture2D metallic_gold = {};
    Texture2D roughness_gold = {};
    Texture2D ao_gold = {};

    Texture2D albedo_wall = {};
    Texture2D normal_wall = {};
    Texture2D metallic_wall = {};
    Texture2D roughness_wall = {};
    Texture2D ao_wall = {};

    // hdr env map
    Texture2D hdr = {};

    Texture2D cerberus_d = {};
    Texture2D cerberus_n = {};
    Texture2D cerberus_r = {};
    Texture2D cerberus_m = {};
    Texture2D cerberus_ao = {};

    Texture2D gun_diff = {};
    Texture2D character_diff = {};

    /*device->createTexture2D("assets/cerberus/albedo.tga", &cerberus_d);
    device->createTexture2D("assets/cerberus/normal.tga", &cerberus_n);
    device->createTexture2D("assets/cerberus/roughness.tga", &cerberus_r);
    device->createTexture2D("assets/cerberus/metallic.tga", &cerberus_m);
    device->createTexture2D("assets/cerberus/ao.tga", &cerberus_ao);

    device->createTexture2D("assets/pbr/rusted_iron/albedo.png", &albedo_iron);
    device->createTexture2D("assets/pbr/rusted_iron/normal.png", &normal_iron);
    device->createTexture2D("assets/pbr/rusted_iron/metallic.png", &metallic_iron);
    device->createTexture2D("assets/pbr/rusted_iron/roughness.png", &roughness_iron);
    device->createTexture2D("assets/pbr/rusted_iron/ao.png", &ao_iron);

    device->createTexture2D("assets/pbr/grass/albedo.png", &albedo_grass);
    device->createTexture2D("assets/pbr/grass/normal.png", &normal_grass);
    device->createTexture2D("assets/pbr/grass/metallic.png", &metallic_grass);
    device->createTexture2D("assets/pbr/grass/roughness.png", &roughness_grass);
    device->createTexture2D("assets/pbr/grass/ao.png", &ao_grass);

    device->createTexture2D("assets/pbr/wall/albedo.png", &albedo_wall);
    device->createTexture2D("assets/pbr/wall/normal.png", &normal_wall);
    device->createTexture2D("assets/pbr/wall/metallic.png", &metallic_wall);
    device->createTexture2D("assets/pbr/wall/roughness.png", &roughness_wall);
    device->createTexture2D("assets/pbr/wall/ao.png", &ao_wall);

    device->createTexture2D("assets/pbr/plastic/albedo.png", &albedo_plastic);
    device->createTexture2D("assets/pbr/plastic/normal.png", &normal_plastic);
    device->createTexture2D("assets/pbr/plastic/metallic.png", &metallic_plastic);
    device->createTexture2D("assets/pbr/plastic/roughness.png", &roughness_plastic);
    device->createTexture2D("assets/pbr/plastic/ao.png", &ao_plastic);

    device->createTexture2D("assets/pbr/gold/albedo.png", &albedo_gold);
    device->createTexture2D("assets/pbr/gold/normal.png", &normal_gold);
    device->createTexture2D("assets/pbr/gold/metallic.png", &metallic_gold);
    device->createTexture2D("assets/pbr/gold/roughness.png", &roughness_gold);
    device->createTexture2D("assets/pbr/gold/ao.png", &ao_gold);

    device->createTexture2D("assets/m1911/m1911_color.png", &gun_diff);
    device->createTexture2D("assets/animated/character_diff.jpg", &character_diff);

    //device->createTexture2D("assets/hdr/barce_3k.hdr", TEXTURE_TYPE::HDR, false, &hdr);*/

    Texture2D wood_texture = Texture2D::create("assets/get.png");
    Texture2D water_texture = Texture2D::create("assets/water-texture.jpg");

    addTexture("wood", wood_texture);
    addTexture("water", water_texture);

    /*addTexture("gun_diff", gun_diff);
    addTexture("girl_diffuse", character_diff);

    addTexture("albedo_iron", albedo_iron);
    addTexture("normal_iron", normal_iron);
    addTexture("metallic_iron", metallic_iron);
    addTexture("roughness_iron", roughness_iron);
    addTexture("ao_iron", ao_iron);

    addTexture("albedo_grass", albedo_grass);
    addTexture("normal_grass", normal_grass);

    addTexture("metallic_grass", metallic_grass);
    addTexture("roughness_grass", roughness_grass);
    addTexture("ao_grass", ao_grass);

    addTexture("albedo_wall", albedo_wall);
    addTexture("normal_wall", normal_wall);
    addTexture("metallic_wall", metallic_wall);
    addTexture("roughness_wall", roughness_wall);;
    addTexture("ao_wall", ao_wall);

    addTexture("albedo_gold", albedo_gold);
    addTexture("normal_gold", normal_gold);
    addTexture("metallic_gold", metallic_gold);
    addTexture("roughness_gold", roughness_gold);
    addTexture("ao_gold", ao_gold);

    addTexture("albedo_plastic", albedo_plastic);
    addTexture("normal_plastic", normal_plastic);
    addTexture("metallic_plastic", metallic_plastic);
    addTexture("roughness_plastic", roughness_plastic);
    addTexture("ao_plastic", ao_plastic);

    addTexture("hdr_env", hdr);
    addTexture("cerberus_d", cerberus_d);
    addTexture("cerberus_n", cerberus_n);
    addTexture("cerberus_r", cerberus_r);
    addTexture("cerberus_m", cerberus_m);
    addTexture("cerberus_ao", cerberus_ao);*/
  
    print_info("Free textures loaded!!!");
    return true;
}

bool32 Render::initRenderGL()
{
    bool32 loaded_shaders = false;
    bool32 loaded_tex = false;
    bool32 loaded_font = false;

    if (!loadShaders())
    {
        print_error("Failed to init shader module!");
    }
    /*std::thread l_shaders([&loaded_shaders]()
    {
        loaded_shaders = load_shaders();
    });*/

    /*std::thread l_fre_tex([&loaded_tex]()
    {
        loaded_tex = load_free_textures();
    });

    std::thread l_font([&loaded_font]()
    {
        loaded_font = load_font("assets/fonts/arial.ttf");
    });/*/

    if (!loadFreeTextures())
    {
        print_error("Failed to init textures module!");
    }

    /*if (!loadFont("assets/fonts/arial.ttf"))
    {
        print_error("Failed to init font module!");
    }*/

    initCommonGpuResources();

    return true;
}

bool32 Render::initRenderDX11()
{

    return true;
}

void Render::initGui()
{
    //@ Add from GUI Init
    //initImguiImpl();
}

bool32 Render::initCommonGpuResources()
{
    /*if (skybox_obj == nullptr)
    {
        skybox_obj = new Skybox();
    }

    bool32 is_skybox_created = createSkybox(skybox_obj);

    if (!is_skybox_created)
    {
        xe_utility::error("Failed to init skybox!");
        return false;
    }

    Shader *texture_shader = getShader("simple_tex");

    graphics_state.graphics_state.graphics_device->bindShader(texture_shader);
    graphics_state.graphics_state.graphics_device->setInt("tex_diff", 0, texture_shader);

    Shader *pbr = getShader("pbr");

    graphics_state.graphics_state.graphics_device->bindShader(pbr);
    graphics_state.graphics_state.graphics_device->setInt("irradiance_map", 0, pbr);
    graphics_state.graphics_state.graphics_device->setInt("prefilter_map", 1, pbr);
    graphics_state.graphics_state.graphics_device->setInt("brdf_LUT", 2, pbr);

    graphics_state.graphics_state.graphics_device->setInt("albedo_map", 3, pbr);
    graphics_state.graphics_state.graphics_device->setInt("normal_map", 4, pbr);
    graphics_state.graphics_state.graphics_device->setInt("metallic_map", 5, pbr);
    graphics_state.graphics_state.graphics_device->setInt("roughness_map", 6, pbr);
    graphics_state.graphics_state.graphics_device->setInt("ao_map", 7, pbr);

    Shader *equ_to_cubemap = getShader("equirect");

    graphics_state.graphics_state.graphics_device->bindShader(equ_to_cubemap);
    graphics_state.graphics_state.graphics_device->setInt("equirectangular_map", 0, equ_to_cubemap);

    Shader *animation = getShader("skeletal_animation");
    graphics_state.graphics_state.graphics_device->bindShader(animation);
    graphics_state.graphics_state.graphics_device->setInt("tex_diff1", 0, animation);
    //graphics_state.graphics_state.graphics_device->set_int("tex_norm1", 1, animation);


    createLinesBuffer();
    createQuadBuffer();*/

    return true;
}

void Render::viewport(int32 w, int32 h)
{
    GraphicsContext *context = getContext();
    Viewport &vp = context->getViewport();
    
    vp.width = w;
    vp.height = h;
     
    //Render::pushCommand([=]()
    //{
        context->setViewport(0, 0, w, h);
    //});
}

void Render::clear(uint32 flags)
{
    g_context->clearColor(graphics_state.target_clear_color);
    g_context->clear(flags);
}

void Render::clearColor(real32 x, real32 y, real32 z, real32 a)
{
    graphics_state.target_clear_color.x = x;
    graphics_state.target_clear_color.y = y;
    graphics_state.target_clear_color.z = z;
    graphics_state.target_clear_color.w = a;
}

void Render::shutdown()
{
    // @Clear destroy free textures
    auto textures = graphics_state.gpu_resources.textures;

    std::unordered_map<std::string, Texture2D>::iterator it = textures.begin();
    for (uint32 i = 0; i < textures.size(); ++i)
    {
        g_device->destroyTexture2D(&it->second);
    }

    //clearContextGui();
}

bool32 Render::createMesh(Mesh *meh, Vertex *vertex_type, bool32 calculate_tspace)
{
    pushCommand([meh, vertex_type, calculate_tspace]()
    {
        Vertex *vert = vertex_type;

        meh->vao = VertexArray::create();

        meh->vao.buffers.push_back(new VertexBuffer());
        meh->vao.ib = new IndexBuffer();

        BufferLayout buffer_layout = {};

        std::initializer_list<BufferElement> init_list;

        if (calculate_tspace)
        {
            init_list =
            {
                { "aPos",       ElementType::Float3, },
                { "aNormal",    ElementType::Float3, },
                { "aTangent",   ElementType::Float3, },
                { "aBitangent", ElementType::Float3, },
                { "aUV",        ElementType::Float2, }
            };
        }
        else
        {
            init_list =
            {
                { "aPos",       ElementType::Float3, },
                { "aNormal",    ElementType::Float3, },
                { "aUV",        ElementType::Float2, }
            };
        }

        g_context->bindVertexArray(&meh->vao);

        g_device->createVertexBuffer(&meh->vertices_fl[0], meh->vertices_fl.size() * sizeof(real32), DRAW_TYPE::STATIC, meh->vao.buffers[0]);

        // TODO: rework
        g_device->createIndexBuffer(&meh->indices[0], meh->indices.size(), meh->vao.ib);

        g_device->createBufferLayout(init_list, &buffer_layout);
        g_device->setVertexBufferLayout(meh->vao.buffers[0], &buffer_layout);
        g_device->addVertexBuffer(&meh->vao, meh->vao.buffers[0]);
        g_device->setIndexBuffer(&meh->vao, meh->vao.ib);

        g_context->unbindVertexArray();
        //device->destroyBuffer(meh->vao.buffers[0]);
        //device->destroyBuffer(meh->vao.ib);
    });

    return true;
}

bool32 Render::createLineMesh(const Vec3 &start, const Vec3 &end, LineMesh *line_com)
{
    /*std::vector<real32> array_vertex;

    array_vertex.push_back(start.x);
    array_vertex.push_back(start.y);
    array_vertex.push_back(start.z);
    array_vertex.push_back(end.x);
    array_vertex.push_back(end.y);
    array_vertex.push_back(end.z);

    line_com->va = alloc_mem VertexArray();
    line_com->va->buffers.push_back(alloc_mem VertexBuffer());

    line_com->vertex_count = array_vertex.size();

    graphics_state.graphics_state.graphics_device->createVertexArray(line_com->va);
    graphics_state.graphics_state.graphics_device->bindVertexArray(line_com->va);

    graphics_state.graphics_state.graphics_device->createVertexBuffer(&array_vertex[0], line_com->vertex_count * sizeof(real32), DRAW_TYPE::STATIC, line_com->va->buffers[0]);

    using namespace xe_graphics;

    BufferLayout buffer_layout = {};

    std::initializer_list<BufferElement> init_list =
    {
        { "aPos",    ElementType::Float3, }
    };

    graphics_state.graphics_state.graphics_device->createBufferLayout(init_list, &buffer_layout);
    graphics_state.graphics_state.graphics_device->setVertexBufferLayout(line_com->va->buffers[0], &buffer_layout);
    graphics_state.graphics_state.graphics_device->addVertexBuffer(line_com->va, line_com->va->buffers[0]);*/

    return true;
}

bool32 Render::createLineMesh(LineMesh *line_com)
{
    Vec3 start = createVec3(0.0f, 0.0f, 0.0f);
    Vec3 end = createVec3(1.0f, 1.0f, 0.0f);
    return createLineMesh(start, end, line_com);
}

bool32 Render::createLinesBuffer()
{
    RenderState &rs = graphics_state.render_state_batch;
  
    rs.line_vb_base = alloc_mem LineVertexMesh[rs.max_line_vert];

    g_device->createVertexArray(&rs.line_vertex_array);
    g_device->createVertexBuffer(nullptr, rs.max_line_vert * sizeof(LineVertexMesh), DRAW_TYPE::DYNAMIC, &rs.line_vertex_buffer);

    BufferLayout buffer_layout3 = {};

    std::initializer_list<BufferElement> init_list3 =
    {
        { "aPos",       ElementType::Float3, },
        { "aColor",     ElementType::Float4 }
    };

    g_device->createBufferLayout(init_list3, &buffer_layout3);
    g_device->setVertexBufferLayout(&rs.line_vertex_buffer, &buffer_layout3);
    g_device->addVertexBuffer(&rs.line_vertex_array, &rs.line_vertex_buffer);

    uint32* line_ind = alloc_mem uint32[rs.max_line_ind];
    for (uint32_t i = 0; i < rs.max_line_ind; i++)
    {
        line_ind[i] = i;
    }

    IndexBuffer *ib = alloc_mem IndexBuffer();

    g_device->createIndexBuffer(line_ind, rs.max_line_ind, ib);
    g_device->setIndexBuffer(&rs.line_vertex_array, ib);

    free_mem[] line_ind;

    return true;
}

bool32 Render::createQuadBuffer()
{
    RenderState &rs = graphics_state.render_state_batch;
    
    rs.quad_vertex_data[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    rs.quad_vertex_data[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    rs.quad_vertex_data[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    rs.quad_vertex_data[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    rs.quad_vb_base = alloc_mem QuadVertexMesh[rs.max_quad_vert];

    g_device->createVertexArray(&rs.quad_vertex_array);
    g_device->createVertexBuffer(nullptr, rs.max_quad_vert * sizeof(QuadVertexMesh), DRAW_TYPE::DYNAMIC, &rs.quad_vertex_buffer);

    BufferLayout quad_buffer_layout = {};

    std::initializer_list<BufferElement> quad_init_list =
    {
        { "aPos",       ElementType::Float3, },
        { "aColor",     ElementType::Float4 }
    };

    g_device->createBufferLayout(quad_init_list, &quad_buffer_layout);
    g_device->setVertexBufferLayout(&rs.quad_vertex_buffer, &quad_buffer_layout);
    g_device->addVertexBuffer(&rs.quad_vertex_array, &rs.quad_vertex_buffer);

    uint32* quad_ind = alloc_mem uint32[rs.max_quad_indices];
    uint32 offset = 0;

    for (uint32 i = 0; i < rs.max_quad_indices; i += 6)
    {
        quad_ind[i + 0] = offset + 0;
        quad_ind[i + 1] = offset + 1;
        quad_ind[i + 2] = offset + 2;

        quad_ind[i + 3] = offset + 2;
        quad_ind[i + 4] = offset + 3;
        quad_ind[i + 5] = offset + 0;

        offset += 4;
    }

    IndexBuffer *ib = alloc_mem IndexBuffer();

    g_device->createIndexBuffer(quad_ind, rs.max_quad_indices, ib);
    g_device->setIndexBuffer(&rs.quad_vertex_array, ib);

    free_mem[] quad_ind;

    return true;
}

bool32 Render::createRenderPass(const RenderPassData data, RenderPass *rp)
{
    assert(rp);

    rp->data = data;

    return true;
}

bool32 Render::createRenderPass(const char *name, bool32 clearDepth, bool32 clearColor, Framebuffer *active, RenderPass *rp)
{
    rp->data.name = name;
    rp->data.clearColor = clearColor;
    rp->data.clearDepth = clearDepth;
       
    rp->active_framebuffer = active;

    return true;
}

bool32 Render::createCubemap(std::vector<const char*> paths, Cubemap *cube)
{
    for (uint32 i = 0; i < paths.size(); ++i)
    {
        g_device->createTexture2D(paths[i], cube->face_textures[i]);
    }

    return true;
}

bool32 Render::createQuad()
{
    real32 vertices[] =
    {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    uint32 indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    int vertex_size = sizeof(vertices) / sizeof(vertices[0]);
    int indices_size = sizeof(indices) / sizeof(indices[0]);

    /*render_state_batch.quad_vertex_array.buffers.push_back(alloc_mem VertexBuffer);
    render_state_batch.quad_vertex_array.ib = alloc_mem IndexBuffer;

    g_device->createVertexArray(&render_state_batch.quad_vertex_array);
    g_device->bindVertexArray(&render_state_batch.quad_vertex_array);

    g_device->createVertexBuffer(render_state_batch.quad_vertex_data, vertex_size * sizeof(real32), DRAW_TYPE::STATIC, render_state_batch.quad_vertex_array.buffers[0]);
    g_device->createIndexBuffer(indices, indices_size, render_state_batch.quad_vertex_array.ib);

    using namespace xe_graphics;

    BufferLayout buffer_layout = {};

    std::initializer_list<BufferElement> init_list =
    {
        { "aPos",    ElementType::Float3, },
        { "aUV",     ElementType::Float2, }
    };

    g_device->createBufferLayout(init_list, &buffer_layout);
    g_device->setVertexBufferLayout(q->vertex_array->buffers[0], &buffer_layout);
    g_device->addVertexBuffer(q->vertex_array, q->vertex_array->buffers[0]);
    g_device->setIndexBuffer(q->vertex_array, q->vertex_array->ib);*/
    
    return true;
}

bool32 Render::createFullquad()
{
    g_device->createVertexArray(&graphics_state.quad_vao);

    return true;
}

bool32 Render::createSkybox(Skybox *sky)
{
    real32 skybox_vertices[] =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    int vertex_size = sizeof(skybox_vertices) / sizeof(skybox_vertices[0]);

    sky->va = new VertexArray();
    sky->vb = new VertexBuffer();

    g_device->createVertexArray(sky->va);
    g_device->createVertexBuffer(skybox_vertices, vertex_size * sizeof(real32), DRAW_TYPE::STATIC, sky->vb);

    BufferLayout buffer_layout = {};

    std::initializer_list<BufferElement> init_list =
    {
        { "aPos",    ElementType::Float3, }
    };

    g_device->createBufferLayout(init_list, &buffer_layout);
    g_device->setVertexBufferLayout(sky->vb, &buffer_layout);
    g_device->addVertexBuffer(sky->va, sky->vb);

    sky->cubemap = alloc_mem Cubemap();

    if (!createCubemap(sky->cubemap))
    {
        return false;
    }

    return true;
}

bool32 Render::createCubemap(Cubemap *cube)
{
    cube->face_textures.reserve(16);

    std::vector<const char*> skybox_faces = {};
    skybox_faces.push_back("lakes_rt.tga");
    skybox_faces.push_back("lakes_lf.tga");
    skybox_faces.push_back("lakes_up.tga");
    skybox_faces.push_back("lakes_dn.tga");
    skybox_faces.push_back("lakes_bk.tga");
    skybox_faces.push_back("lakes_ft.tga");

    Texture2D *cubemap_texture = new Texture2D();
    cubemap_texture->desc.texture_type = CUBEMAP;
    cubemap_texture->desc.dimension = TEXTURE_2D;

    g_device->createTexture(cubemap_texture);
    g_context->bindTexture(TEXTURE_TYPE::CUBEMAP, cubemap_texture);

    for (uint32 i = 0; i < skybox_faces.size(); i++)
    {
        int channels = 0;
        std::string final_path = "assets/skybox/" + std::string(skybox_faces[i]);
        void *texture_data = xe_core::loadTextureFromDisc(final_path.c_str(), cubemap_texture->desc.width, cubemap_texture->desc.height, channels, 0, false);

        g_device->loadTextureGpu(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap_texture->desc.width, cubemap_texture->desc.height, GL_RGB, GL_RGB, texture_data);

        xe_core::deleteData(texture_data);
    }

    g_device->setTextureWrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    g_device->setTextureWrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    g_device->setTextureWrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_R, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

    g_device->setTextureSampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR);
    g_device->setTextureSampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

    cube->rhi.setID(cubemap_texture->rhi.getID());

    Shader *cubemap_shader = getShader("cubemap");
    Material skyBoxMaterial(cubemap_shader);

    skyBoxMaterial.set("skybox", ShaderUniformType::SamplerCube, 0);
    
    addMaterial("cubemap", skyBoxMaterial);

    return true;
}

bool32 Render::createShadowMaps(ShadowMap *shadow)
{
    constexpr uint32 SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    constexpr uint32 WIDTH = 1280, HEIGHT = 720;

    /*shadow->depth_fbo = {};

    shadow->width_map = SHADOW_WIDTH;
    shadow->height_map = SHADOW_HEIGHT;

    g_device->createFramebuffer(1, &shadow->depth_fbo);
    g_device->addDepthTexture2D(SHADOW_WIDTH, SHADOW_HEIGHT, &shadow->depth_fbo);

    real32 colorattach[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colorattach);

    g_device->bindFramebuffer(&shadow->depth_fbo);
    g_device->setTexture2DFbo(GL_DEPTH_ATTACHMENT, TEXTURE_TYPE::DEPTH, shadow->depth_fbo.depth_texture);
    g_device->setDrawBuffer(GL_NONE);
    g_device->setReadBuffer(GL_NONE);
    g_device->unbindFramebuffer();

    Framebuffer hdr = {};
    g_device->createFramebuffer(1, &hdr);
    g_device->bindFramebuffer(&hdr);

    Texture2D color_attach[2];

    g_device->createTexture(2, color_attach);

    //unsigned int colorBuffers[2];
    //glGenTextures(2, colorBuffers);

    for (uint32 i = 0; i < 2; ++i)
    {
        g_device->bindTexture(TEXTURE_TYPE::COLOR, &color_attach[i]);
        g_device->loadTextureGpu(TEXTURE_TYPE::COLOR, WIDTH, HEIGHT, GL_RGB16F, GL_RGB, NULL);

        g_device->setTextureWrapping(TEXTURE_TYPE::COLOR, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
        g_device->setTextureWrapping(TEXTURE_TYPE::COLOR, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

        g_device->setTextureSampling(TEXTURE_TYPE::COLOR, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR);
        g_device->setTextureSampling(TEXTURE_TYPE::COLOR, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

        g_device->setTexture2DFbo(GL_COLOR_ATTACHMENT0 + i, TEXTURE_TYPE::COLOR, &color_attach[i]);

        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, &color_attach->desc[i], 0);
    }

    g_device->createRenderbuffer(1, &shadow->depth_fbo);

    g_device->bindRenderbuffer(&shadow->depth_fbo);

    uint32 attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    g_device->setDrawBuffers(2, attachments);

    g_device->checkFramebuffer();
    g_device->unbindFramebuffer();

    Framebuffer p_fbo[2];
    Texture2D p_color_buffer[2];

    //glGenFramebuffers(2, pingphongFBO);
    //glGenTextures(2, pingcolorBuffer);

    for (uint16 i = 0; i < 2; ++i)
    {
        g_device->createFramebuffer(1, p_fbo + i);
    }

    for (uint16 j = 0; j < 2; ++j)
    {
        g_device->createTexture(1, p_color_buffer + j);
    }

    for (uint16 i = 0; i < 2; ++i)
    {
        g_device->bindFramebuffer(p_fbo + i);
        g_device->bindTexture(TEXTURE_TYPE::COLOR, p_color_buffer + i);

        //glBindFramebuffer(GL_FRAMEBUFFER, pingphongFBO[i]);
        //glBindTexture(GL_TEXTURE_2D, pingcolorBuffer[i]);

        g_device->loadTextureGpu(TEXTURE_TYPE::COLOR, WIDTH, HEIGHT, GL_RGB16F, GL_RGB, NULL);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);

        g_device->setTextureSampling(TEXTURE_TYPE::COLOR, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR);
        g_device->setTextureSampling(TEXTURE_TYPE::COLOR, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

        g_device->setTextureWrapping(TEXTURE_TYPE::COLOR, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
        g_device->setTextureWrapping(TEXTURE_TYPE::COLOR, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //graphics_state.graphics_state.graphics_device->addColorTexture2D(p_color_buffer + i, i, p_fbo + i);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingcolorBuffer[i], 0);

        g_device->checkFramebuffer();
    }*/

    return true;
}

bool32 Render::createSphere(SphereMesh *sphre)
{
    sphre->vertex_array = new VertexArray();
    sphre->vertex_array->buffers.push_back(new VertexBuffer());
    sphre->vertex_array->ib = new IndexBuffer();

    std::vector<Vec3> positions = {};
    std::vector<Vec2> uv = {};
    std::vector<Vec3> normals = {};
    std::vector<uint32> indices = {};

    /*for (uint32 y = 0; y <= 64; ++y)
    {
        for (uint32 x = 0; x <= 64; ++x)
        {
            real32 xSegment = (real32)x / (real32)64;
            real32 ySegment = (real32)y / (real32)64;
            real32 xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            real32 yPos = std::cos(ySegment * PI);
            real32 zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(createVec3(xPos, yPos, zPos));
            uv.push_back(createVec2(xSegment, ySegment));
            normals.push_back(createVec3(xPos, yPos, zPos));
        }
    }

    bool32 odd_row = false;
    for (int y = 0; y < 64; ++y)
    {
        if (!odd_row)
        {
            for (int x = 0; x <= 64; ++x)
            {
                indices.push_back(y       * (64 + 1) + x);
                indices.push_back((y + 1) * (64 + 1) + x);
            }
        }
        else
        {
            for (int x = 64; x >= 0; --x)
            {
                indices.push_back((y + 1) * (64 + 1) + x);
                indices.push_back(y       * (64 + 1) + x);
            }
        }
        odd_row = !odd_row;
    }

    std::vector<real32> data = {};
    for (int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);

        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }

        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
    }

    graphics_state.graphics_state.graphics_device->createVertexArray(sphre->vertex_array);
    graphics_state.graphics_state.graphics_device->bindVertexArray(sphre->vertex_array);

    graphics_state.graphics_state.graphics_device->createVertexBuffer(&data[0], data.size() * sizeof(real32), DRAW_TYPE::STATIC, sphre->vertex_array->buffers[0]);
    graphics_state.graphics_state.graphics_device->createIndexBuffer(&indices[0], indices.size(), sphre->vertex_array->ib);

    BufferLayout buffer_layout = {};

    std::initializer_list<BufferElement> init_list =
    {
        { "aPos",       ElementType::Float3, },
        { "aUV",        ElementType::Float2, },
        { "aNormal",    ElementType::Float3, }
    };

    graphics_state.graphics_state.graphics_device->createBufferLayout(init_list, &buffer_layout);
    graphics_state.graphics_state.graphics_device->setVertexBufferLayout(sphre->vertex_array->buffers[0], &buffer_layout);
    graphics_state.graphics_state.graphics_device->addVertexBuffer(sphre->vertex_array, sphre->vertex_array->buffers[0]);
    graphics_state.graphics_state.graphics_device->setIndexBuffer(sphre->vertex_array, sphre->vertex_array->ib);

    sphre->vertex_array->ib->count = indices.size();*/

    return true;
}

bool32 Render::createCube(CubeMesh *cube)
{
    //Render::pushCommand([=]()
    //{
        Array<real32> *data = mesh_factory.createArrayOfVertices(GeometryType::CUBE_MESH);

        cube->vertex_array = alloc_mem VertexArray();
        cube->vertex_array->buffers.push_back(alloc_mem VertexBuffer());

        g_device->createVertexArray(cube->vertex_array);
        g_context->bindVertexArray(cube->vertex_array);

        g_device->createVertexBuffer(data->begin(), data->size() * sizeof(real32), DRAW_TYPE::STATIC, cube->vertex_array->buffers[0]);

        BufferLayout buffer_layout = {};

        std::initializer_list<BufferElement> init_list =
        {
            { "aPos",       ElementType::Float3, },
            { "aNormal",    ElementType::Float3, },
            { "aUV",        ElementType::Float2, }
        };

        g_device->createBufferLayout(init_list, &buffer_layout);
        g_device->setVertexBufferLayout(cube->vertex_array->buffers[0], &buffer_layout);
        g_device->addVertexBuffer(cube->vertex_array, cube->vertex_array->buffers[0]);
    //});
    
    return true;
}

void Render::drawTriangle()
{    
    if (!graphics_state.is_inited_triangle)
    {
        //Render::pushCommand([=]()
        //{
            real32 vertices[] = {
                -0.5f, -0.5f, 0.0f,   
                 0.5f, -0.5f, 0.0f,   
                 0.0f,  0.5f, 0.0f   
            };

            glGenVertexArrays(1, &graphics_state.VAO);
            glGenBuffers(1, &graphics_state.VBO);

            glBindVertexArray(graphics_state.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, graphics_state.VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        //});

        graphics_state.is_inited_triangle = true;
    }

    //Render::pushCommand([=]()
    //{
        Shader *shd = getShader("triangle");
        
        g_context->bindShader(shd);
        
        glBindVertexArray(graphics_state.VAO);
        g_context->drawArray(PRIMITIVE_TOPOLOGY::TRIANGLE, 0, 3);
    //});
}

void Render::drawFullquad(Material *mat)
{    
    if (!mat)
    {
        return;
    }

    mat->bind();
    mat->activate();

    drawFullquad();

    mat->unbind();
}

void Render::drawFullquad()
{
    g_context->bindVertexArray(&graphics_state.quad_vao);
    g_context->drawArray(PRIMITIVE_TOPOLOGY::TRIANGLE, 0, 6);
    g_context->unbindVertexArray();
}

void Render::drawQuad(const Vec2 &pos, const Vec2 &size, const ColRGBA &color)
{
    drawQuad({ pos.x, pos.y, 0.0f }, size, color);
}

void Render::drawQuad(const Vec3 &pos, const Vec2 &size, const ColRGBA& color)
{
    RenderState &rs = graphics_state.render_state_batch;

    if (rs.quad_index_count >= RenderState::max_quads_count)
    {
        //FlushAndResetLines();
    }

    //Matrix4x4 translationMatrix = graphics_state.IDENTITY_MATRIX;
    //translateMat(translationMatrix, pos);

    //Matrix4x4 scaleMatrix = graphics_state.IDENTITY_MATRIX;
    //scaleMat(scaleMatrix, { size.x, size.y, 1.0f });

    Matrix4x4 model = translateMat(pos) * scaleMat({ size.x, size.y, 1.0f });

    rs.quads_count++;

    Vec4 qVert1 = model * rs.quad_vertex_data[0];
    Vec4 qVert2 = model * rs.quad_vertex_data[1];
    Vec4 qVert3 = model * rs.quad_vertex_data[2];
    Vec4 qVert4 = model * rs.quad_vertex_data[3];

    rs.quad_vb_ptr->pos = createVec3(qVert1.x, qVert1.y, 1.0f);
    rs.quad_vb_ptr->color = color;
    rs.quad_vb_ptr++;

    rs.quad_vb_ptr->pos = createVec3(qVert2.x, qVert2.y, 1.0f);
    rs.quad_vb_ptr->color = color;
    rs.quad_vb_ptr++;

    rs.quad_vb_ptr->pos = createVec3(qVert3.x, qVert3.y, 1.0f);
    rs.quad_vb_ptr->color = color;
    rs.quad_vb_ptr++;

    rs.quad_vb_ptr->pos = createVec3(qVert4.x, qVert4.y, 1.0f);
    rs.quad_vb_ptr->color = color;
    rs.quad_vb_ptr++;

    rs.quad_index_count += 6;
}

void Render::drawQuad(real32 x, real32 y, real32 w, real32 h, const ColRGBA& color)
{
    drawQuad({ x, y, 0.0f }, { w, h }, color);
}

void Render::drawQuad(real32 x, real32 t, real32 w, real32 h, Texture2D *texture)
{
    /*if (texture != nullptr)
    {
        graphics_state.graphics_state.graphics_device->bindTexture(TEXTURE_TYPE::COLOR, texture);
    }

    q->vertex_array = rect_vao.vertex_array;

    glm::mat4 model = IDENTITY_MATRIX;

    graphics_state.graphics_state.graphics_device->bindShader(shd);

    if (q->w == 0 || q->h == 0)
    {
        q->w = 10;
        q->h = 10;
    }

    model = glm::translate(model, glm::vec3(q->x, q->y, 0));
    model = glm::scale(model, glm::vec3(q->w, q->h, 1.0f));

    xe_ecs::Camera2DComponent& camera2d = getCamera2D();

    glm::mat4 view_projection = camera2d.get_view_projection();
    graphics_state.graphics_state.graphics_device->setMat4("mvp", view_projection * model, shd);

    graphics_state.graphics_state.graphics_device->bindVertexArray(q->vertex_array);
    graphics_state.graphics_state.graphics_device->drawIndexed(PRIMITIVE_TOPOLOGY::TRIANGLE, 6, GL_UNSIGNED_INT, 0);
    graphics_state.graphics_state.graphics_device->unbindVertexArray();
    graphics_state.graphics_state.graphics_device->unbindShader();*/


}

/*void drawQuad(Shader *shd, Texture2D *texture)
{
    using namespace xe_ecs;
    QuadComponent* mesh = ent->findComponent<QuadComponent>();
    TransformComponent *tr = ent->findComponent<TransformComponent>();

    if (mesh == nullptr)
        return;

    graphics_state.graphics_state.graphics_device->bindShader(shd);

    if (texture != nullptr)
        graphics_state.graphics_state.graphics_device->activateBindTexture(TEXTURE_TYPE::COLOR, texture);

    glm::mat4 model_matrix = IDENTITY_MATRIX;

    model_matrix = glm::translate(model_matrix, tr->position);
    model_matrix = glm::scale(model_matrix, tr->scale);

    xe_ecs::Camera2DComponent& camera2d = getCamera2D();

    glm::mat4 view_projection = camera2d.get_view_projection();
    glm::mat4 mvp = view_projection * model_matrix;

    graphics_state.graphics_state.graphics_device->setMat4("mvp", mvp, shd);

    graphics_state.graphics_state.graphics_device->bindVertexArray(mesh->quad_mesh->vertex_array);

    graphics_state.graphics_state.graphics_device->drawIndexed(PRIMITIVE_TOPOLOGY::TRIANGLE, 6, GL_UNSIGNED_INT, 0);

    graphics_state.graphics_state.graphics_device->unbindVertexArray();
    graphics_state.graphics_state.graphics_device->unbindShader();
}*/

void Render::drawModel(Model *mod, Material *mat)
{
    ModelNode *root = mod->root;

    mat->bind();    
    
    if (mat->isDepth())
    {
        g_context->enable(GL_DEPTH_TEST);
    }

    for (uint32 i = 0; i < root->children.size(); i++)
    {
        ModelNode* curr_node = root->children[i];

        for (uint32 j = 0; j < curr_node->meshes.size(); j++)
        {
            Mesh *cur_mesh = curr_node->meshes.at(j);
            drawMesh(cur_mesh, mat);
        }
    }

    mat->unbind();
    
    if (mat->isDepth())
    {
        g_context->disable(GL_DEPTH_TEST);
    }
}

/*void drawModel(xe_assets::AnimModel *mod, Shader *shd, const Matrix4x4 &transform)
{
    GraphicsDevice *device = xe_render::getDevice();
    device->bindShader(shd);

    Texture2D *texture_diff = xe_render::getTexture2DResource("girl_diffuse");
    Texture2D *texture_norm = xe_render::getTexture2DResource("girl_norm");

    if (texture_diff != nullptr && texture_norm != nullptr)
    {
        device->activateBindTexture(TEXTURE_TYPE::COLOR, texture_diff);
        //device->activate_bind_texture(TEXTURE_TYPE::COLOR, texture_diff);
    }

    Camera3DComponent &camera = getCamera3D();

    for (xe_assets::AnimNode& mesh : mod->anim_meshes)
    {
        for (size_t i = 0; i < mod->bone_transformation.size(); i++)
        {
            std::string uniformName = std::string("u_bones[") + std::to_string(i) + std::string("]");
            device->setMat4(uniformName, mod->bone_transformation[i], shd);
        }

        glm::mat4 ide_model = IDENTITY_MATRIX;
        ide_model = transform * mesh.transform;

        device->setMat4("model", ide_model, shd);
        device->setMat4("vp", camera.get_view_projection(), shd);
        device->bindVertexArray(&mod->va);
        device->drawIndexed(PRIMITIVE_TOPOLOGY::TRIANGLE, mesh.index_count, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * mesh.start_index));
    }

    device->unbindShader();
}*/

void Render::drawText(const std::string &text, real32 x, real32 y, Vec3 &color, real32 scale)
{
    GraphicsDevice *device = getDevice();
    GraphicsContext *context = getContext();

    context->enable(GL_BLEND);
    context->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader *text_shader = getShader("text");

    context->bindShader(text_shader);
    //device->setVec3("color", color);

    context->activateTexture(0);

    VertexArray va = {};
    va.rhi.setID(1);

    context->bindVertexArray(&va);

    std::string::const_iterator c;
    
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = graphics_state.gpu_resources.characters_map.at(*c);

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };

        context->bindTexture(TEXTURE_TYPE::COLOR, ch.texture_id);

        //glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, 1);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        context->unbindBuffer(BUFFER_TYPE::VERTEX);

        context->drawArray(PRIMITIVE_TOPOLOGY::TRIANGLE, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    context->unbindVertexArray();
    context->unbindTexture(TEXTURE_TYPE::COLOR);

    context->disable(GL_BLEND);
}

void Render::drawText(const std::string &text, Vec2 &pos, Vec3 &color)
{
    //drawText(text, pos.x, pos.y, color, default_text_scale);
}

void Render::drawText(const std::string &text, Vec2 &pos)
{
    drawText(text, pos.x, pos.y, graphics_state.default_text_color, graphics_state.default_text_scale);
}

void Render::drawText(const std::string &text, real32 x, real32 y)
{
    drawText(text, x, y, graphics_state.default_text_color, graphics_state.default_text_scale);
}

void Render::drawText(const std::string & text, real32 x, real32 y, real32 scale)
{
    //Vec3 text_color = convertToVec3(default_text_color);
    drawText(text, x, y, graphics_state.default_text_color, scale);
}

void Render::drawWaterPlane(Entity *ent)
{
    /*Shader *water = xe_render::getShader("water");

    graphics_state.graphics_state.graphics_device->bindShader(water);

    WaterComponent *water_comp = ent->findComponent<xe_ecs::WaterComponent>();
    MeshComponent *mesh = ent->findComponent<xe_ecs::MeshComponent>();
    TransformComponent *transform = ent->findComponent<xe_ecs::TransformComponent>();

    Matrix4x4 model_matrix = IDENTITY_MATRIX;

    model_matrix = translateMat(model_matrix, transform->position);
    model_matrix = scaleMat(model_matrix, transform->scale);

    Camera3DComponent cam = getCamera3D();

    Matrix4x4 view_matrix = cam.get_view_matrix();
    Matrix4x4 proj_matrix = cam.get_projection_matrix();

    Matrix4x4 mvp = proj_matrix * view_matrix * model_matrix;

    graphics_state.graphics_state.graphics_device->setMat4("mvp", mvp, water);
    graphics_state.graphics_state.graphics_device->setMat4("model", model_matrix, water);

    graphics_state.graphics_device->activateBindTexture(TEXTURE_TYPE::COLOR, water_comp->water_tex);
    drawModel(mesh->model_asset, water);*/
}

void Render::drawSkybox()
{
    g_context->setDepthFunc(GL_LEQUAL);
    /*Shader *cubemap_shader = xe_render::getShader("cubemap");

    graphics_state.graphics_device->bindShader(cubemap_shader);

    Camera3D &camera = getCamera3D();

    Matrix4x4 view = createMat4(createMatrix3x3(camera.getViewMatrix()));

    //graphics_state.graphics_device->setMat4("view", view, cubemap_shader);
    //graphics_state.graphics_device->setMat4("projection", camera.get_projection_matrix(), cubemap_shader);

    context->bindVertexArray(skybox_obj->va);

    context->activateBindTexture(TEXTURE_TYPE::CUBEMAP, skybox_obj->cubemap);

    context->drawArray(PRIMITIVE_TOPOLOGY::TRIANGLE, 0, 36);

    context->setDepthFunc(GL_LESS);*/
}

void Render::drawAABB(const AABB &bb, const Matrix4x4 &matrix)
{
    /*Vec4 min = createVec4(bb.min.x, bb.min.y, bb.min.z, 1.0f);
    Vec4 max = createVec4(bb.max.x, bb.max.y, bb.max.z, 1.0f);

    Vec4 corners[8] =
    {
        matrix * createVec4( min.x, min.y, max.z, 1.0f),
        matrix * createVec4( min.x, max.y, max.z, 1.0f),
        matrix * createVec4( max.x, max.y, max.z, 1.0f),
        matrix * createVec4( max.x, min.y, max.z, 1.0f),

        matrix * createVec4( min.x, min.y, min.z, 1.0f),
        matrix * createVec4( min.x, max.y, min.z, 1.0f),
        matrix * createVec4( max.x, max.y, min.z, 1.0f),
        matrix * createVec4( max.x, min.y, min.z, 1.0f)
    };

    for (uint32_t i = 0; i < 4; i++)
    {
        auto &corner = corners[i];
        auto &corner_next = corners[(i + 1) % 4];
        drawLine(corner.x, corner.y, corner.z, corner_next.x, corner_next.y, corner_next.z);
    }

    for (uint32_t i = 0; i < 4; i++)
    {
        auto &corner = corners[i + 4];
        auto &corner_next = corners[((i + 1) % 4) + 4];
        drawLine(corner.x, corner.y, corner.z, corner_next.x, corner_next.y, corner_next.z);
    }

    for (uint32_t i = 0; i < 4; i++)
    {
        auto &corner = corners[i];
        auto &corner_next = corners[i + 4];
        drawLine(corner.x, corner.y, corner.z, corner_next.x, corner_next.y, corner_next.z);
    }*/
}

void Render::applyShadowMap(ShadowMap *shadow)
{
    real32 near_p = 1.0f;
    real32 far_p = 7.5f;

    //shadow->light_projection_matrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_p, far_p);
}

void Render::applyTransform(TransformComponent *transform, Shader *shd)
{
    Matrix4x4 model_matrix = createMat4x4();

    /*model_matrix = glm::translate(model_matrix, transform->position);
    model_matrix = glm::scale(model_matrix, transform->scale);

    Camera3DComponent camera = getCamera3D();

    Matrix4x4 view_matrix = camera.get_view_matrix();
    Matrix4x4 proj_matrix = camera.get_projection_matrix();

    Matrix4x4 mvp = proj_matrix * view_matrix * model_matrix;

    graphics_state.graphics_device->setMat4("mvp", mvp, shd);
    graphics_state.graphics_device->setMat4("model", model_matrix, shd);

    //graphics_state.graphics_device->set_vec3("dir_light_color", glm::vec3(1.0, 0.0, 0.0), shd);
    graphics_state.graphics_device->setVec3("light_pos", glm::vec3(4.0, 4.0, 0.0), shd);
    graphics_state.graphics_device->setVec3("cam_pos", camera.pos, shd);

    if (enable_shadows)
    {
        real32 near_p = 1.0f;
        real32 far_p = 7.5f;

        Matrix4x4 light_projection_matrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_p, far_p);
        Vec3 light_pos = glm::vec3(-2.0f, 4.0f, -2.0f);
        Matrix4x4 light_view_matrix = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        Matrix4x4 light_space_matrix = light_projection_matrix * light_view_matrix;

        shd = getShader("base3d");
        graphics_state.graphics_device->bindShader(shd);
        graphics_state.graphics_device->setMat4("light_space_matrix", light_space_matrix, shd);
        graphics_state.graphics_device->setBool("shadows_enabled", true, shd);
    }
    else
    {
        graphics_state.graphics_device->setBool("shadows_enabled", false, shd);
    }*/    
}

void Render::applyDirLight(Shader *shd, DirLightComponent *directional_light, TransformComponent *transform)
{
    Vec3 light_color = createVec3(1.0f, 1.0f, 1.0f);

    if (directional_light)
    {
        light_color = directional_light->color;
    }

    //graphics_state.graphics_device->bindShader(shd);
    //graphics_state.graphics_device->setVec3("dir_light_color", light_color);
    //graphics_state.graphics_device->setVec3("light_pos", transform->position);
}

void Render::applySpotLight(Shader *shd, SpotLightComponent *spot_light, TransformComponent *transform)
{


}

void Render::applyPointLight(Shader *shd, PointLightComponent *point_light, TransformComponent *transform)
{

}

void Render::beginFrame(bool32 shouldClearScreen)
{
    if (shouldClearScreen)
    {
        Render::pushCommand([]()
        {
            clearColor(0.7f, 0.7f, 0.9f, 1.0f);
            clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });
    }

    RenderState &rs = graphics_state.render_state_batch;
    rs.draw_calls = 0;
}

void Render::beginRenderPass(const char *name)
{
    RenderPass *current = &graphics_state.render_passes[name];

    if (!current)
    {
        return;
    }

    graphics_state.active_render_pass = current;

    Render::pushCommand([current]()
    {    
        g_context->bindFramebuffer(current->active_framebuffer);

        uint32 clearFlags = 0;

        current->data.clearColor > 0 ? clearFlags |= GL_COLOR_BUFFER_BIT : clearFlags &= ~GL_COLOR_BUFFER_BIT;
        current->data.clearDepth > 0 ? clearFlags |= GL_DEPTH_BUFFER_BIT : clearFlags &= ~GL_DEPTH_BUFFER_BIT;

        Render::clear(clearFlags);
    });
}

void Render::setupRenderCommand(CommandType type)
{
    RenderState &rs = graphics_state.render_state_batch;

    switch (type)
    {
    case CommandType::LINE_COMMAND:
    {
        rs.line_index_count = 0;
        rs.line_vb_ptr = rs.line_vb_base;
    } break;
    case CommandType::QUAD_COMMAND:
    {
        rs.quad_index_count = 0;
        rs.quad_vb_ptr = rs.quad_vb_base;
    } break;
    default:
        //printf_info("Set render command!");
        break;
    }
}
 
void Render::executeRenderCommand(CommandType type)
{
    uint32 dSize = 0;

    RenderState &rs = graphics_state.render_state_batch;
    GraphicsContext *context = getContext();

    switch (type)
    {
    case CommandType::QUAD_COMMAND:
    {
        dSize = (uint8*)rs.quad_vb_ptr - (uint8*)rs.quad_vb_base;

        if (dSize)
        {
            VertexBuffer &quad_vb = rs.quad_vertex_buffer;
            uint32 offset = 0;
            context->bindBuffer(&quad_vb);
            context->pushDataToBuffer(quad_vb.rhi.getID(), BUFFER_TYPE::VERTEX, offset, dSize, rs.quad_vb_base);
            drawQuads();           
        }
    } break;
    case CommandType::LINE_COMMAND:
    {
        dSize = (uint8_t*)rs.line_vb_ptr - (uint8_t*)rs.line_vb_base;

        if (dSize)
        {
            VertexBuffer &line_vb = rs.line_vertex_buffer;

            uint32 offset = 0;
            context->bindBuffer(&rs.line_vertex_buffer);
            context->pushDataToBuffer(line_vb.rhi.getID(), BUFFER_TYPE::VERTEX, offset, dSize, rs.line_vb_base);
            drawLines();
        }
    } break;
    default:
        break;
    }
}

void Render::executeCommands()
{    
    render_queue.executeQueue();
}

void Render::endFrame()
{
    g_device->checkError();
    executeCommands();
}

void Render::endRenderPass()
{
    RenderPass *active_pass = graphics_state.active_render_pass;
    active_pass = nullptr;

    Render::pushCommand([&]()
    {
        g_context->unbindFramebuffer();
    });
}

/*void Render::drawModel(xe_assets::Model *mod, const glm::mat4 &transform)
{
    Shader *static_model_shader = xe_render::getShader("model3d");
    drawModel(mod, static_model_shader, transform);
}*/

/*void Render::drawModel(xe_assets::AnimModel *mod, const Matrix4x4 &transform)
{
    Shader *animation_shader = xe_render::getShader("skeletal_animation");
    drawModel(mod, animation_shader, transform);
}*/

void Render::drawCube(Texture2D *texture_diff)
{
    if (texture_diff)
    {
        GraphicsContext *context = getContext();
        context->activateBindTexture(TEXTURE_TYPE::COLOR, texture_diff);
    }

    drawCube();
}

void Render::drawCube()
{ 
    if (!graphics_state.cube_vao.vertex_array)
    {
        createCube(&graphics_state.cube_vao);
    }
    
    //Render::pushCommand([=]()
    //{
        GraphicsContext *context = getContext();
        Shader *shd = getShader("color");
        Camera3D &camera = getCamera3D();

        Matrix4x4 model_matrix = graphics_state.IDENTITY_MATRIX;

        Vec3 positionTest = createVec3(0.0f, 0.0f, -50.0f);
        Vec3 scaleTest = createVec3(0.2f, 0.2f, 0.2f);
        
        translateMat(model_matrix, positionTest);
        scaleMat(model_matrix, scaleTest);
        
        // view matrix is broken
        Matrix4x4 view_matrix = inverseMat(model_matrix);
        Matrix4x4 proj_matrix = camera.getProjectionMatrix();
        
        Matrix4x4 mvp = proj_matrix * view_matrix * model_matrix;

        context->bindShader(shd);
        //device->setVec3("color", createVec3(1.0f, 0.0f, 0.0f));

        //device->setMat4("mvp", mvp);
        //device->setMat4("model", model_matrix);

        context->enable(GL_DEPTH_TEST);

        context->bindVertexArray(graphics_state.cube_vao.vertex_array);
        context->drawArray(PRIMITIVE_TOPOLOGY::TRIANGLE, 0, 36);
        context->unbindVertexArray();

        context->disable(GL_DEPTH_TEST);
    //});    
}

void Render::drawLine(real32 x, real32 y, real32 x_end, real32 y_end)
{
    ColRGB default_color = ColRGB(1.0f, 0.0f, 0.0f);
    drawLine(x, y, x_end, y_end, default_color);
}

void Render::drawLine(real32 x, real32 y, real32 z, real32 x_end, real32 y_end, real32 z_end)
{
    ColRGB default_color = ColRGB(1.0f, 0.0f, 0.0f);
    drawLine(x, y, z, x_end, y_end, z_end, default_color);
}

void Render::drawLine(real32 x, real32 y, real32 z, real32 x_end, real32 y_end, real32 z_end, ColRGBA color)
{
    /*if (graphics_state.line_index_count >= RenderState::max_line_ind)
    {
        //flushLines();
    }

    graphics_state.lines_count++;

    graphics_state.line_vb_ptr->pos = glm::vec3(x, y, z);
    graphics_state.line_vb_ptr->color = color;
    graphics_state.line_vb_ptr++;

    graphics_state.line_vb_ptr->pos = glm::vec3(x_end, y_end, z_end);
    graphics_state.line_vb_ptr->color = color;
    graphics_state.line_vb_ptr++;

    graphics_state.line_index_count += 2;*/
}

void Render::drawLine(real32 x, real32 y, real32 z, real32 x_end, real32 y_end, real32 z_end, ColRGB color)
{
    drawLine(x, y, z, x_end, y_end, z_end, ColRGBA(color.x, color.y, color.z, 1.0f));
}

void Render::drawLine(real32 x, real32 y, real32 x_end, real32 y_end, ColRGBA color)
{
    /*if (graphics_state.line_index_count >= RenderState::max_line_ind)
    {
        //flushLines();
    }

    graphics_state.lines_count++;

    graphics_state.line_vb_ptr->pos = glm::vec3(x, y, 0);
    graphics_state.line_vb_ptr->color = color;
    graphics_state.line_vb_ptr++;

    graphics_state.line_vb_ptr->pos = glm::vec3(x_end, y_end, 0);
    graphics_state.line_vb_ptr->color = color;
    graphics_state.line_vb_ptr++;

    graphics_state.line_index_count += 2;*/
}

void Render::drawLine(real32 x, real32 y, real32 x_end, real32 y_end, ColRGB color)
{
    drawLine(x, y, x_end, y_end, ColRGBA(color.x, color.y, color.z, 1.0f));
}

void Render::drawLines()
{
    Camera3D& camera = getCamera3D();
    Shader *line = getShader("line");
    GraphicsContext *context = getContext();
    RenderState &rs = graphics_state.render_state_batch;

    context->bindShader(line);

    //device->setMat4("vp", camera.getViewProjectionMatrix());

    context->bindVertexArray(&rs.line_vertex_array);
    context->setLineWidth(rs.default_line_width);
    context->drawIndexed(PRIMITIVE_TOPOLOGY::LINE, rs.line_index_count, GL_UNSIGNED_INT, 0);

    ++rs.draw_calls;
}

void Render::drawQuads()
{
    Material *mat = getMaterial("render2d");

    Camera2D& camera2d = getCamera2D();
    GraphicsContext *context = getContext();
    RenderState &rs = graphics_state.render_state_batch;

    mat->bind();
    mat->set("vp", ShaderUniformType::Mat4x4, &camera2d.getViewProjection());

    //device->bindShader(shd);
    //device->setMat4("vp", camera2d.getViewProjection());

    mat->activate();

    context->bindVertexArray(&rs.quad_vertex_array);
    context->drawIndexed(PRIMITIVE_TOPOLOGY::TRIANGLE, rs.quad_index_count, GL_UNSIGNED_INT, 0);

    ++rs.draw_calls;
}

void Render::drawEnt(Entity *ent)
{
    /*switch (type)
    {
    case ENTITY_TYPE::ENT_STATIC_OBJECT:
    case ENTITY_TYPE::ENT_DIR_LIGHT:
        drawEntStatic(ent);
        break;
    case ENTITY_TYPE::ENT_ANIMATED_OBJECT:

        break;
    case ENTITY_TYPE::ENT_PRIMITIVE_OBJECT:
        drawEntPrimitive(ent);
        break;
    case ENTITY_TYPE::ENT_WATER:
        drawWaterPlane(ent);
        break;
    case ENTITY_TYPE::ENT_LINE:
        drawLine(ent);
        break;
    default:
        xe_utility::debug("Entity type not declared!");
        break;
    }*/

}

void Render::drawEntStatic(Entity *ent)
{
    /*MeshComponent *model = ent->findComponent<MeshComponent>();

    Shader *shader_to_draw = nullptr;

    if (model)
        shader_to_draw = xe_render::getShader("base3d");

    if (model->draw_with_color)
        shader_to_draw = xe_render::getShader("color");

    TransformComponent *transform = ent->findComponent<TransformComponent>();

    if (transform)
    {
        device->bindShader(shader_to_draw);

        if (model->draw_with_color)
        {
            device->setVec3("color", convertToVec3(default_cube_color));
        }

        glm::mat4 transformMatrix = applyTransform(transform, shader_to_draw);

        /*if (model->diffuse_texture != nullptr)
        {
            device->set_int("tex_diff1", 0, shader_to_draw);
            device->activate_bind_texture(TEXTURE_TYPE::COLOR, model->diffuse_texture);
        }*/

        /*MeshNode *root = model->model_asset->root;

        for (uint32 i = 0; i < root->children.size(); i++)
        {
            MeshNode* curr_node = root->children[i];

            for (uint32 j = 0; j < curr_node->meshes.size(); j++)
            {
                Mesh *cur_mesh = curr_node->meshes.at(j);
                drawMesh(cur_mesh, shader_to_draw);

                aabb &bb = cur_mesh->bounding_box;
                drawAABB(bb, transformMatrix);
            }
        }

        device->unbindShader();
    }*/
}

void Render::drawEntWithShader(Entity *ent, Shader *shd)
{   
    /*MeshComponent *model = ent->findComponent<MeshComponent>();

    Shader *shader_to_draw = shd;

    TransformComponent *transform = ent->findComponent<TransformComponent>();

    if (transform)
    {
        device->bindShader(shader_to_draw);

        applyTransform(transform, shader_to_draw);

        xe_assets::Node *root = model->model_asset->root;

        for (uint32 i = 0; i < root->children.size(); i++)
        {
            xe_assets::Node* curr_node = root->children[i];

            for (uint32 j = 0; j < curr_node->meshes.size(); j++)
            {
                xe_assets::Mesh *cur_mesh = curr_node->meshes.at(j);
                drawMesh(cur_mesh, shader_to_draw);
            }
        }

        device->unbindShader();
    }*/
}

void drawEntPrimitive(Entity *ent)
{
    /*SphereComponent *sphere = ent->findComponent<SphereComponent>();

    if (sphere)
    {
        TransformComponent *transform = ent->findComponent<TransformComponent>();
        Shader *shd = xe_render::getShader("color");

        graphics_state.graphics_device->bindShader(shd);
        graphics_state.graphics_device->setVec3("color", convertToVec3(default_cube_color), shd);

        if (sphere->diffuse)
        {
            shd = xe_render::getShader("simple_tex");
            graphics_state.graphics_device->bindShader(shd);
            graphics_state.graphics_device->activateBindTexture(TEXTURE_TYPE::COLOR, sphere->diffuse);
        }

        applyTransform(transform, shd);

        drawSphere();
    }*/
}

void Render::drawMesh(Mesh *msh, Material *mat)
{
    /*for (uint32 i = 0; i < msh->mesh_textures.size(); i++)
    {
        uint32 diff_texture_num = 1;
        uint32 normal_texture_num = 1;
        uint32 specular_texture_num = 1;

        // @Refactor!!!
        Texture2D *mesh_texture = mat->getTexture(i);
        g_context->activateTexture(i);

        std::string name = msh->mesh_textures[i].type;
        std::string num;

        if (name == "tex_diff")
        {
            num = std::to_string(diff_texture_num++);
        }
        else if (name == "tex_norm")
        {
            num = std::to_string(normal_texture_num++);
        }
        else if (name == "tex_spec")
        {
            num = std::to_string(specular_texture_num++);
        }

        uint32 location = i;

        g_context->setInt(location, i);
        g_context->bindTexture(TEXTURE_TYPE::COLOR, mesh_texture);
    }*/
     
    mat->activate();

    g_context->bindVertexArray(&msh->vao);
    g_context->drawIndexed(PRIMITIVE_TOPOLOGY::TRIANGLE, msh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Render::drawSphere(Texture2D *texture_diff)
{
    if (texture_diff)
    {
        g_context->activateBindTexture(TEXTURE_TYPE::COLOR, texture_diff);
    }
    
    drawSphere();
}

void Render::drawSphere()
{
    if (!graphics_state.sphere_vao.vertex_array)
    {
        createSphere(&graphics_state.sphere_vao);
    }

    g_context->bindVertexArray(graphics_state.sphere_vao.vertex_array);
    g_context->drawIndexed(PRIMITIVE_TOPOLOGY::TRIANGLE_STRIP, graphics_state.sphere_vao.vertex_array->ib->count, GL_UNSIGNED_INT, 0);
    g_context->unbindVertexArray();
}

Texture2D* Render::getTextureFromRenderPass(const char *name, uint32 index)
{
    RenderPass *current = &graphics_state.render_passes[name];

    if (!current)
    {
        return nullptr;
    }

    Texture2D *tex = &current->active_framebuffer->tex_attachments[index];
    return tex;
}

RenderPass* Render::getRenderPass(const char *name)
{
    RenderPass *result = &graphics_state.render_passes[name];
    return result;
}

RenderCommandQueue::RenderCommandQueue()
{
    command_buffer_ptr_base = alloc_mem uint8_t[10 * 1024 * 1024];
    command_buffer_ptr = command_buffer_ptr_base;
    memset(command_buffer_ptr_base, 0, 10 * 1024 * 1024);
}

RenderCommandQueue::~RenderCommandQueue()
{
    free_mem[] command_buffer_ptr_base;
}

void *RenderCommandQueue::submit(RenderCommand func_ptr, uint32 size)
{    
    *(RenderCommand*)command_buffer_ptr = func_ptr;
    command_buffer_ptr += sizeof(func_ptr);

    *(uint32*)command_buffer_ptr = size;
    command_buffer_ptr += sizeof(uint32);

    void* memory = command_buffer_ptr;
    command_buffer_ptr += size;

    command_count++;

    return memory;
}

void RenderCommandQueue::executeQueue()
{
    for (uint32 i = 0; i < command_count; ++i)
    {
        RenderCommand function = *(RenderCommand*)command_buffer_ptr_base;     
        command_buffer_ptr_base += sizeof(RenderCommand);
                     
        uint32_t size = *(uint32*)command_buffer_ptr_base;
        command_buffer_ptr_base += sizeof(uint32);
        function(command_buffer_ptr_base);
        command_buffer_ptr_base += size;
    }

    command_buffer_ptr = command_buffer_ptr_base;
    command_count = 0;
}

RenderCommandQueue& Render::getRenderCommandQueue()
{
    return render_queue;
}