#include "assetmanager.h"


namespace XEngine
{
    namespace Assets
    {
        std::unordered_map<uint32, Rendering::BasicMaterial*> AssetManager::materials;
        std::unordered_map<uint32, Rendering::BPMaterial*> AssetManager::bpmaterials;
        std::unordered_map<uint32, Rendering::PBRMaterial*> AssetManager::pbrmaterials;
        std::unordered_map<std::string, Model*> AssetManager::models;

        uint32 AssetManager::getNumMaterials()
        {
            return materials.size();
        }

        Rendering::BasicMaterial * AssetManager::getMaterial(uint32 id)
        {
            return nullptr;
        }

        Rendering::BPMaterial * AssetManager::getBPMaterial(uint32 id)
        {
            return nullptr;
        }

        Rendering::PBRMaterial * AssetManager::getPBRMaterial(uint32 id)
        {
            return nullptr;
        }

        Model * AssetManager::getMesh(std::string id)
        {
            return nullptr;
        }

        void AssetManager::addMaterial(uint32 id, Rendering::BasicMaterial * material)
        {
        }

        void AssetManager::addModel(std::string id, Model * mesh)
        {
        }



    }
}