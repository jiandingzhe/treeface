#include "TestFramework.h"
#include "treeface/math/vec2.h"

typedef treeface::Vec2f v2f;

void TestFramework::content()
{
    {
        v2f vec;
        IS( vec.x, 0.0f );
        IS( vec.y, 0.0f );

        vec.set( 123.4f, 345.2f );
        IS( vec.x, 123.4f );
        IS( vec.y, 345.2f );
    }

    {
        v2f vec( 12.34f, 65.21f );
        IS( vec.x, 12.34f );
        IS( vec.y, 65.21f );
    }

    // unary arithmetic operators
    {
        v2f vec( 123.4f, -234.5f );
        v2f vec_neg = -vec;
        IS_EPSILON( vec_neg.x, -123.4f );
        IS_EPSILON( vec_neg.y, 234.5f );
    }

    {
        v2f vec( 84.13f, 33.16f );
        vec += v2f( 12.34f, 55.33f );
        IS_EPSILON( vec.x, 96.47f );
        IS_EPSILON( vec.y, 88.49f );
    }

    {
        v2f vec( 84.13f, 33.16f );
        vec -= v2f( 12.34f, 55.33f );
        IS_EPSILON( vec.x, 71.79f );
        IS_EPSILON( vec.y, -22.17f );
    }

    {
        v2f vec( 84.13f, 33.16f );
        vec *= 88.3f;
        IS_EPSILON( vec.x, 7428.679f );
        IS_EPSILON( vec.y, 2928.028f );
    }

    {
        v2f vec( 84.13f, 33.16f );
        vec /= 88.3f;
        IS_EPSILON( vec.x, 0.9527746f );
        IS_EPSILON( vec.y, 0.3755379f );
    }

    // binary arithmetic operators
    {
        v2f vec = v2f( 84.13f, 33.16f ) + v2f( 12.34f, 55.33f );
        IS_EPSILON( vec.x, 96.47f );
        IS_EPSILON( vec.y, 88.49f );
    }

    {
        v2f vec = v2f( 84.13f, 33.16f ) - v2f( 12.34f, 55.33f );
        IS_EPSILON( vec.x, 71.79f );
        IS_EPSILON( vec.y, -22.17f );
    }

    {
        v2f vec = v2f( 84.13f, 33.16f ) * 88.3f;
        IS_EPSILON( vec.x, 7428.679f );
        IS_EPSILON( vec.y, 2928.028f );
    }

    {
        v2f vec = 123.456f * v2f( 33.04f, 11.22f );
        IS_EPSILON( vec.x, 4078.986f );
        IS_EPSILON( vec.y, 1385.176f );
    }

    {
        v2f vec = v2f( 84.13f, 33.16f ) / 88.3f;
        IS_EPSILON( vec.x, 0.9527746f );
        IS_EPSILON( vec.y, 0.3755379f );
    }

    // dot multiply
    {
        float dot_result = v2f( 12.34f, 36.21f ) * v2f( 56.78f, 98.67f );
        IS_EPSILON( dot_result, 4273.506f );

        dot_result = v2f( 1.0f, 0.0f ) * v2f( 0.0f, 1.0f );
        IS_EPSILON( dot_result, 0.0f );
    }

    // cross multiply
    {
        float cross_result = v2f( 12.34f, 36.21f ) % v2f( 56.78f, 98.67f );
        IS_EPSILON( cross_result, -838.416f );

        cross_result = v2f( 1.0f, 0.0f ) % v2f( 0.0f, 1.0f );
        IS_EPSILON( cross_result, 1.0f );

        cross_result = v2f( 0.0f, 1.0f ) % v2f( -1.0f, 0.0f );
        IS_EPSILON( cross_result, 1.0f );
    }
}

