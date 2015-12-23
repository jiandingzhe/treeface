#ifndef TREEFACE_GRAPHICS_UTILS_H
#define TREEFACE_GRAPHICS_UTILS_H

#include "treeface/math/vec2.h"

namespace treeface
{

inline bool vec_are_cclw(const Vec2f& v1, const Vec2f& v2, const Vec2f& v3)
{
    int num_neg = 0;
    if ((v1 ^ v2) < 0.0f) num_neg++;
    if ((v2 ^ v3) < 0.0f) num_neg++;
    if ((v3 ^ v1) < 0.0f) num_neg++;
    return num_neg < 2;
}

inline bool is_below(const Vec2f& a, const Vec2f& b)
{
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

inline bool is_convex(const Vec2f& edge1, const Vec2f& edge2) noexcept
{
    float cross = edge1 ^ edge2;
    return cross > 0;
}

inline bool is_convex(const Vec2f& vtx1, const Vec2f& vtx2, const Vec2f& vtx3) noexcept
{
    return is_convex(vtx2 - vtx1, vtx3 - vtx2);
}

} // namespace treeface

#endif // TREEFACE_GRAPHICS_UTILS_H
