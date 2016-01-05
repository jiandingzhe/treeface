#include "TestFramework.h"
#include "treeface/graphics/guts/utils.h"

using treeface::Vec2f;

void TestFramework::content()
{
    {
        OK( "normal crossing" );
        Vec2f p1( 0.5f, 1.5f );
        Vec2f p2( 3.5f, 1.5f );

        Vec2f p3( 2.0f, 1.0f );
        Vec2f p4( 2.0f, 2.0f );

        Vec2f cross;
        OK( cross_test_inc( p1, p2, p3, p4, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p4, p3, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p3, p4, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p4, p3, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p1, p2, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p1, p2, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p2, p1, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p2, p1, cross ) );
        IS( cross.x, 2.0f );
        IS( cross.y, 1.5f );
    }

    {
        OK( "crossing on vertex" );
        Vec2f p1( 0.5f, 1.5f );
        Vec2f p2( 3.5f, 1.5f );

        Vec2f p3( 10.0f, 20.0f );
        Vec2f p4( 3.5f, 1.5f );

        Vec2f cross;
        OK( cross_test_inc( p1, p2, p3, p4, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p4, p3, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p3, p4, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p4, p3, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p1, p2, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p1, p2, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p2, p1, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p2, p1, cross ) );
        IS( cross.x, 3.5f );
        IS( cross.y, 1.5f );
    }

    {
        OK( "line segments are overlapping" );
        Vec2f p1( 1.0f, 1.0f );
        Vec2f p2( 1.0f, 2.0f );
        Vec2f p3( 1.0f, 1.5f );
        Vec2f p4( 1.0f, 10.0f );
        Vec2f cross;

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p3, p4, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p3, p4, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p4, p3, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p4, p3, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p1, p2, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p2, p1, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p1, p2, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p2, p1, cross ) );
        IS_EPSILON( cross.x, 1.0f );
        IS_EPSILON( cross.y, 1.75f );
    }

    {
        OK( "one line segment covers another" );

        Vec2f p1( 1.0f, 2.0f );
        Vec2f p2( 2.5f, 2.0f );
        Vec2f p3( 0.7f, 2.0f );
        Vec2f p4( 4.0f, 2.0f );
        Vec2f cross;

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p3, p4, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p3, p4, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p4, p3, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p4, p3, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p1, p2, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p2, p1, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p1, p2, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p2, p1, cross ) );
        IS_EPSILON( cross.x, 1.75f );
        IS_EPSILON( cross.y, 2.0f );
    }

    {
        OK( "cross on one end point" );

        Vec2f p1( 1.0f, 2.0f );
        Vec2f p2( 3.0f, 4.0f );
        Vec2f p3( 2.0f, 3.0f );
        Vec2f p4( 5.0f, 0.0f );
        Vec2f cross;

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p3, p4, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p3, p4, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p1, p2, p4, p3, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p2, p1, p4, p3, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p1, p2, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p3, p4, p2, p1, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p1, p2, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );

        cross.set( 0.0f, 0.0f );
        OK( cross_test_inc( p4, p3, p2, p1, cross ) );
        IS_EPSILON( cross.x, 2.0f );
        IS_EPSILON( cross.y, 3.0f );
    }

    {
        OK("no crossing, not parallel");

        Vec2f p1( 1.0f, 2.0f );
        Vec2f p2( 3.0f, 4.0f );
        Vec2f p3( 4.0f, 3.0f );
        Vec2f p4( 5.0f, 0.0f );
        Vec2f cross;

        OK( !cross_test_inc( p1, p2, p3, p4, cross ) );
        OK( !cross_test_inc( p2, p1, p3, p4, cross ) );
        OK( !cross_test_inc( p1, p2, p4, p3, cross ) );
        OK( !cross_test_inc( p2, p1, p4, p3, cross ) );
        OK( !cross_test_inc( p3, p4, p1, p2, cross ) );
        OK( !cross_test_inc( p3, p4, p2, p1, cross ) );
        OK( !cross_test_inc( p4, p3, p1, p2, cross ) );
        OK( !cross_test_inc( p4, p3, p2, p1, cross ) );
    }

    {
        OK("no crossing, parallel, not in same line");

        Vec2f p1( 1.0f, 2.0f );
        Vec2f p2( 3.0f, 4.0f );
        Vec2f p3( 4.0f, 3.0f );
        Vec2f p4( 6.0f, 5.0f );
        Vec2f cross;

        OK( !cross_test_inc( p1, p2, p3, p4, cross ) );
        OK( !cross_test_inc( p2, p1, p3, p4, cross ) );
        OK( !cross_test_inc( p1, p2, p4, p3, cross ) );
        OK( !cross_test_inc( p2, p1, p4, p3, cross ) );
        OK( !cross_test_inc( p3, p4, p1, p2, cross ) );
        OK( !cross_test_inc( p3, p4, p2, p1, cross ) );
        OK( !cross_test_inc( p4, p3, p1, p2, cross ) );
        OK( !cross_test_inc( p4, p3, p2, p1, cross ) );
    }

    {
        OK("no crossing, in same line");

        Vec2f p1( 1.0f, 2.0f );
        Vec2f p2( 3.0f, 4.0f );
        Vec2f p3( 4.0f, 5.0f );
        Vec2f p4( 8.0f, 9.0f );
        Vec2f cross;

        OK( !cross_test_inc( p1, p2, p3, p4, cross ) );
        OK( !cross_test_inc( p2, p1, p3, p4, cross ) );
        OK( !cross_test_inc( p1, p2, p4, p3, cross ) );
        OK( !cross_test_inc( p2, p1, p4, p3, cross ) );
        OK( !cross_test_inc( p3, p4, p1, p2, cross ) );
        OK( !cross_test_inc( p3, p4, p2, p1, cross ) );
        OK( !cross_test_inc( p4, p3, p1, p2, cross ) );
        OK( !cross_test_inc( p4, p3, p2, p1, cross ) );
    }
}
