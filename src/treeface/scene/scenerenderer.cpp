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

#include <treecore/HashMap.h>
#include <treecore/HashMultiMap.h>
#include <treecore/Result.h>
#include <treecore/ScopedPointer.h>

using namespace treecore;

namespace treeface {

typedef HashMultiMap<VisualObject*, SceneNode*> TransformedItems;
typedef HashMap<SceneGraphMaterial*, TransformedItems*> SceneCollection;

struct ItemCombination
{
    SceneGraphMaterial* mat;
    VisualObject* item;
    SceneNode* trans;
};

struct ItemCombinationSorter
{
    int compareElements(ItemCombination a, ItemCombination b) const NOEXCEPT
    {
        // opaque items should go front
        if (a.mat->is_translucent())
        {
            if (!b.mat->is_translucent())
                return 1;
        }
        else
        {
            if (b.mat->is_translucent())
                return -1;
        }

        if (a.mat < b.mat)
            return -1;
        else if (a.mat > b.mat)
            return 1;

        if (a.item < b.item)
            return -1;
        else if (a.item > b.item)
            return 1;

        if (a.trans < b.trans)
            return -1;
        else if (a.trans > b.trans)
            return 1;

        return 0;
    }
};

struct SceneRenderer::Impl
{
    Array<ItemCombination> combs;
};

SceneRenderer::SceneRenderer()
    : m_impl(new Impl())
{
}

SceneRenderer::~SceneRenderer()
{
    if (m_impl)
        delete m_impl;
}

void SceneRenderer::render(const Mat4f& matrix_proj,
                           const Mat4f& matrix_view,
                           Scene* scene)
{
    traverse(scene->m_guts->root_node);

    ItemCombinationSorter sorter;
    m_impl->combs.sort(sorter);

    // light direction in model-view coordinate
    Vec4f light_direct_in_view = matrix_view * scene->get_global_light_direction();

    // traverse scene items
    SceneGraphMaterial* prev_mat = nullptr;
    VisualObject* prev_item = nullptr;

    for (int i = 0; i < m_impl->combs.size(); i++)
    {
        ItemCombination comb = m_impl->combs[i];
        Program* prog = comb.mat->get_program();
        Geometry* geom = comb.item->get_geometry();

        if (comb.mat != prev_mat)
        {
            prev_mat = comb.mat;
            comb.mat->use();

            prog->instant_set_uniform(comb.mat->m_uni_proj,          matrix_proj);
            prog->instant_set_uniform(comb.mat->m_uni_light_direct,  light_direct_in_view);
            prog->instant_set_uniform(comb.mat->m_uni_light_color,   scene->get_global_light_color());
            prog->instant_set_uniform(comb.mat->m_uni_light_ambient, scene->get_global_light_ambient());
        }

        if (comb.item != prev_item)
        {
            prev_item = comb.item;

            comb.item->get_vertex_array()->use();
            geom->get_buffer()->use();
        }

        const Mat4f& mat_model = comb.trans->get_global_transform();
        const Mat4f mat_model_view = matrix_view * mat_model;
        const Mat4f mat_model_view_proj = matrix_proj * mat_model_view;

        prog->instant_set_uniform(comb.mat->m_uni_model_view, mat_model_view);
        prog->instant_set_uniform(comb.mat->m_uni_model_view_proj, mat_model_view_proj);
        prog->instant_set_uniform(comb.mat->m_uni_norm, mat_model_view.get_normal_matrix());

        // do draw
        geom->get_buffer()->draw(geom->get_primitive());
    }

    VertexArray::unuse();
    VertexIndexBuffer::unuse();
    Program::unuse();
}

treecore::Result SceneRenderer::traverse_begin() NOEXCEPT
{
    m_impl->combs.clear();
    return Result::ok();
}

treecore::Result SceneRenderer::traverse_one_node(SceneNode* node) NOEXCEPT
{
    jassert(node != nullptr);
    for (int i = 0; i < node->get_num_items(); i++)
    {
        VisualObject* item = dynamic_cast<VisualObject*>(node->get_item_at(i));
        jassert(item != nullptr);

        if (!item)
            continue;

        SceneGraphMaterial* mat = item->get_material();
        jassert(mat != nullptr);

        m_impl->combs.add({mat, item, node});
    }
    return Result::ok();
}

treecore::Result SceneRenderer::traverse_end() NOEXCEPT
{
    return Result::ok();
}

} // namespace treeface
