#include "pch.h"

#include "core/log.h"
#include "scene/ubo.h"

namespace scene {

    UBO::UBO(GLuint binding_point, size_t buffer_size, const std::vector<GLuint>& offset,
        const std::vector<GLuint>& size, GLenum hint)
        : binding_point(binding_point), buffer_size(buffer_size), offset(offset), size(size)
    {
        // by default, we will `GL_DYNAMIC_DRAW` as the buffer usage hint, because uniform
        // buffers are expected to change every frame, we will update the data repeatedly.

        glGenBuffers(1, &id);
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        glBufferData(GL_UNIFORM_BUFFER, buffer_size, NULL, hint);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, id);
    }

    UBO::~UBO() {
        if (id > 0) {
            CORE_WARN("Deleting uniform buffer (id = {0})!", id);
        }

        glDeleteBuffers(1, &id);
    }

    bool UBO::Bind() const {
        CORE_ASERT(id > 0, "Attempting to bind an invalid uniform buffer (id <= 0)!");
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        return id > 0;
    }

    void UBO::Unbind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void UBO::SetData(GLuint uniform_index, const void* data) const {
        // we must not wrap Bind() and Unbind() inside this function because they are expensive.
        // in between the binding operations, we'd better set a large batch of data all at once.
        glBufferSubData(GL_UNIFORM_BUFFER, offset[uniform_index], size[uniform_index], data);

        // we can also set data into the buffer without binding, by using the named buffer id
        // but it's unclear as to which one is better or faster, no documentation mentions it
        #ifdef EXPERIMENTAL
            glNamedBufferSubData(id, offset[uniform_index], size[uniform_index], data);
        #endif
    }

    GLuint UBO::Count() const {
        return offset.size();
    }

    GLuint UBO::Size() const {
        return buffer_size;
    }

}
