#include "xe_render_pass.h"
#include "xe_render.h"
#include "xe_assets.h"
#include "config.h"

#include "xe_scene.h"

#include "app_state.h"

using namespace xe_ecs;

void xe_graphics::render_pass2D::init()
{
    graphics_device *device = xe_render::get_device();

    result_texture = xe_render::get_texture2D_resource("water");

    simple_shader = xe_render::get_shader("simple2d");

    main_ent.add_component(new quad_component());
    main_ent.add_component(new transform_component());
    main_ent.add_component(new update_component());

    quad_component* mesh = main_ent.find_component<quad_component>();
    transform_component *transform = main_ent.find_component<transform_component>();

    transform->position = glm::vec3(200.0f, 200.0f, 0.0f);
    transform->scale = glm::vec3(100.0f, 100.0f, 100.0f);

    mesh->quad_mesh = new quad();
    xe_render::create_quad(mesh->quad_mesh);

    device->bind_shader(simple_shader);
    device->set_int("tex_diff", 0, simple_shader);
}

void xe_graphics::render_pass2D::clear()
{
  
}

void xe_graphics::render_pass2D::render()
{ 
    using namespace xe_render;

    application::application_state *app_state = application::get_app_state();

    draw_quad(&main_ent, simple_shader, result_texture);
    draw_text("FPS: " + std::to_string(app_state->fps), 10, 10);
}

void xe_graphics::render_pass2D::update(real32 dt)
{
    update_component *updater = main_ent.find_component<update_component>();

    if (!updater)
        return;

    transform_component *transform = main_ent.find_component<transform_component>();
    
    transform->position += glm::vec3(0.0f, 1.0f, 0.0f) * dt * 12.f;
    transform->scale += glm::vec3(1.0f, 1.0f, 0.0f) * dt * 12.f;

    if (transform->position.y >= 720)
        transform->position.y = 0;
    if (transform->scale.x >= 200.0f || transform->scale.y >= 200.0f)
    {
        transform->scale.x = 100.f;
        transform->scale.y = 100.f;
    }
}

void xe_graphics::render_pass3D::init()
{
    graphics_device *device = xe_render::get_device();
    
    shader *model_shader = xe_render::get_shader("base3d");
    shader *color_shader = xe_render::get_shader("color");
    shader *pbr = xe_render::get_shader("pbr");
    shader *water = xe_render::get_shader("water");

    device->bind_shader(water);
    device->set_int("tex_diff", 0, water);

    device->bind_shader(model_shader);
    device->set_int("tex_diff", 0, model_shader);

    device->bind_shader(color_shader);
    device->set_vec3("color", glm::vec3(0.5f, 0.9f, 0.9f), color_shader);

    device->create_texture2D(1280, 720, &color_texture);
   
    device->create_framebuffer(1, &fbo);  
    device->bind_framebuffer(&fbo);
    device->bind_for_write(&fbo);
    device->create_render_buffer(1, &fbo);

    device->add_color_texture2D(&color_texture, 0, &fbo);
    device->add_depth_texture2D(1280, 720, &fbo);

    device->check_framebuffer();
    device->unbind_framebuffer();




}

void xe_graphics::render_pass3D::clear()
{

}

void xe_graphics::render_pass3D::render()
{
    graphics_device *device = xe_render::get_device();
    
    shader *color_shader = xe_render::get_shader("color");
    shader *model_shader = xe_render::get_shader("base3d");
     
    viewport vp = device->get_viewport();

    device->bind_framebuffer(&fbo);
    device->set_viewport(0, 0, vp.width, vp.height);
    device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    device->enable(GL_DEPTH_TEST);
    device->enable(GL_CULL_FACE);
    device->set_cull_mode(GL_BACK);
    
    for (uint16 i = 0; i < current_scene->entities.size(); ++i)
    {
        entity *current_ent = current_scene->entities[i];
        
        dir_light *light = current_ent->find_component<dir_light>();
        transform_component *transform = current_ent->find_component<transform_component>();

        if(light)
            xe_render::apply_dir_light(model_shader, light, transform);
        
        xe_render::draw_ent(current_ent);
    }

    device->disable(GL_CULL_FACE);

    xe_render::draw_skybox();

    device->disable(GL_DEPTH_TEST);
    device->unbind_framebuffer();

    //device->set_viewport(0, 0, vp.width, vp.height);
}

void xe_graphics::render_pass3D::update(real32 dt)
{   
    xe_ecs::entity *light_ent = current_scene->directional_light;
    if (light_ent)
    {
        transform_component *light_transform = light_ent->find_component<transform_component>();

        if (light_transform)
        {
            glm::vec3 &pos = light_transform->position;
            light_transform->set_translation(pos.x + 12.2f * dt, pos.y, pos.z + 12.2f * dt);
        }
    }
}

void xe_graphics::gamma_correction_pass::init()
{
    graphics_device *device = xe_render::get_device();
    gmshd = xe_render::get_shader("gc");
    device->bind_shader(gmshd);
    device->set_int("tex_diff", 0, gmshd);
}

void xe_graphics::gamma_correction_pass::clear()
{

}

void xe_graphics::gamma_correction_pass::render()
{
    graphics_device *device = xe_render::get_device();

    device->bind_shader(gmshd);

    if(texture != nullptr)
        device->activate_bind_texture(TEXTURE_TYPE::COLOR, texture);
    
    xe_render::draw_full_quad();

    device->unbind_texture(TEXTURE_TYPE::COLOR);
    device->unbind_shader();
}

void xe_graphics::gamma_correction_pass::update(real32 dt)
{

}

void xe_graphics::shadow_map_pass::init()
{
    // @Refactor: initialization here
    shadow = new shadow_map();

    bool32 shadow_map_inited = xe_render::create_shadow_maps(shadow);

    if (!shadow_map_inited)
        printf("Failed to create shadow map!\n");
   
    graphics_device *device = xe_render::get_device();

    shader *depth_shader = xe_render::get_shader("shadow_depth");
    shader *shadow_map_shader = xe_render::get_shader("shadow_map");

    device->bind_shader(shadow_map_shader);
    device->set_int("diff_tex", 0, shadow_map_shader);
    device->set_int("shadow_map", 1, shadow_map_shader);
}

void xe_graphics::shadow_map_pass::clear()
{

}

void xe_graphics::shadow_map_pass::unload_resources()
{

}

void xe_graphics::shadow_map_pass::render()
{
    graphics_device *device = xe_render::get_device();
    
    device->bind_framebuffer(&shadow->depth_fbo);
    device->set_viewport(0, 0, shadow->w, shadow->h);

    device->clear(GL_DEPTH_BUFFER_BIT);

    xe_render::apply_shadow_map(shadow);

    glm::mat4 light_view_matrix = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 light_space_matrix = light_view_matrix * shadow->light_projection_matrix;
  
    shader *depth_shader = xe_render::get_shader("shadow_depth");
    device->bind_shader(depth_shader);
    device->set_mat4("light_space_matrix", light_space_matrix, depth_shader);

    for (uint16 i = 0; i < current_scene->entities.size(); ++i)
    {
        entity *current_ent = current_scene->entities[i];

        xe_render::draw_ent_with_shader(current_ent, depth_shader);
    }
        
    device->unbind_framebuffer();
    
}

void xe_graphics::shadow_map_pass::update(real32 dt)
{

}

void xe_graphics::shadow_map_pass::bind_depth_texture() const
{
    graphics_device *device = xe_render::get_device();

    device->bind_texture(TEXTURE_TYPE::DEPTH, shadow->depth_fbo.depth_texture);

}

void xe_graphics::pbr_pass::init()
{
    graphics_device *device = xe_render::get_device();

    constexpr int env_map_size = 1024;
    constexpr int irradiance_map_size = 32;
    constexpr int brdf_lut_size = 256;

    glm::mat4 projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    glm::mat4 views_to_capture[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f),  glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f),  glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f),  glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f),  glm::vec3(0.0f, -1.0f,  0.0f))
    };

    device->enable(GL_DEPTH_TEST);
    device->set_depth_func(GL_LEQUAL);
    device->enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
    device->create_framebuffer(1, &fbo);
    device->create_render_buffer(1, &fbo);

    device->bind_framebuffer(&fbo);
    device->bind_renderbuffer(&fbo);
    device->set_renderbuffer(GL_DEPTH_COMPONENT24, env_map_size, env_map_size);
    device->set_framebuffer_renderbuffer_attachment(&fbo);
    
    device->create_texture(&env_cubemap);
    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &env_cubemap);

    for (uint32 i = 0; i < 6; ++i)
    {
        device->load_texture_gpu(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_map_size, env_map_size, GL_RGB16F, GL_RGB, GL_FLOAT, nullptr);
    }

    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_R, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR_MIPMAP_LINEAR);
    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);
   
    shader *equ_to_cubemap = xe_render::get_shader("equirect");

    device->bind_shader(equ_to_cubemap);
    device->set_mat4("projection", projection_matrix, equ_to_cubemap);

    texture2D *hdr_tex = xe_render::get_texture2D_resource("hdr_env");

    device->activate_bind_texture(TEXTURE_TYPE::COLOR, hdr_tex);

    device->set_viewport(0, 0, env_map_size, env_map_size);
    device->bind_framebuffer(&fbo);

    for (uint32 i = 0; i < 6; ++i)
    {
        device->set_mat4("view", views_to_capture[i], equ_to_cubemap);
        device->set_texture2D_fbo(GL_COLOR_ATTACHMENT0, TEXTURE_TYPE::CUBEMAP_POSITIVE, i, &env_cubemap);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap.id, 0);
        //device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        xe_render::draw_cube();
    }

    device->unbind_framebuffer();

    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &env_cubemap);
    device->generate_texture_mipmap(TEXTURE_TYPE::CUBEMAP);

    device->create_texture(&irr_map);
    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &irr_map);

    for (uint32 i = 0; i < 6; ++i)
    {
        device->load_texture_gpu(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map_size, irradiance_map_size, GL_RGB16F, GL_RGB, GL_FLOAT, nullptr);
        //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, irradiance_map_size, irradiance_map_size, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_R, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR);
    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

    device->bind_framebuffer(&fbo);
    device->bind_renderbuffer(&fbo);
    device->set_renderbuffer(GL_DEPTH_COMPONENT24, irradiance_map_size, irradiance_map_size);

    shader *irradiance = xe_render::get_shader("irradiance");

    device->bind_shader(irradiance);
    device->set_int("environment_map", 0, irradiance);
    device->set_mat4("projection", projection_matrix, irradiance);

    device->activate_bind_texture(TEXTURE_TYPE::COLOR, &env_cubemap);

    device->set_viewport(0, 0, irradiance_map_size, irradiance_map_size);
    device->bind_framebuffer(&fbo);

    for (uint32 i = 0; i < 6; ++i)
    {
        device->set_mat4("view", views_to_capture[i], irradiance);
        device->set_texture2D_fbo(GL_COLOR_ATTACHMENT0, TEXTURE_TYPE::CUBEMAP_POSITIVE, i, &irr_map);
        device->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        xe_render::draw_cube();
    }

    device->unbind_framebuffer();

    device->create_texture(&prefilter_map);
    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &prefilter_map);

    for (uint32 i = 0; i < 6; i++)
    {
        device->load_texture_gpu(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, brdf_lut_size, brdf_lut_size, GL_RGB16F, GL_RGB, GL_FLOAT, nullptr);
        //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, brdf_lut_size, brdf_lut_size, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_R, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR_MIPMAP_LINEAR);
    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

    device->generate_texture_mipmap(TEXTURE_TYPE::CUBEMAP);

    shader *prefilter = xe_render::get_shader("prefilter");

    device->bind_shader(prefilter);
    device->set_int("environment_map", 0, prefilter);
    device->set_mat4("projection", projection_matrix, prefilter);

    device->activate_bind_texture(TEXTURE_TYPE::CUBEMAP, &env_cubemap);
    device->bind_framebuffer(&fbo);

    uint32 max_mip_levels = 5;
    for (uint32 mip = 0; mip < max_mip_levels; ++mip)
    {
        uint32 mip_width = brdf_lut_size * std::pow(0.5, mip);
        uint32 mip_height = brdf_lut_size * std::pow(0.5, mip);
        
        device->bind_renderbuffer(&fbo);
        device->set_renderbuffer(GL_DEPTH_COMPONENT24, mip_width, mip_height);
        
        device->set_viewport(0, 0, mip_width, mip_height);
    
        real32 roughness = (real32)mip / (real32)(max_mip_levels - 1);
        device->set_float("roughness", roughness, prefilter);
        
        for (uint32 i = 0; i < 6; ++i)
        {
            device->set_mat4("view", views_to_capture[i], prefilter);
            device->set_texture2D_fbo(GL_COLOR_ATTACHMENT0, TEXTURE_TYPE::CUBEMAP_POSITIVE, i, &prefilter_map, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            xe_render::draw_cube();
        }
    }

    device->unbind_framebuffer();

    device->create_texture(&brdf_lut);
    device->bind_texture(TEXTURE_TYPE::COLOR, &brdf_lut);

    device->load_texture_gpu(TEXTURE_TYPE::COLOR, env_map_size, env_map_size, GL_RG16F, GL_RG, GL_FLOAT, nullptr);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, env_map_size, env_map_size, 0, GL_RG, GL_FLOAT, 0);

    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_S, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);
    device->set_texture_wrapping(TEXTURE_TYPE::CUBEMAP, TEXTURE_WRAPPING_AXIS::TEXTURE_AXIS_T, TEXTURE_WRAPPING::TEXTURE_ADDRESS_CLAMP);

    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MIN, TEXTURE_SAMPLING::LINEAR);
    device->set_texture_sampling(TEXTURE_TYPE::CUBEMAP, TEXTURE_FILTER_OPERATION::MAG, TEXTURE_SAMPLING::LINEAR);

    device->bind_framebuffer(&fbo);
    device->bind_renderbuffer(&fbo);
    device->set_renderbuffer(GL_DEPTH_COMPONENT24, env_map_size, env_map_size);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_lut.id, 0);

    device->set_viewport(0, 0, env_map_size, env_map_size);

    shader *brdf = xe_render::get_shader("brdf");

    device->bind_shader(brdf);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    xe_render::draw_full_quad();
    
    device->unbind_framebuffer();

    shader *pbr = xe_render::get_shader("pbr");
    shader *back = xe_render::get_shader("background");

    camera3d_component camera = xe_render::get_camera3D();

    device->bind_shader(pbr);
    device->set_mat4("projection", camera.get_projection_matrix(), pbr);

    glm::vec3 light_pos[] =
    {
        glm::vec3(-10.0f,  0.0f, 10.0f),
        glm::vec3(-10.0f,  0.0f, 10.0f),
        glm::vec3(10.0f, 0.0f, 10.0f),
        glm::vec3(10.0f,  0.0f, 10.0f),
    };

    glm::vec3 light_colors[] =
    {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };


    for (uint32 i = 0; i < sizeof(light_pos) / sizeof(light_pos[0]); ++i)
    {
        device->set_vec3("light_positions[" + std::to_string(i) + "]", light_pos[i], pbr);
        device->set_vec3("light_colors[" + std::to_string(i) + "]", light_colors[i], pbr);
    }

    device->bind_shader(back);
    device->set_mat4("projection", camera.get_projection_matrix(), back);

    viewport vp = device->get_viewport();
    
    device->set_viewport(0, 0, vp.width, vp.height);
}

void xe_graphics::pbr_pass::clear()
{
    graphics_device *device = xe_render::get_device();

    device->destroy_framebuffer(&fbo);

    device->destroy_texture2D(&env_cubemap);
    device->destroy_texture2D(&irr_map);
    device->destroy_texture2D(&brdf_lut);
}

void xe_graphics::pbr_pass::render()
{
    shader *pbr_shader = xe_render::get_shader("pbr");
    graphics_device *device = xe_render::get_device();
    camera3d_component camera3d = xe_render::get_camera3D();

    device->bind_shader(pbr_shader);

    glm::mat4 view = camera3d.get_view_matrix();

    device->set_mat4("view", view, pbr_shader);
    device->set_vec3("cam_pos", camera3d.pos, pbr_shader);
    
    // bind pre-computed IBL data
    
    device->activate_texture(0);
    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &irr_map);

    device->activate_texture(1);
    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &prefilter_map);

    device->activate_texture(2);
    device->bind_texture(TEXTURE_TYPE::COLOR, &brdf_lut);

    /*texture2D *iron_albedo = xe_render::get_texture2D_resource("albedo_iron");
    texture2D *iron_normal = xe_render::get_texture2D_resource("normal_iron");
    texture2D *iron_metallic = xe_render::get_texture2D_resource("metallic_iron");
    texture2D *iron_roughness = xe_render::get_texture2D_resource("roughness_iron");
    texture2D *iron_ao = xe_render::get_texture2D_resource("ao_iron");

    device->activate_texture(3);
    device->bind_texture(TEXTURE_TYPE::COLOR, iron_albedo);
    device->activate_texture(4);
    device->bind_texture(TEXTURE_TYPE::COLOR, iron_normal);
    device->activate_texture(5);
    device->bind_texture(TEXTURE_TYPE::COLOR, iron_metallic);
    device->activate_texture(6);
    device->bind_texture(TEXTURE_TYPE::COLOR, iron_roughness);
    device->activate_texture(7);
    device->bind_texture(TEXTURE_TYPE::COLOR, iron_ao);

    glm::mat4 model_matrix = glm::mat4(1.0);

    model_matrix = glm::translate(model_matrix, glm::vec3(-3.0, 0.0, -10.0f));
    device->set_mat4("model", model_matrix, pbr_shader);  
    xe_render::draw_sphere();

    model_matrix = glm::translate(model_matrix, glm::vec3(-8.0, 0.0, -2.0f));
    device->set_mat4("model", model_matrix, pbr_shader);
    xe_render::draw_sphere();

    model_matrix = glm::translate(model_matrix, glm::vec3(3.0, 0.0, -5.0f));
    device->set_mat4("model", model_matrix, pbr_shader);
    xe_render::draw_sphere();*/

    texture2D *albedo_cerberus = xe_render::get_texture2D_resource("cerberus_d");
    texture2D *normal_cerberus = xe_render::get_texture2D_resource("cerberus_n");
    texture2D *metallic_cerberus = xe_render::get_texture2D_resource("cerberus_m");
    texture2D *rougnhess_cerberus = xe_render::get_texture2D_resource("cerberus_r");
    texture2D *ao_cerberus = xe_render::get_texture2D_resource("cerberus_ao");

    device->activate_texture(3);
    device->bind_texture(TEXTURE_TYPE::COLOR, albedo_cerberus);
    device->activate_texture(4);
    device->bind_texture(TEXTURE_TYPE::COLOR, normal_cerberus);
    device->activate_texture(5);
    device->bind_texture(TEXTURE_TYPE::COLOR, metallic_cerberus);
    device->activate_texture(6);
    device->bind_texture(TEXTURE_TYPE::COLOR, rougnhess_cerberus);
    device->activate_texture(7);
    device->bind_texture(TEXTURE_TYPE::COLOR, ao_cerberus);

    glm::mat4 model_matrix = xe_render::IDENTITY_MATRIX;
   
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, -2.0f));
    device->set_mat4("model", model_matrix, pbr_shader);
   
    xe_assets::model *cerberus = application::get_model_by_name("Cerberus");
    xe_render::draw_model(cerberus, pbr_shader);

    shader *back = xe_render::get_shader("background");
    
    device->bind_shader(back);
    device->set_mat4("view", view, back);

    device->activate_texture(0);
    device->bind_texture(TEXTURE_TYPE::CUBEMAP, &env_cubemap);
    
    //device->bind_texture(TEXTURE_TYPE::CUBEMAP, &irr_map);
    //device->bind_texture(TEXTURE_TYPE::CUBEMAP, &prefilter_map);
    
    xe_render::draw_cube();

}

void xe_graphics::pbr_pass::update(real32 dt)
{

}
