#ifndef TREEFACE_GEOMETRY_H
#define TREEFACE_GEOMETRY_H

#include "treeface/common.h"

#include <treejuce/Array.h>
#include <treejuce/IntTypes.h>
#include <treejuce/Object.h>

#include <GL/glew.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class MemoryBlock;
class Result;
class var;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class VertexIndexBuffer;
class VertexTemplate;

class Geometry: public treejuce::Object
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
    treejuce::Result build(const treejuce::var& geom_root_node) NOEXCEPT;

    /**
     * @brief check if data is modified and is out of sync with server side
     */
    bool is_dirty() const NOEXCEPT;

    void mark_dirty() NOEXCEPT;

    GLenum get_primitive() const NOEXCEPT;
    void set_primitive(GLenum value) NOEXCEPT;

    treeface::VertexIndexBuffer* get_buffer() NOEXCEPT;
    const VertexTemplate& get_vertex_template() const NOEXCEPT;

    /**
     * @brief send data to GL server side
     */
    void send_to_gl_server() NOEXCEPT;

protected:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GEOMETRY_H
