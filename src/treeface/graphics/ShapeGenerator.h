#ifndef TREEFACE_SHAPE_GENERATOR_H
#define TREEFACE_SHAPE_GENERATOR_H

#include <treecore/Identifier.h>
#include <treecore/RefCountObject.h>
#include <treecore/RefCountSingleton.h>

#include "treeface/math/Vec2.h"
#include "treeface/graphics/Utils.h"
#include "treeface/base/Enums.h"

class TestFramework;

namespace treeface
{

class Geometry;
class VertexTemplate;

class ShapeGenerator: public treecore::RefCountObject, public treecore::RefCountSingleton<ShapeGenerator>
{
    friend class ::TestFramework;
    friend class treecore::RefCountSingleton<ShapeGenerator>;

public:
    TREECORE_DECLARE_NON_COPYABLE( ShapeGenerator );
    TREECORE_DECLARE_NON_MOVABLE( ShapeGenerator );

    void clear();

    void close_path();

    ///
    /// \brief do simple fill without edge crossing test
    ///
    /// If shape has self-intersections, fill result could be not correct
    ///
    /// \return
    /// \return a new Geometry object containing triangulated result
    ///
    void fill_simple( Geometry* geom );

    ///
    /// \brief do simple fill without clearing current path state
    /// \see fill_simple(float)
    void fill_simple_preserve( Geometry* geom ) const;

    void stroke_complicated( const StrokeStyle& style, Geometry* geom );

    void stroke_complicated_preserve( const StrokeStyle& style, Geometry* geom ) const;

    void line_to( const Vec2f& position );

    void line_to_rel( const Vec2f& offset );

    void move_to( const Vec2f& position );

    void move_to_rel( const Vec2f& offset );

    void curve_to( const Vec2f& ctrl, const Vec2f& end );

    void curve_to( const Vec2f& ctrl1, const Vec2f& ctrl2, const Vec2f& end );

    static Geometry* create_simple_stroke_geometry();
    static Geometry* create_complicated_stroke_geometry();
    static Geometry* create_fill_geometry();

    static const VertexTemplate& VERTEX_TEMPLATE_STROKE();
    static const VertexTemplate& VERTEX_TEMPLATE_FILL();

private:
    ShapeGenerator();
    virtual ~ShapeGenerator();

    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_H
