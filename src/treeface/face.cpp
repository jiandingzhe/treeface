#include "treeface/face.h"

#include "treeface/guts/face_guts.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/materialmanager.h"

using namespace treecore;

namespace treeface {

Face::Face(uint32 width, uint32 height) noexcept
    : m_guts(new (std::nothrow) Guts())
{
    m_guts->width = width;
    m_guts->height = height;

    GeometryManager* geo_mgr = new GeometryManager;
    MaterialManager* mat_mgr = new MaterialManager;

    m_guts->scene_frust = new Scene(geo_mgr, mat_mgr);
    m_guts->scene_ortho = new Scene(geo_mgr, mat_mgr);

    m_guts->root_widget_frust = new Widget(m_guts->scene_frust->get_root_node());
    m_guts->root_widget_ortho = new Widget(m_guts->scene_ortho->get_root_node());
}

treecore::uint32 Face::get_width() const noexcept
{
    return m_guts->width;
}

void Face::set_width(treecore::uint32 value) noexcept
{
    m_guts->width = value;
    m_guts->matrix_view_dirty = true;
    m_guts->matrix_frust_dirty = true;
    m_guts->matrix_ortho_dirty = true;
    m_guts->project_dist_dirty = true;
}

treecore::uint32 Face::get_height() const noexcept
{
    return m_guts->height;
}

void Face::set_height(treecore::uint32 value) noexcept
{
    m_guts->height = value;
    m_guts->matrix_view_dirty = true;
    m_guts->matrix_frust_dirty = true;
    m_guts->matrix_ortho_dirty = true;
}

float Face::get_project_depth() const noexcept
{
    return m_guts->project_depth;
}

void Face::set_project_depth(float value) noexcept
{
    m_guts->project_depth = value;
    m_guts->matrix_ortho_dirty = true;
    m_guts->matrix_frust_dirty = true;
}

float Face::get_project_dist() noexcept
{
    if (m_guts->project_dist_dirty)
        m_guts->update_project_dist();
    return m_guts->project_dist;
}

float Face::get_frustrum_angle() const noexcept
{
    return m_guts->frustrum_angle;
}

void Face::set_frustrum_angle(float value) noexcept
{
    m_guts->frustrum_angle = value;
    m_guts->matrix_view_dirty = true;
    m_guts->matrix_frust_dirty = true;
    m_guts->matrix_ortho_dirty = true;
    m_guts->project_dist_dirty = true;
}

Widget* Face::get_root_widget_frust() noexcept
{
    return m_guts->root_widget_frust;
}

Widget* Face::get_root_widget_ortho() noexcept
{
    return m_guts->root_widget_ortho;
}

Scene* Face::get_scene_frust() noexcept
{
    return m_guts->scene_frust;
}

Scene* Face::get_scene_ortho() noexcept
{
    return m_guts->scene_ortho;
}

} // namespace treeface
