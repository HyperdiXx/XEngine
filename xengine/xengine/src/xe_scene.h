#pragma once

#include "xe_ecs.h"
#include "xe_core.h"

#include <vector>
#include <map>

namespace xe_graphics
{
    class shadow_map_pass;
    class render_pass;
}

namespace application
{
    struct application_state;
}

namespace xe_scene
{   
    struct objects
    {
        std::map<const char*, xe_assets::model> models3D;
    };

    struct layer
    {
        std::string name;
        std::vector<xe_ecs::entity*> entities;
    };

    struct scene
    {
        std::string name;
        std::vector<xe_ecs::entity*> entities;
        std::vector<xe_graphics::render_pass*> passes;
        std::vector<layer*> layers;
        xe_graphics::shadow_map_pass *shadow_pass;
        xe_ecs::entity *directional_light;
    };

    struct resources
    {
        std::vector<scene*> scenes;
    };

    scene create_scene(const char *name);
   
    // @Rework init by parsing 
    void load_test_scene(scene *sc);
    void load_spheres_scene(scene *sc);
}
