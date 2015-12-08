#include "treeface/graphics/shapegenerator.h"

#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/vertextemplate.h"

#include "treeface/guts/shapegenerator_guts.h"

using namespace treecore;

namespace treeface
{

struct ShapeGenerator::Guts
{
    Array<SubPath> subpaths;
};


ShapeGenerator::ShapeGenerator(): m_guts(new Guts())
{
}

ShapeGenerator::~ShapeGenerator()
{
    if (m_guts)
        delete m_guts;
}

void ShapeGenerator::clear()
{
    m_guts->subpaths.clear();
}

void ShapeGenerator::close_path()
{
    if (m_guts->subpaths.size() < 2)
        return;

    SubPath& curr_path = m_guts->subpaths.getLast();

    curr_path.closed = true;
}

Geometry* ShapeGenerator::fill_simple()
{
    Geometry* result = fill_simple_preserve();
    clear();
    return result;
}

Geometry* ShapeGenerator::fill_simple_preserve()
{
    Array<Vec2f> vertices;
    Array<IndexType> indices;

    for (int i_sub = 0; i_sub < m_guts->subpaths.size(); i_sub++)
    {
        const SubPath& curr_sub = m_guts->subpaths[i_sub];
        curr_sub.triangulate_simple(vertices, indices);
    }

    Geometry* result = new Geometry(fill_template());
    result->get_buffer()->set_host_data(ArrayRef<const Vec2f>(vertices), ArrayRef<const IndexType>(indices));

    return result;
}

void ShapeGenerator::line_to(const Vec2f& position)
{
    if (m_guts->subpaths.size() == 0)
        m_guts->subpaths.add(SubPath());

    // get last sub path, ensure it is unclosed
    SubPath& curr_path = m_guts->subpaths.getLast();
    curr_path.try_reopen_closed_path();

    // record glyph
    curr_path.vertices.add(position);
}

void ShapeGenerator::line_to_rel(const Vec2f& offset)
{
    if (m_guts->subpaths.size() == 0)
        m_guts->subpaths.add(SubPath());

    // get last sub path, ensure it is unclosed
    SubPath& curr_path = m_guts->subpaths.getLast();
    curr_path.try_reopen_closed_path();

    // record glyph using relative position
    Vec2f pos = offset;
    if (curr_path.vertices.size() > 0)
        pos += curr_path.vertices.getLast();
    curr_path.vertices.add(pos);
}

void ShapeGenerator::move_to(const Vec2f& position)
{
    // create new subpath and set its first position
    m_guts->subpaths.add(SubPath());
    SubPath& curr_path = m_guts->subpaths.getLast();
    curr_path.vertices.add(position);
}

void ShapeGenerator::move_to_rel(const Vec2f& offset)
{
    // calculate absolute position
    Vec2f pos = offset;
    if (m_guts->subpaths.size() > 0)
    {
        SubPath& prev_path = m_guts->subpaths.getLast();
        jassert(prev_path.vertices.size() > 0);
        if (prev_path.closed)
            pos += prev_path.vertices.getFirst();
        else
            pos += prev_path.vertices.getLast();
    }

    move_to(pos);
}

} // namespace treeface
