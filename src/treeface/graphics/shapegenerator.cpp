#include "treeface/graphics/shapegenerator.h"

#include "treeface/gl/vertextemplate.h"

#include "treeface/graphics/guts/shapegenerator_guts.h"

using namespace treecore;

namespace treeface
{

ShapeGenerator::ShapeGenerator(): m_guts( new Guts() )
{}

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

void ShapeGenerator::fill_simple( Geometry* geom )
{
    fill_simple_preserve( geom );
    clear();
}

void ShapeGenerator::fill_simple_preserve( Geometry* geom )
{
    abort(); // TODO: implement me
}

void ShapeGenerator::line_to( const Vec2f& position )
{
    if (m_guts->subpaths.size() == 0)
        m_guts->subpaths.add( SubPath() );

    // get last sub path, ensure it is unclosed
    SubPath& curr_path = m_guts->subpaths.getLast();
    curr_path.try_reopen_closed_path();

    // record glyph
    curr_path.glyphs.add( PathGlyph( position ) );
}

void ShapeGenerator::line_to_rel( const Vec2f& offset )
{
    if (m_guts->subpaths.size() == 0)
        m_guts->subpaths.add( SubPath() );

    // get last sub path, ensure it is unclosed
    SubPath& curr_path = m_guts->subpaths.getLast();
    curr_path.try_reopen_closed_path();

    // record glyph using relative position
    Vec2f pos = offset;
    if (curr_path.glyphs.size() > 0)
        pos += curr_path.glyphs.getLast().end;
    curr_path.glyphs.add( PathGlyph( pos ) );
}

void ShapeGenerator::move_to( const Vec2f& position )
{
    // create new subpath and set its first position
    m_guts->subpaths.add( SubPath() );
    SubPath& curr_path = m_guts->subpaths.getLast();
    curr_path.glyphs.add( PathGlyph( position ) );
}

void ShapeGenerator::move_to_rel( const Vec2f& offset )
{
    // calculate absolute position
    Vec2f pos = offset;
    if (m_guts->subpaths.size() > 0)
    {
        SubPath& prev_path = m_guts->subpaths.getLast();
        jassert( prev_path.glyphs.size() > 0 );
        if (prev_path.closed)
            pos += prev_path.glyphs.getFirst().end;
        else
            pos += prev_path.glyphs.getLast().end;
    }

    move_to( pos );
}

Geometry* ShapeGenerator::create_simple_stroke_geometry()
{
    return new Geometry( VERTEX_TEMPLATE_STROKE(), TFGL_PRIMITIVE_LINE_STRIP, TFGL_BUFFER_DYNAMIC_DRAW );
}

Geometry* ShapeGenerator::create_complicated_stroke_geometry()
{
    return new Geometry( VERTEX_TEMPLATE_STROKE(), TFGL_PRIMITIVE_TRIANGLES, TFGL_BUFFER_DYNAMIC_DRAW );
}

struct StrokeTemplateHelper: public treecore::RefCountObject, public treecore::RefCountSingleton<StrokeTemplateHelper>
{
    StrokeTemplateHelper()
    {
        value.add_attrib( VertexAttrib( "position", 2, TFGL_TYPE_FLOAT ),      false, 8 );
        value.add_attrib( VertexAttrib( "tangent_unorm", 2, TFGL_TYPE_FLOAT ), false, 8 );
        value.add_attrib( VertexAttrib( "trip", 1, TFGL_TYPE_FLOAT ),          false, 4 );
        value.add_attrib( VertexAttrib( "side", 1, TFGL_TYPE_FLOAT ),          false, 4 );
        jassert( sizeof(StrokeVertex) == value.vertex_size() );
    }

    virtual ~StrokeTemplateHelper() = default;

    VertexTemplate value;
};

struct FillTemplateHelper: public treecore::RefCountObject, public treecore::RefCountSingleton<FillTemplateHelper>
{
    FillTemplateHelper()
    {
        value.add_attrib( VertexAttrib( "position", 2, TFGL_TYPE_FLOAT ), false, 8 );
        jassert( sizeof(Vec2f) == value.vertex_size() );
    }

    virtual ~FillTemplateHelper() = default;

    VertexTemplate value;
};

const VertexTemplate& ShapeGenerator::VERTEX_TEMPLATE_STROKE()
{
    return StrokeTemplateHelper::getInstance()->value;
}

const VertexTemplate& ShapeGenerator::VERTEX_TEMPLATE_FILL()
{
    return FillTemplateHelper::getInstance()->value;
}

} // namespace treeface
