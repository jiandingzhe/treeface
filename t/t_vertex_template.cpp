#include "TestFramework.h"

#include "treeface/misc/vertextemplate.h"
#include "treeface/gl/vertexattrib.h"

#include <treejuce/Array.h>
#include <treejuce/ArrayRef.h>

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
}
