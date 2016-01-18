#ifndef TREEFACE_SHAPE_GENERATOR_H
#define TREEFACE_SHAPE_GENERATOR_H

#include <treecore/RefCountObject.h>
#include <treecore/RefCountSingleton.h>

#include "treeface/math/vec2.h"
#include "treeface/enums.h"

class TestFramework;

namespace treeface
{

class Geometry;
class VertexTemplate;

class ShapeGenerator: public treecore::RefCountObject, public treecore::RefCountSingleton<ShapeGenerator>
{
    friend class ::TestFramework;

public:
    ShapeGenerator();

    TREECORE_DECLARE_NON_COPYABLE(ShapeGenerator);
    TREECORE_DECLARE_NON_MOVABLE(ShapeGenerator);

    virtual ~ShapeGenerator();

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
    void fill_simple(Geometry* geom);

    ///
    /// \brief do simple fill without clearing current path state
    /// \see fill_simple(float)
    void fill_simple_preserve(Geometry* geom);

    void stroke_complicated(LineCap cap_style, LineJoin join_style, float line_width, Geometry* geom);

    void stroke_complicated_preserve(LineCap cap_style, LineJoin join_style, float line_width, Geometry* geom);

    void line_to(const Vec2f& position);

    void line_to_rel(const Vec2f& offset);

    void move_to(const Vec2f& position);

    void move_to_rel(const Vec2f& offset);

    Geometry* create_simple_stroke_geometry();
    Geometry* create_complicated_stroke_geometry();

    static const VertexTemplate& VERTEX_TEMPLATE_STROKE();
    static const VertexTemplate& VERTEX_TEMPLATE_FILL();

private:
    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_H
