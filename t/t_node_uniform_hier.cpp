#include "TestFramework.h"

#include <treecore/RefCountHolder.h>
#include <treecore/Identifier.h>
#include "treeface/scene/SceneNode.h"
#include "treeface/misc/UniversalValue.h"

using namespace treecore;
using namespace treeface;

Identifier foo( "foo" );
Identifier bar( "bar" );
Identifier baz( "baz" );

void TestFramework::content()
{
    RefCountHolder<SceneNode> node0 = new SceneNode();
    node0->set_uniform_value( foo, int(123) );
    node0->set_uniform_value( bar, float(456) );
    OK( node0->has_self_uniform( foo ) );
    OK( node0->has_uniform( foo ) );
    OK( node0->has_self_uniform( bar ) );
    OK( node0->has_uniform( bar ) );
    OK( !node0->has_self_uniform( baz ) );
    OK( !node0->has_uniform( baz ) );

    OK( "fetch uniform store" );
    {
        HashMap<Identifier, UniversalValue> uni_store;
        IS( node0->collect_uniforms( uni_store ), 2 );
        IS( uni_store.size(),                     2 );
        OK( uni_store.contains( foo ) );
        OK( uni_store.contains( bar ) );
        IS( uni_store[foo].get_type(), TFGL_TYPE_INT );
        IS( int32( uni_store[foo] ),   123 );
        IS( uni_store[bar].get_type(), TFGL_TYPE_FLOAT );
        IS( float(uni_store[bar]),     456.0f );

        uni_store.clear();
        uni_store[foo] = UniversalValue( uint16( 12345 ) );

        IS( node0->collect_uniforms( uni_store ), 1 );
        IS( uni_store.size(),                     2 );
        OK( uni_store.contains( foo ) );
        OK( uni_store.contains( bar ) );
        IS( uni_store[foo].get_type(), TFGL_TYPE_UNSIGNED_SHORT );
        IS( uint16( uni_store[foo] ),  12345 );
        IS( uni_store[bar].get_type(), TFGL_TYPE_FLOAT );
        IS( float(uni_store[bar]),     456.0f );
    }

    OK( "add node 1 to node 0" );
    RefCountHolder<SceneNode> node1 = new SceneNode();
    node1->set_uniform_value( foo, Vec2f( 1.2f, 3.4f ) );
    node0->add_child( node1 );

    OK( node1->has_self_uniform( foo ) );
    OK( !node1->has_self_uniform( bar ) );
    OK( node1->has_uniform( foo ) );
    OK( node1->has_uniform( bar ) );

    {
        UniversalValue node1_foo( int(0) );
        OK( node1->get_uniform_value( foo, node1_foo ) );
        IS( node1_foo.get_type(), TFGL_TYPE_VEC2F );
        IS( Vec2f( node1_foo ).x, 1.2f );
        IS( Vec2f( node1_foo ).y, 3.4f );
    }

    {
        UniversalValue node1_bar( int(0) );
        OK( node1->get_uniform_value( bar, node1_bar ) );
        IS( node1_bar.get_type(), TFGL_TYPE_FLOAT );
        IS( float(node1_bar),     456.0f );
    }

    OK( "add node 2 to node 1" );
    RefCountHolder<SceneNode> node2 = new SceneNode();
    node1->add_child( node2 );
    node2->set_uniform_value( baz, uint8( 48 ) );

    OK( node2->has_uniform( foo ) );
    OK( node2->has_uniform( bar ) );
    OK( node2->has_uniform( baz ) );
    OK( !node2->has_self_uniform( foo ) );
    OK( !node2->has_self_uniform( bar ) );
    OK( node2->has_self_uniform( baz ) );

    {
        UniversalValue node2_foo( int(0) );
        OK( node2->get_uniform_value( foo, node2_foo ) );
        IS( node2_foo.get_type(), TFGL_TYPE_VEC2F );
        IS( Vec2f( node2_foo ).x, 1.2f );
        IS( Vec2f( node2_foo ).y, 3.4f );
    }

    {
        UniversalValue node2_bar( int(0) );
        OK( node2->get_uniform_value( bar, node2_bar ) );
        IS( node2_bar.get_type(), TFGL_TYPE_FLOAT );
        IS( float(node2_bar),     456.0f );
    }

    {
        UniversalValue node2_baz( int(0) );
        OK( node2->get_uniform_value( baz, node2_baz ) );
        IS( node2_baz.get_type(), TFGL_TYPE_UNSIGNED_BYTE );
        IS( uint8( node2_baz ),   48 );
    }

    OK( "fetch uniform store" );
    {
        HashMap<Identifier, UniversalValue> uni_store;
        IS( node1->collect_uniforms( uni_store ), 2 );
        OK( uni_store.contains( foo ) );
        OK( uni_store.contains( bar ) );
        IS( uni_store[foo].get_type(), TFGL_TYPE_VEC2F );
        IS( Vec2f( uni_store[foo] ).x, 1.2f );
        IS( Vec2f( uni_store[foo] ).y, 3.4f );
        IS( uni_store[bar].get_type(), TFGL_TYPE_FLOAT );
        IS( float(uni_store[bar]),     456.0f );
    }

    OK( "modify node hierarchy structure" );
    OK( node0->remove_child( node1 ) );

    OK( node1->has_uniform( foo ) );
    OK( !node1->has_uniform( bar ) );
    OK( !node1->has_uniform( baz ) );
    OK( node2->has_uniform( foo ) );
    OK( !node2->has_uniform( bar ) );
    OK( node2->has_uniform( baz ) );

}
