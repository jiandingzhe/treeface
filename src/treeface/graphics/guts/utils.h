#ifndef TREEFACE_GRAPHICS_GUTS_UTILS_H
#define TREEFACE_GRAPHICS_GUTS_UTILS_H

#include "treeface/enums.h"
#include "treeface/gl/type.h"
#include "treeface/math/vec2.h"

#include <treecore/Array.h>

#define TAIL_FIND_LIMIT 5

namespace treeface
{

typedef treecore::int16 JointID;

struct InternalStrokeStyle
{
    LineCap cap;
    LineJoin join;
    float miter_cutoff_cosine;
    float half_width;
};

double clockwise_accum(const treecore::Array<Vec2f>& vertices, IndexType i_begin, IndexType i_end) noexcept;

inline float calc_step(float total, float step_size, int min_num_step)
{
    int num_step_use = int(total / step_size);
    if (num_step_use < min_num_step) num_step_use = min_num_step;
    return total / num_step_use;
}

inline bool cross_test_exc(const Vec2f& p1, const Vec2f& p2, const Vec2f& v12,
                           const Vec2f& p3, const Vec2f& p4, const Vec2f& v34,
                           Vec2f& result)
{
    float denom = v12 ^ v34;
    if (denom == 0.0f) return false;

    float cross12 = p1 ^ p2;
    float cross34 = p3 ^ p4;

    result.set((v12.x * cross34 - v34.x * cross12) / denom,
               (v12.y * cross34 - v34.y * cross12) / denom);

    Vec2f v1c = result - p1;
    Vec2f v3c = result - p3;

    if (v1c * v12 > 0.0f      &&
        v1c.length2() < v12.length2() &&
        v3c * v34 > 0.0f      &&
        v3c.length2() < v34.length2())
    {
        return true;
    }
}

} // namespace treeface

#endif // TREEFACE_GRAPHICS_GUTS_UTILS_H
