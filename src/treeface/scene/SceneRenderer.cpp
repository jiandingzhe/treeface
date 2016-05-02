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

struct RenderItem
{
    SceneGraphMaterial* mat;
    VisualObject*       vis_obj;
    SceneNode* node;
};

struct ItemCombinationSorter
{
    int compareElements( RenderItem a, RenderItem b ) const noexcept
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

        if (a.vis_obj < b.vis_obj)
            return -1;
        else if (a.vis_obj > b.vis_obj)
            return 1;

        if (a.node < b.node)
            return -1;
        else if (a.node > b.node)
            return 1;

        return 0;
    }
};

struct SceneRenderer::Impl
{
    Array<RenderItem> combs;
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
            Geometry* geom = m_impl->combs[i].vis_obj->get_geometry();
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
    SceneGraphMaterial* prev_mat     = nullptr;
    VisualObject*       prev_vis_obj = nullptr;

    for (int i = 0; i < m_impl->combs.size(); i++)
    {
        RenderItem curr_render        = m_impl->combs[i];
        Program*   prog               = curr_render.mat->get_program();
        bool       upload_obj_uniform = false;

        if (curr_render.mat != prev_mat)
        {
            upload_obj_uniform = true;
            prev_mat = curr_render.mat;
            curr_render.mat->bind();

            prog->set_uniform( curr_render.mat->m_uni_proj,          matrix_proj );
            prog->set_uniform( curr_render.mat->m_uni_light_direct,  light_direct_in_view );
            prog->set_uniform( curr_render.mat->m_uni_light_color,   scene->get_global_light_color() );
            prog->set_uniform( curr_render.mat->m_uni_light_ambient, scene->get_global_light_ambient() );
        }

        if (curr_render.vis_obj != prev_vis_obj)
        {
            upload_obj_uniform = true;
            prev_vis_obj       = curr_render.vis_obj;
            curr_render.vis_obj->get_vertex_array()->bind();
        }

        // update geometry and visual obj's uniforms to current program
        if (upload_obj_uniform)
        {
            if (curr_render.vis_obj->m_impl->uniform_cache_dirty)
                curr_render.vis_obj->m_impl->update_uniform_cache();

            for (int i_uni = 0; i_uni < curr_render.vis_obj->m_impl->cached_uniforms.size(); i_uni++)
            {
                const UniformKV& kv = curr_render.vis_obj->m_impl->cached_uniforms[i_uni];
                prog->set_uniform( kv.first, kv.second );
            }
        }

        // set current node's transformation and uniforms
        const Mat4f& mat_model = curr_render.node->get_global_transform();
        const Mat4f  mat_model_view      = matrix_view * mat_model;
        const Mat4f  mat_model_view_proj = matrix_proj * mat_model_view;

        prog->set_uniform( curr_render.mat->m_uni_model_view,      mat_model_view );
        prog->set_uniform( curr_render.mat->m_uni_model_view_proj, mat_model_view_proj );
        prog->set_uniform( curr_render.mat->m_uni_norm,            mat_model_view.get_normal_matrix() );

        // do draw
        curr_render.vis_obj->render();
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
    treecore_assert( node != nullptr );
    for (int i = 0; i < node->get_num_items(); i++)
    {
        VisualObject* vis_obj = dynamic_cast<VisualObject*>( node->get_item_at( i ) );
        treecore_assert( vis_obj != nullptr );

        if (!vis_obj)
            continue;

        SceneGraphMaterial* mat = vis_obj->get_material();
        treecore_assert( mat != nullptr );

        m_impl->combs.add( { mat, vis_obj, node } );
    }
    return Result::ok();
}

treecore::Result SceneRenderer::traverse_end() noexcept
{
    return Result::ok();
}

} // namespace treeface
