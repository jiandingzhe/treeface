#include "treeface/scene/SceneRenderer.h"

#include "treeface/gl/Program.h"
#include "treeface/gl/VertexArray.h"

#include "treeface/misc/UniversalValue.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/VisualObject.h"

#include "treeface/scene/guts/Geometry_guts.h"
#include "treeface/scene/guts/VisualObject_guts.h"
#include "treeface/scene/guts/Material_guts.h"
#include "treeface/scene/guts/SceneNode_guts.h"
#include "treeface/scene/guts/Scene_guts.h"
#include "treeface/scene/guts/Utils.h"

#include <treecore/HashSet.h>
#include <treecore/HashMap.h>
#include <treecore/HashMultiMap.h>
#include <treecore/Result.h>
#include <treecore/ScopedPointer.h>

using namespace treecore;

namespace treeface {

typedef HashMultiMap<VisualObject*, SceneNode*>         TransformedItems;
typedef HashMap<SceneGraphMaterial*, TransformedItems*> SceneCollection;

struct ItemCombination
{
    SceneGraphMaterial* mat;
    VisualObject*       obj;
    SceneNode* node;
};

struct ItemCombinationSorter
{
    int compareElements( ItemCombination a, ItemCombination b ) const noexcept
    {
        // opaque items should go front
        if ( a.mat->is_translucent() )
        {
            if ( !b.mat->is_translucent() )
                return 1;
        }
        else
        {
            if ( b.mat->is_translucent() )
                return -1;
        }

        if (a.mat < b.mat)
            return -1;
        else if (a.mat > b.mat)
            return 1;

        if (a.obj < b.obj)
            return -1;
        else if (a.obj > b.obj)
            return 1;

        if (a.node < b.node)
            return -1;
        else if (a.node > b.node)
            return 1;

        return 0;
    }
};

void _set_uniforms_( Program* prog, const UniformMap& uniforms )
{
    for (UniformMap::ConstIterator it_uni( uniforms ); it_uni.next(); )
    {
        int i_uni = prog->get_uniform_index( it_uni.key() );
        if (i_uni == -1) continue;

        const UniformInfo& uni_info = prog->get_uniform( i_uni );
        if ( uni_info.type != it_uni.value().get_type() )
            continue;

        prog->set_uniform( uni_info.location, it_uni.value() );
    }
}

struct SceneRenderer::Impl
{
    Array<ItemCombination> combs;
};

SceneRenderer::SceneRenderer()
    : m_impl( new Impl() )
{}

SceneRenderer::~SceneRenderer()
{
    if (m_impl)
        delete m_impl;
}

void SceneRenderer::render( const Mat4f& matrix_proj,
                            const Mat4f& matrix_view,
                            Scene* scene )
{
    traverse( scene->m_guts->root_node );

    ItemCombinationSorter sorter;
    m_impl->combs.sort( sorter );

    // upload geometry data
    {
        HashSet<Geometry*> dirty_geoms;
        for (int i = 0; i < m_impl->combs.size(); i++)
        {
            Geometry* geom = m_impl->combs[i].obj->get_geometry();
            if ( geom->is_dirty() ) dirty_geoms.insert( geom );
        }

        HashSet<Geometry*>::Iterator i_dirty( dirty_geoms );
        while ( i_dirty.next() )
        {
            i_dirty.content()->get_vertex_buffer()->bind();
            i_dirty.content()->get_index_buffer()->bind();
            i_dirty.content()->upload_data();
            i_dirty.content()->get_vertex_buffer()->unbind();
            i_dirty.content()->get_index_buffer()->unbind();
        }
    }

    // light direction in model-view coordinate
    Vec4f light_direct_in_view = matrix_view * scene->get_global_light_direction();

    // traverse scene items
    SceneGraphMaterial* prev_mat  = nullptr;
    VisualObject*       prev_item = nullptr;

    for (int i = 0; i < m_impl->combs.size(); i++)
    {
        ItemCombination curr_render = m_impl->combs[i];
        Program*        prog        = curr_render.mat->get_program();
        Geometry*       geom        = curr_render.obj->get_geometry();
        bool update_obj_uniform     = false;

        if (curr_render.mat != prev_mat)
        {
            update_obj_uniform = true;
            prev_mat = curr_render.mat;
            curr_render.mat->bind();

            prog->set_uniform( curr_render.mat->m_uni_proj,          matrix_proj );
            prog->set_uniform( curr_render.mat->m_uni_light_direct,  light_direct_in_view );
            prog->set_uniform( curr_render.mat->m_uni_light_color,   scene->get_global_light_color() );
            prog->set_uniform( curr_render.mat->m_uni_light_ambient, scene->get_global_light_ambient() );
        }

        if (curr_render.obj != prev_item)
        {
            update_obj_uniform = true;
            prev_item = curr_render.obj;
            curr_render.obj->get_vertex_array()->bind();
        }

        // TODO update geometry and visual obj's uniforms to current program
        if (update_obj_uniform)
        {
            _set_uniforms_( prog, geom->m_impl->uniforms );
            _set_uniforms_( prog, curr_render.obj->m_impl->uniforms );
        }

        // set current node's transformation and uniforms
        const Mat4f& mat_model = curr_render.node->get_global_transform();
        const Mat4f  mat_model_view      = matrix_view * mat_model;
        const Mat4f  mat_model_view_proj = matrix_proj * mat_model_view;

        prog->set_uniform( curr_render.mat->m_uni_model_view,      mat_model_view );
        prog->set_uniform( curr_render.mat->m_uni_model_view_proj, mat_model_view_proj );
        prog->set_uniform( curr_render.mat->m_uni_norm,            mat_model_view.get_normal_matrix() );

        if (curr_render.node->m_impl->uniform_cache_dirty)
            curr_render.node->m_impl->recur_update_uniform_cache_from_parent();

        _set_uniforms_( prog, curr_render.node->m_impl->cached_inherit_uniforms );
        _set_uniforms_( prog, curr_render.node->m_impl->self_uniforms );

        // do draw
        curr_render.obj->render();
    }

    VertexArray::unbind();
    prev_mat->unbind();
}

treecore::Result SceneRenderer::traverse_begin() noexcept
{
    m_impl->combs.clear();
    return Result::ok();
}

treecore::Result SceneRenderer::traverse_one_node( SceneNode* node ) noexcept
{
    jassert( node != nullptr );
    for (int i = 0; i < node->get_num_items(); i++)
    {
        VisualObject* item = dynamic_cast<VisualObject*>( node->get_item_at( i ) );
        jassert( item != nullptr );

        if (!item)
            continue;

        SceneGraphMaterial* mat = item->get_material();
        jassert( mat != nullptr );

        m_impl->combs.add( { mat, item, node } );
    }
    return Result::ok();
}

treecore::Result SceneRenderer::traverse_end() noexcept
{
    return Result::ok();
}

} // namespace treeface
