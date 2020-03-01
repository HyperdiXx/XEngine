#pragma once

#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <xenpch.h>
#include <types.h>

namespace XEngine
{
    namespace Rendering
    {
        class IndexBuffer
        {
        public:
            virtual ~IndexBuffer() = default;

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            static IndexBuffer* create(uint32* indices, uint32 size);
            static IndexBuffer* create(std::vector<uint32> indices, uint32 size);
        };
    }
}
#endif // !VERTEX_BUFFER_H
