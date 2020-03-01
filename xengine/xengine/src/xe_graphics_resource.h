#pragma once

#ifndef XENGINE_GRAPHICS_RESOURCES_H
#define XENGINE_GRAPHICS_RESOURCES_H

#include <vector>

#include "xe_graphics_res_desc.h"

#include <glm/glm.hpp>

namespace xe_graphics
{
    class graphics_device;

    struct texture2D
    {
        uint32 id;
        texture_desc desc;
    };

    struct shader
    {
        uint32 id;
    };
   
    struct index_buffer
    {
        uint32 id;
        uint32 *data;
    };

    struct framebuffer
    {
        uint32 id;
    };

    enum ElementType
    {
        None,
        Bool,
        Int,
        Int2,
        Int3,
        Int4,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3x3,
        Mat4x4
    };

    struct buffer_element
    {
        std::string name; // own string impl 
        ElementType type;
        uint32 size;
        uint32 offset = 0;
        
        buffer_element(const std::string& name , ElementType type)
        {
            this->name = name;
            this->type = type;
            this->size = type_sizeof(type);
        }

        uint32 type_sizeof(ElementType type)
        {
            switch (type)
            {
            case ElementType::None:
                return 0;
            case ElementType::Bool:
                return 1;
            case ElementType::Int:
                return 4;
            case ElementType::Int2:
                return 4 * 2;
            case ElementType::Int3:
                return 4 * 3;
            case ElementType::Int4:
                return 4 * 4;
            case ElementType::Float:
                return 4;
            case ElementType::Float2:
                return 4 * 2;
            case ElementType::Float3:
                return 4 * 3;
            case ElementType::Float4:
                return 4 * 4;
            case ElementType::Mat3x3:
                return 4 * 9;
            case ElementType::Mat4x4:
                return 4 * 16;
            }

            return 0;
        }

        uint32 element_type_count()
        {
            switch (type)
            {
            case ElementType::None:
                return 0;
            case ElementType::Bool:
            case ElementType::Int:
            case ElementType::Float:
                return 1;
            case ElementType::Int2:
            case ElementType::Float2:
                return 2;
            case ElementType::Int3:
            case ElementType::Float3:
                return 3;
            case ElementType::Int4:
            case ElementType::Float4:
                return 4;
            case ElementType::Mat3x3:
                return 9;
            case ElementType::Mat4x4:
                return 16;
            }

            // Log function !!!

            return 0;
        }
    };

    struct buffer_layout
    {
        std::vector<buffer_element> elements;
        uint32 stride = 0;
    };

    struct vertex_buffer
    {
        uint32 id;
        uint32 element_count;
        buffer_layout layout;
        real32 *data;
    };
   
    struct texture_wrapper
    {
        texture2D *texture = nullptr;
        std::string path;
        std::string type;
    };

    struct vertex_array
    {
        uint32 id;
        uint32 ibuffer_index = 0;
        std::vector<vertex_buffer*> buffers;
        index_buffer *ib;
    };

    struct quad
    {
        xe_graphics::vertex_buffer *vertex_buffer;
        xe_graphics::index_buffer *index_buffer;
        xe_graphics::vertex_array *vertex_array;
    };
}
#endif