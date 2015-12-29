#include "treeface/graphics/guts/utils.h"

using namespace treecore;

namespace treeface
{

double clockwise_accum(const Array<Vec2f>& vertices, IndexType i_begin, IndexType i_end) noexcept
{
    jassert(i_end - i_begin > 2);
    jassert(i_end <= vertices.size());

    double sum = 0.0;

    for (int i = i_begin + 1; i != i_end; i++)
    {
        const Vec2f& p_curr = vertices[i];
        const Vec2f& p_prev = vertices[i - 1];
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    {
        const Vec2f& p_curr = vertices[i_begin];
        const Vec2f& p_prev = vertices[i_end - 1];
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    return sum;
}

} // namespace treeface
