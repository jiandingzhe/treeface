#include "treeface/graphics/guts/Utils.h"

using namespace treecore;

namespace treeface
{

double clockwise_accum( const Geometry::HostVertexCache& vertices, IndexType i_begin, IndexType i_end ) noexcept
{
    jassert( i_end - i_begin > 2 );
    jassert( i_end <= vertices.size() );

    double sum = 0.0;

    for (int i = i_begin + 1; i != i_end; i++)
    {
        const Vec2f& p_curr = vertices.get<Vec2f>( i );
        const Vec2f& p_prev = vertices.get<Vec2f>( i - 1 );
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    {
        const Vec2f& p_curr = vertices.get<Vec2f>( i_begin );
        const Vec2f& p_prev = vertices.get<Vec2f>( i_end - 1 );
        sum += (p_curr.x - p_prev.x) * (p_curr.y + p_prev.y);
    }

    return sum;
}

} // namespace treeface
