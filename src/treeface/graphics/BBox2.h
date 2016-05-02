#ifndef TREEFACE_BBOX2_H
#define TREEFACE_BBOX2_H

#include "treeface/math/Vec2.h"

#include <limits>

namespace treeface
{

template<typename T>
struct BBox2
{
    BBox2() = default;

    BBox2(const Vec2<T>& p1, const Vec2<T>& p2) noexcept
    {
        if (p1.x < p2.x)
        {
            x_min = p1.x;
            x_max = p2.x;
        }
        else
        {
            x_min = p2.x;
            x_max = p1.x;
        }

        if (p1.y < p2.y)
        {
            y_min = p1.y;
            y_max = p2.y;
        }
        else
        {
            y_min = p2.y;
            y_max = p1.y;
        }
    }

    T x_min = std::numeric_limits<T>::quiet_NaN();
    T x_max = std::numeric_limits<T>::quiet_NaN();
    T y_min = std::numeric_limits<T>::quiet_NaN();
    T y_max = std::numeric_limits<T>::quiet_NaN();
};

typedef BBox2<float> BBox2f;

template<typename T>
bool operator ^ (const BBox2<T>& a, const BBox2<T>& b)
{
    return !(a.x_max < b.x_min || b.x_max < a.x_min || a.y_max < b.y_min || b.y_max < a.y_min);
}

} // namespace treeface

#endif // TREEFACE_BBOX2_H
