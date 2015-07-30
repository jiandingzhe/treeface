#ifndef TREEFACE_FACE_GUTS_H
#define TREEFACE_FACE_GUTS_H

#include "treeface/face.h"
#include "treeface/math.h"
#include "treeface/nedbase.h"

#include "treeface/ui/widget.h"
#include "treeface/scene/scene.h"

#include <treejuce/Holder.h>

#include <cmath>

TREEFACE_NAMESPACE_BEGIN

struct Face::Guts: public NedBase
{
    treejuce::uint32 width;
    treejuce::uint32 height;

    float project_depth = 2;
    float project_dist = std::nanf("");
    float frustrum_angle = TREEFACE_PI_4;

    bool project_dist_dirty = true;
    bool matrix_view_dirty = true;
    bool matrix_frust_dirty = true;
    bool matrix_ortho_dirty = true;

    treejuce::Holder<Widget> root_widget_frust;
    treejuce::Holder<Widget> root_widget_ortho;
    treejuce::Holder<Scene> scene_frust;
    treejuce::Holder<Scene> scene_ortho;

    Mat4f matrix_view;
    Mat4f matrix_frust;
    Mat4f matrix_ortho;

    void update_project_dist() NOEXCEPT
    {
        project_dist = width/2/tan(frustrum_angle);
        project_dist_dirty = false;
    }

    void update_matrix_view() NOEXCEPT
    {
        if (project_dist_dirty)
            update_project_dist();

        matrix_view.set(1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        -width/2, -height/2, -project_dist, 1.0);
        matrix_view_dirty = false;
    }

    void update_matrix_frust() NOEXCEPT
    {
        if (project_dist_dirty)
            update_project_dist();

        if (matrix_ortho_dirty)
            update_matrix_ortho();

        matrix_frust.set_perspective(width, height, project_dist, project_depth);

        matrix_frust_dirty = false;
    }

    void update_matrix_ortho() NOEXCEPT
    {
        if (project_dist_dirty)
            update_project_dist();

        matrix_ortho.set(2/width, 0, 0, 0,
                         0, 2/height, 0, 0,
                         0, 0, -2/project_depth, 0,
                         0, 0, 0, 1);
        matrix_ortho_dirty = false;
    }
};

TREEFACE_NAMESPACE_END

#endif
