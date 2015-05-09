#include "treeface/scene/scenerenderer.h"

#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/program.h"
#include "treeface/gl/vertexarray.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/node.h"
#include "treeface/scene/visualitem.h"

#include "treeface/private/material_private.h"

#include <treejuce/HashMap.h>
#include <treejuce/HashMultiMap.h>
#include <treejuce/Result.h>
#include <treejuce/ScopedPointer.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

typedef HashMultiMap<VisualItem*, Node*> TransformedItems;
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

void SceneRenderer::render(const treeface::Mat4f& matrix_proj, Node* root_node)
{
    traverse(root_node);


    SceneCollection::Iterator it_scene(m_impl->obj_by_material_transform);

    while (it_scene.next())
    {
        SceneGraphMaterial* mat = it_scene.getKey();
        mat->use();

        // set common uniforms defined by treeface
        Program* prog = mat->get_program();

        int uni_proj  = mat->m_uni_proj_trans;
        int uni_trans = mat->m_uni_trans;
        int uni_norm  = mat->m_uni_norm_trans;

        if (uni_proj >= 0)
            prog->instant_set_uniform(uni_proj, matrix_proj);

        // TODO set light uniforms

        // traverse items and transforms
        TransformedItems::Iterator it_items(*it_scene.getValue());

        while (it_items.next())
        {
            VisualItem* item = it_items.getKey();
            Geometry* geom = item->get_geometry();

            item->get_vertex_array()->use();
            geom->get_buffer()->use();

            // traverse transform
            ArrayRef<Node*> item_nodes = it_items.getValues();

            for (int i = 0; i < item_nodes.size(); i++)
            {
                if (uni_trans >= 0)
                    prog->instant_set_uniform(uni_trans, item_nodes[i]->get_global_transform());
                if (uni_norm >= 0)
                    prog->instant_set_uniform(uni_norm, item_nodes[i]->get_global_normal_transform());

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

treejuce::Result SceneRenderer::traverse_one_node(Node* node) NOEXCEPT
{
    for (int i = 0; i < node->get_num_visual_items(); i++)
    {
        VisualItem* item = node->get_visual_item_at(i);
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
