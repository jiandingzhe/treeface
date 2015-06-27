#include "treeface/scene/scenerenderer.h"

#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/program.h"
#include "treeface/gl/vertexarray.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/visualobject.h"

#include "treeface/guts/material_guts.h"
#include "treeface/guts/scene_guts.h"

#include <treejuce/HashMap.h>
#include <treejuce/HashMultiMap.h>
#include <treejuce/Result.h>
#include <treejuce/ScopedPointer.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

typedef HashMultiMap<VisualObject*, SceneNode*> TransformedItems;
typedef HashMap<SceneGraphMaterial*, TransformedItems*> SceneCollection;

struct SceneRenderer::Impl
{
    SceneCollection obj_by_material_transform;
};

SceneRenderer::SceneRenderer()
    : m_impl(new Impl())
{
}

SceneRenderer::~SceneRenderer()
{
    if (m_impl)
    {
        SceneCollection::Iterator it(m_impl->obj_by_material_transform);
        while (it.next())
        {
            delete it.getValue();
        }

        delete m_impl;
    }
}

void SceneRenderer::render(const Mat4f& matrix_proj,
                           const Mat4f& matrix_view,
                           Scene* scene)
{
    traverse(scene->m_guts->root_node);

    SceneCollection::Iterator it_scene(m_impl->obj_by_material_transform);

    Vec4f light_direct_in_view = matrix_view * scene->get_global_light_direction();

    while (it_scene.next())
    {
        SceneGraphMaterial* mat = it_scene.getKey();
        mat->use();

        // set common uniforms defined by treeface
        Program* prog = mat->get_program();

        prog->instant_set_uniform(mat->m_uni_proj, matrix_proj);

        // set light uniforms
        prog->instant_set_uniform(mat->m_uni_light_direct, light_direct_in_view);
        prog->instant_set_uniform(mat->m_uni_light_color, scene->get_global_light_color());
        prog->instant_set_uniform(mat->m_uni_light_ambient, scene->get_global_light_ambient());
        // TODO set light intensities

        // traverse items and transforms
        TransformedItems::Iterator it_items(*it_scene.getValue());

        while (it_items.next())
        {
            VisualObject* item = it_items.getKey();
            Geometry* geom = item->get_geometry();

            item->get_vertex_array()->use();
            geom->get_buffer()->use();

            // traverse transform
            ArrayRef<SceneNode*> item_nodes = it_items.getValues();

            for (int i = 0; i < item_nodes.size(); i++)
            {
                // set transform uniform
                const Mat4f& mat_model = item_nodes[i]->get_global_transform();
                const Mat4f mat_model_view = matrix_view * mat_model;
                const Mat4f mat_model_view_proj = matrix_proj * mat_model_view;

                prog->instant_set_uniform(mat->m_uni_model_view, mat_model_view);
                prog->instant_set_uniform(mat->m_uni_model_view_proj, mat_model_view_proj);
                prog->instant_set_uniform(mat->m_uni_norm, mat_model_view.get_normal_matrix());


                // do draw
                geom->get_buffer()->draw(geom->get_primitive());
            }

            VertexArray::unuse();
            VertexIndexBuffer::unuse();
        }

        Program::unuse();
    }
}

treejuce::Result SceneRenderer::traverse_begin() NOEXCEPT
{
    m_impl->obj_by_material_transform.clear();
    return Result::ok();
}

treejuce::Result SceneRenderer::traverse_one_node(SceneNode* node) NOEXCEPT
{
    for (int i = 0; i < node->get_num_items(); i++)
    {
        VisualObject* item = dynamic_cast<VisualObject*>(node->get_item_at(i));

        if (!item)
            continue;

        SceneGraphMaterial* mat = item->get_material();

        if (!m_impl->obj_by_material_transform.contains(mat))
            m_impl->obj_by_material_transform.set(mat, new TransformedItems());

        m_impl->obj_by_material_transform[mat]->insert(item, node);
    }
    return Result::ok();
}

treejuce::Result SceneRenderer::traverse_end() NOEXCEPT
{
    return Result::ok();
}

TREEFACE_NAMESPACE_END
