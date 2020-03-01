#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "xe_graphics_resource.h"
#include "xe_render.h"

#include <glm/glm.hpp>
#include <unordered_map>

#include <types.h>

namespace xe_assets
{
    using namespace xe_graphics;

    struct mesh
    {
        void add_vertex(static_vertex vertex);
        void add_index(uint32 ind);        
        
        std::vector<static_vertex> vertices;
        std::vector<uint32> indices;
        std::vector<texture_wrapper> mesh_textures;

        //xe_graphics::vertex_array vao;
        //xe_graphics::vertex_buffer vbo;
        //xe_graphics::index_buffer ibo;
        uint32 vao, vbo, ibo;
    };

    struct node
    {
        void add_child(node* child);
        void add_mesh(mesh *msh);

        std::string name;
        std::vector<mesh*> meshes;
        std::vector<node*> children;
    };

    struct bone
    {
        std::string name;
        glm::mat4 transform;
        glm::mat4 offset;
        bone *parent;
    };

    struct model
    {
        node* root;
        std::string parent_dir;
        std::vector<texture_wrapper> model_textures;
        std::vector<node*> nodes;
    };

    struct anim_node
    {
        std::string name;
        std::vector<anim_node*> nodes;
    };

    struct anim_mesh
    {

    };

    struct anim_model
    {

    };

    glm::mat4 transposeAiMat4(aiMatrix4x4 ai_mat);
 
    std::vector<texture_wrapper> load_textures_from_material(model *mdl, aiMaterial *material, aiTextureType type, std::string texture_type);
    
    anim_node *find_node(anim_node *node, std::string &name);
  
    void parse_bones(anim_model *animmodel, anim_mesh *mesh, aiMesh *ai_mesh);
    void parse_animations(anim_model *model, const aiScene *ai_scene);
    void parse_materials(model *m, mesh *mesh, aiMesh* ai_mesh, const aiScene *scene);
    void parse_vert(mesh* meh, aiMesh *aimesh);
    void parse_faces(mesh* mesh, aiMesh *aimesh);
    mesh* parse_mesh(model *model, aiMesh* ai_mesh, const aiScene *scene);
    node* parse_node(model* model, aiNode* ai_node, const aiScene *scene);

    void mem_cpyvec(aiVector3D & aivec3, glm::vec3 &vec3);
    void mem_cpymatrix(aiMatrix4x4 & aimat, glm::mat4 &mat4);

    void calc_weight(uint32 id, real32 weight, glm::ivec4& bone_id, glm::vec4& wts);

    bool32 create_mesh(mesh *meh);
   
    model *parse_static_model(const aiScene* scene, const std::string &path);

    anim_model *parse_anim_model(const aiScene* scene);

    model* load_model_from_file(const std::string &path);
    anim_model* load_anim_model_from_file(const std::string &path);
}