#include "TestFramework.h"

#include "treeface/misc/vertextemplate.h"
#include "treeface/gl/vertexattrib.h"

#include <treejuce/Array.h>
#include <treejuce/ArrayRef.h>
#include <treejuce/Variant.h>

using namespace treeface;
using namespace treejuce;

void TestFramework::content()
{
    VertexAttrib attr1{"position", 3, GL_FLOAT};
    VertexAttrib attr2{"color", 4, GL_UNSIGNED_BYTE};
    VertexAttrib attr3{"normal", 3, GL_FLOAT};
    VertexAttrib attr4{"tangent", 3, GL_FLOAT};

    VertexTemplate vtx_temp;
    vtx_temp.add_attrib(attr1, false);
    vtx_temp.add_attrib(attr2, false);
    vtx_temp.add_attrib(attr3, false);
    vtx_temp.add_attrib(attr4, false);

    IS(vtx_temp.vertex_size(), 40);
    IS(vtx_temp.n_elems(), 13);
    IS(vtx_temp.n_attribs(), 4);

    IS(vtx_temp.get_elem_offset(0),  0);
    IS(vtx_temp.get_elem_offset(1),  4);
    IS(vtx_temp.get_elem_offset(2),  8);
    IS(vtx_temp.get_elem_offset(3),  12);
    IS(vtx_temp.get_elem_offset(4),  13);
    IS(vtx_temp.get_elem_offset(5),  14);
    IS(vtx_temp.get_elem_offset(6),  15);
    IS(vtx_temp.get_elem_offset(7),  16);
    IS(vtx_temp.get_elem_offset(8),  20);
    IS(vtx_temp.get_elem_offset(9),  24);
    IS(vtx_temp.get_elem_offset(10), 28);
    IS(vtx_temp.get_elem_offset(11), 32);
    IS(vtx_temp.get_elem_offset(12), 36);

    IS(vtx_temp.get_elem_offset(0, 0), 0);
    IS(vtx_temp.get_elem_offset(0, 1), 4);
    IS(vtx_temp.get_elem_offset(0, 2), 8);
    IS(vtx_temp.get_elem_offset(1, 0), 12);
    IS(vtx_temp.get_elem_offset(1, 1), 13);
    IS(vtx_temp.get_elem_offset(1, 2), 14);
    IS(vtx_temp.get_elem_offset(1, 3), 15);
    IS(vtx_temp.get_elem_offset(2, 0), 16);
    IS(vtx_temp.get_elem_offset(2, 1), 20);
    IS(vtx_temp.get_elem_offset(2, 2), 24);
    IS(vtx_temp.get_elem_offset(3, 0), 28);
    IS(vtx_temp.get_elem_offset(3, 1), 32);
    IS(vtx_temp.get_elem_offset(3, 2), 36);

    IS(vtx_temp.get_elem_type(0),  GL_FLOAT);
    IS(vtx_temp.get_elem_type(1),  GL_FLOAT);
    IS(vtx_temp.get_elem_type(2),  GL_FLOAT);
    IS(vtx_temp.get_elem_type(3),  GL_UNSIGNED_BYTE);
    IS(vtx_temp.get_elem_type(4),  GL_UNSIGNED_BYTE);
    IS(vtx_temp.get_elem_type(5),  GL_UNSIGNED_BYTE);
    IS(vtx_temp.get_elem_type(6),  GL_UNSIGNED_BYTE);
    IS(vtx_temp.get_elem_type(7),  GL_FLOAT);
    IS(vtx_temp.get_elem_type(8),  GL_FLOAT);
    IS(vtx_temp.get_elem_type(9),  GL_FLOAT);
    IS(vtx_temp.get_elem_type(10), GL_FLOAT);
    IS(vtx_temp.get_elem_type(11), GL_FLOAT);
    IS(vtx_temp.get_elem_type(12), GL_FLOAT);

    GLbyte* mem = (GLbyte*) malloc(vtx_temp.vertex_size());
    vtx_temp.set_value_at(mem, 0, var(1.0));
    vtx_temp.set_value_at(mem, 1, var(2.0));
    vtx_temp.set_value_at(mem, 2, var(3.0));

    vtx_temp.set_value_at(mem, 3, var(255));
    vtx_temp.set_value_at(mem, 4, var(127));
    vtx_temp.set_value_at(mem, 5, var(63));
    vtx_temp.set_value_at(mem, 6, var(255));

    vtx_temp.set_value_at(mem, 7, var(4.0));
    vtx_temp.set_value_at(mem, 8, var(5.0));
    vtx_temp.set_value_at(mem, 9, var(6.0));

    vtx_temp.set_value_at(mem, 10, var(7.0));
    vtx_temp.set_value_at(mem, 11, var(8.0));
    vtx_temp.set_value_at(mem, 12, var(9.0));

    IS(*(float*)mem, 1.0);
    IS(*(float*)(mem+4), 2.0);
    IS(*(float*)(mem+8), 3.0);

    IS(*(GLubyte*)(mem+12), 0xff);
    IS(*(GLubyte*)(mem+13), 0x7f);
    IS(*(GLubyte*)(mem+14), 0x3f);
    IS(*(GLubyte*)(mem+15), 0xff);

    IS(*(float*)(mem+16), 4.0);
    IS(*(float*)(mem+20), 5.0);
    IS(*(float*)(mem+24), 6.0);

    IS(*(float*)(mem+28), 7.0);
    IS(*(float*)(mem+32), 8.0);
    IS(*(float*)(mem+36), 9.0);
}
