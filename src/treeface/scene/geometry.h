#ifndef TREEFACE_GEOMETRY_H
#define TREEFACE_GEOMETRY_H

#include "treeface/common.h"

#include <treecore/Array.h>
#include <treecore/IntTypes.h>
#include <treecore/RefCountObject.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treecore {
class MemoryBlock;
class Result;
class var;
} // namespace treecore

namespace treeface {

class VertexIndexBuffer;
class VertexTemplate;

class Geometry: public treecore::RefCountObject
{
public:
    /**
     * @brief create empty geometry
     */
    Geometry();

    virtual ~Geometry();

    void set_vertex_template();

    /**
     * @brief set data.
     */
    // NOTE: this is not thread-safe
    treecore::Result build(const treecore::var& geom_root_node) noexcept;

    /**
     * @brief check if data is modified and is out of sync with server side
     */
    bool is_dirty() const noexcept;

    void mark_dirty() noexcept;

    GLenum get_primitive() const noexcept;
    void set_primitive(GLenum value) noexcept;

    treeface::VertexIndexBuffer* get_buffer() noexcept;
    const VertexTemplate& get_vertex_template() const noexcept;

    /**
     * @brief send data to GL server side
     */
    void send_to_gl_server() noexcept;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_GEOMETRY_H
