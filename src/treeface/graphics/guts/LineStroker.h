#ifndef TREEFACE_LINE_STROKER_H
#define TREEFACE_LINE_STROKER_H

#include "treeface/gl/type.h"
#include "treeface/graphics/shapegenerator.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/constants.h"
#include "treeface/math/mat2.h"
#include "treeface/math/vec2.h"

#include <treecore/Array.h>

namespace treeface
{

struct InternalStrokeStyle
{
    LineCap cap;
    LineJoin join;
    float miter_cutoff_cosine;
    float half_width;
};

struct LineStroker
{
    typedef treecore::int16 JointID;
    void extend_stroke(const Vec2f& p_prev, const Vec2f& p1, const Vec2f& p2, bool use_line_join);

    void triangulate(treecore::Array<ShapeGenerator::StrokeVertex>& result_vertices, treecore::Array<IndexType>& result_indices);

    struct HalfOutline
    {
        void add(const Vec2f& vtx, JointID id)
        {
            jassert(outline.size() == joint_ids.size());
            jassert(outline.size() == outline_bounds.size() + 1);
            if (outline.size())
                outline_bounds.add({outline.getLast(), vtx});
            outline.add(vtx);
            joint_ids.add(id);
        }

        void resize(int size)
        {
            jassert(size > 0);
            outline.resize(size);
            outline_bounds.resize(size - 1);
            joint_ids.resize(size);
        }

        void salvage(const Vec2f& p1, const Vec2f& r_prev, JointID id)
        {
            if (sunken)
            {
                sunken = false;
                add(p1 + r_prev * side, id);
            }
        }

        void add_miter_point(const Vec2f& skeleton1,
                             JointID id,
                             const Vec2f& ortho_prev,
                             const Vec2f& ortho_curr,
                             const InternalStrokeStyle& style);

        void add_round_points(const Vec2f& skeleton1,
                              JointID id,
                              const Vec2f& ortho_prev,
                              const Vec2f& ortho_curr,
                              const InternalStrokeStyle& style);

        int find_cross_from_tail(const Vec2f& p1, const Vec2f& p2, Vec2f& result) const;

        void process_inner(const HalfOutline& outer_peer,
                           const Vec2f& skeleton1,
                           JointID id1,
                           const Vec2f& skeleton2,
                           const Vec2f& ortho_curr,
                           const InternalStrokeStyle& style);

        treecore::Array<Vec2f> outline;
        treecore::Array<BBox2f> outline_bounds;
        treecore::Array<JointID> joint_ids;
        float side; // 1 for left, -1 for right
        bool sunken;
    };

    HalfOutline part_left;
    HalfOutline part_right;

    InternalStrokeStyle style;
};

} // namespace treeface

#endif // TREEFACE_LINE_STROKER_H
