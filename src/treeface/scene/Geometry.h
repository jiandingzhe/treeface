#ifndef TREEFACE_GEOMETRY_H
#define TREEFACE_GEOMETRY_H

#include "treeface/base/Common.h"

#include <treecore/Array.h>
#include <treecore/IntTypes.h>
#include <treecore/RefCountObject.h>

#include "treeface/base/Enums.h"
#include "treeface/gl/Enums.h"
#include "treeface/misc/SteakingArray.h"

namespace treecore {
class Identifier;
class MemoryBlock;
class Result;
class var;
} // namespace treecore

namespace treeface {

class GLBuffer;
class UniversalValue;
class VertexIndexBuffer;
class VertexTemplate;

class Geometry: public treecore::RefCountObject
{
    friend class VisualObject;
    friend class SceneRenderer;

public:
    typedef SteakingArray<16> HostVertexCache;

    class HostDrawScope
    {
    public:
        HostDrawScope( Geometry& geom ): m_geom( geom ) { m_geom.host_draw_begin(); }
        ~HostDrawScope()                            { m_geom.host_draw_end();   }
    private:
        Geometry& m_geom;
    };

    ///
    /// \brief create empty geometry
    ///
    /// \param vtx_temp    vertex template object which describes vertex format
    ///                    of this geometry.
    /// \param primitive   type of geometric primitive.
    /// \param is_dynamic  if set to true, the host-side cache will be kept
    ///                    after data upload; otherwise it will be cleared.
    ///
    Geometry( const VertexTemplate& vtx_temp, GLPrimitive primitive, bool is_dynamic = false );

    ///
    /// \brief create geometry from config data
    ///
    /// \param geom_root_node  root node of config data
    ///
    Geometry( const treecore::var& geom_root_node );

    virtual ~Geometry();

    /**
     * @brief check if data is modified and is out of sync with server side
     */
    bool is_dirty() const noexcept;
    void mark_dirty() noexcept;

    GLPrimitive get_primitive() const noexcept;
    int         get_num_index() const noexcept;

    const VertexTemplate& get_vertex_template() const noexcept;

    GLBuffer* get_vertex_buffer() noexcept;
    GLBuffer* get_index_buffer() noexcept;

    bool get_uniform_value( const treecore::Identifier& name, UniversalValue& result ) const noexcept;
    void set_uniform_value( const treecore::Identifier& name, const UniversalValue& value );
    bool has_uniform( const treecore::Identifier& name ) const noexcept;

    void host_draw_begin();

    template<typename T>
    void add_vertex( const T& vtx_data )
    {
        add_vertex_by_ptr( &vtx_data );
    }

    void add_vertex_by_ptr( const void* vtx_data );

    void add_index( IndexType idx );

    HostVertexCache& get_host_vertex_cache() noexcept;

    treecore::Array<IndexType>& get_host_index_cache() noexcept;

    void host_draw_end();
    void host_draw_end_no_change();

    ///
    /// \brief upload data to device side if data is dirty
    ///
    void upload_data();

protected:

    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace treeface

#endif // TREEFACE_GEOMETRY_H
