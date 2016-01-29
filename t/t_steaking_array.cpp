#include "TestFramework.h"

#include "treeface/misc/SteakingArray.h"
#include "treeface/math/Vec2.h"
#include "treeface/math/Vec4.h"

using namespace treeface;

struct Vertex
{
    Vec4f position;
    Vec2f tex_coord;
    float foo;
    float bar;
    float baz;
};

typedef SteakingArray<16> TestArray;

void TestFramework::content()
{
    TestArray array( sizeof(Vertex) );
    IS( array.size(),      0 );
    IS( array.m_used_byte, 0 );
    GE( array.m_data.numAllocated, array.m_used_byte );

    // add one value by ptr
    Vertex value{Vec4f( 1.0f, 3.0f, 5.0f, 7.0f ), Vec2f( 2.0f, 4.0f ), 0.2f, 0.4f, 0.6f};

    array.add_by_ptr( &value );
    IS( array.size(),      1 );
    IS( array.m_used_byte, sizeof(Vertex) );
    GE( array.m_data.numAllocated, array.m_used_byte );

    {
        Vertex* fetched_ptr = (Vertex*) array.get_by_ptr( 0 );
        IS( fetched_ptr->position.get_x(), 1.0f );
        IS( fetched_ptr->position.get_y(), 3.0f );
        IS( fetched_ptr->position.get_z(), 5.0f );
        IS( fetched_ptr->position.get_w(), 7.0f );
        IS( fetched_ptr->tex_coord.x,      2.0f );
        IS( fetched_ptr->tex_coord.y,      4.0f );
        IS( fetched_ptr->foo,              0.2f );
        IS( fetched_ptr->bar,              0.4f );
        IS( fetched_ptr->baz,              0.6f );

        Vertex& fetched   = array.get<Vertex>( 0 );
        Vertex* first_ptr = (Vertex*) array.get_first_by_ptr();
        Vertex& first     = array.get_first<Vertex>();
        Vertex* last_ptr  = (Vertex*) array.get_last_by_ptr();
        Vertex& last      = array.get_first<Vertex>();

        IS( &fetched,  fetched_ptr );
        IS( first_ptr, fetched_ptr );
        IS( &first,    fetched_ptr );
        IS( last_ptr,  fetched_ptr );
        IS( &last,     fetched_ptr );
    }

    // add another value by reference
    array.add( Vertex{Vec4f( 0.0f, 1.1f, 2.2f, 3.3f ), Vec2f( 4.4f, 5.5f ), 7.7f, 8.8f, 9.9f} );
    IS( array.size(),      2 );
    IS( array.m_used_byte, sizeof(Vertex) * 2 );
    GE( array.m_data.numAllocated, array.m_used_byte );

    {
        Vertex* ptr0 = (Vertex*) array.get_by_ptr( 0 );
        IS( ptr0->position.get_x(), 1.0f );
        IS( ptr0->position.get_y(), 3.0f );
        IS( ptr0->position.get_z(), 5.0f );
        IS( ptr0->position.get_w(), 7.0f );
        IS( ptr0->tex_coord.x,      2.0f );
        IS( ptr0->tex_coord.y,      4.0f );
        IS( ptr0->foo,              0.2f );
        IS( ptr0->bar,              0.4f );
        IS( ptr0->baz,              0.6f );

        Vertex* ptr1 = (Vertex*) array.get_by_ptr( 1 );
        IS( ptr1->position.get_x(), 0.0f );
        IS( ptr1->position.get_y(), 1.1f );
        IS( ptr1->position.get_z(), 2.2f );
        IS( ptr1->position.get_w(), 3.3f );
        IS( ptr1->tex_coord.x,      4.4f );
        IS( ptr1->tex_coord.y,      5.5f );
        IS( ptr1->foo,              7.7f );
        IS( ptr1->bar,              8.8f );
        IS( ptr1->baz,              9.9f );

        Vertex& obj0      = array.get<Vertex>( 0 );
        Vertex& obj1      = array.get<Vertex>( 1 );
        Vertex& obj_first = array.get_first<Vertex>();
        Vertex& obj_last  = array.get_last<Vertex>();
        Vertex* ptr_first = (Vertex*) array.get_first_by_ptr();
        Vertex* ptr_last  = (Vertex*) array.get_last_by_ptr();

        IS( ptr1 - ptr0, 1 );
        IS( &obj0,       ptr0 );
        IS( &obj_first,  ptr0 );
        IS( ptr_first,   ptr0 );
        IS( &obj1,       ptr1 );
        IS( &obj_last,   ptr1 );
        IS( ptr_last,    ptr1 );
    }

    // shrink
    array.resize( 1 );
    IS( array.size(),      1 );
    IS( array.m_used_byte, sizeof(Vertex) );
    GE( array.m_data.numAllocated, array.m_used_byte );

    {
        Vertex* fetched_ptr = (Vertex*) array.get_by_ptr( 0 );
        IS( fetched_ptr->position.get_x(), 1.0f );
        IS( fetched_ptr->position.get_y(), 3.0f );
        IS( fetched_ptr->position.get_z(), 5.0f );
        IS( fetched_ptr->position.get_w(), 7.0f );
        IS( fetched_ptr->tex_coord.x,      2.0f );
        IS( fetched_ptr->tex_coord.y,      4.0f );
        IS( fetched_ptr->foo,              0.2f );
        IS( fetched_ptr->bar,              0.4f );
        IS( fetched_ptr->baz,              0.6f );

        Vertex& fetched   = array.get<Vertex>( 0 );
        Vertex* first_ptr = (Vertex*) array.get_first_by_ptr();
        Vertex& first     = array.get_first<Vertex>();
        Vertex* last_ptr  = (Vertex*) array.get_last_by_ptr();
        Vertex& last      = array.get_first<Vertex>();

        IS( &fetched,  fetched_ptr );
        IS( first_ptr, fetched_ptr );
        IS( &first,    fetched_ptr );
        IS( last_ptr,  fetched_ptr );
        IS( &last,     fetched_ptr );
    }

    // expand
    array.resize( 10 );
    IS( array.size(),      10 );
    IS( array.m_used_byte, sizeof(Vertex) * 10 );
    GE( array.m_data.numAllocated, array.m_used_byte );

    {
        Vertex* ptr0 = (Vertex*) array.get_by_ptr( 0 );
        IS( ptr0->position.get_x(), 1.0f );
        IS( ptr0->position.get_y(), 3.0f );
        IS( ptr0->position.get_z(), 5.0f );
        IS( ptr0->position.get_w(), 7.0f );
        IS( ptr0->tex_coord.x,      2.0f );
        IS( ptr0->tex_coord.y,      4.0f );
        IS( ptr0->foo,              0.2f );
        IS( ptr0->bar,              0.4f );
        IS( ptr0->baz,              0.6f );

        Vertex* ptr9 = (Vertex*) array.get_by_ptr( 9 );

        Vertex& obj0      = array.get<Vertex>( 0 );
        Vertex& obj9      = array.get<Vertex>( 9 );
        Vertex& obj_first = array.get_first<Vertex>();
        Vertex& obj_last  = array.get_last<Vertex>();
        Vertex* ptr_first = (Vertex*) array.get_first_by_ptr();
        Vertex* ptr_last  = (Vertex*) array.get_last_by_ptr();

        IS( ptr9 - ptr0, 9 );
        IS( &obj0,       ptr0 );
        IS( &obj_first,  ptr0 );
        IS( ptr_first,   ptr0 );
        IS( &obj9,       ptr9 );
        IS( &obj_last,   ptr9 );
        IS( ptr_last,    ptr9 );
    }

    // value set
    array.set( 8, Vertex{Vec4f( 12.3f, -23.4f, -34.5f, 45.6f ), Vec2f( 11.1f, 22.2f ), -7.89f, -9.87f, 0.123f} );
    IS( array.size(),      10 );
    IS( array.m_used_byte, sizeof(Vertex) * 10 );
    GE( array.m_data.numAllocated, array.m_used_byte );
    {
        Vertex* ptr8 = (Vertex*) array.get_by_ptr( 8 );
        IS( ptr8->position.get_x(), 12.3f );
        IS( ptr8->position.get_y(), -23.4f );
        IS( ptr8->position.get_z(), -34.5f );
        IS( ptr8->position.get_w(), 45.6f );
        IS( ptr8->tex_coord.x,      11.1f );
        IS( ptr8->tex_coord.y,      22.2f );
        IS( ptr8->foo,              -7.89f );
        IS( ptr8->bar,              -9.87f );
        IS( ptr8->baz,              0.123f );
    }
}
