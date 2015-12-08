#ifndef TREEFACE_SHAPE_GENERATOR_H
#define TREEFACE_SHAPE_GENERATOR_H

#include <treecore/RefCountObject.h>
#include <treecore/RefCountSingleton.h>

#include "treeface/math/vec2.h"
#include "treeface/enums.h"

namespace treeface
{

class Geometry;
class VertexTemplate;

class ShapeGenerator: public treecore::RefCountObject, public treecore::RefCountSingleton<ShapeGenerator>
{
public:
    static const VertexTemplate& stroke_template();
    static const VertexTemplate& fill_template();

    ShapeGenerator();
    virtual ~ShapeGenerator();

    void clear();

    void close_path();

    ///
    /// \brief do simple fill without edge crossing test
    ///
    /// If shape has self-intersections, fill result could be not correct
    ///
    /// \return a new Geometry object containing triangulated result
    ///
    Geometry* fill_simple();

    ///
    /// \brief do simple fill without clearing current path state
    /// \see fill_simple(float)
    Geometry* fill_simple_preserve();

    Geometry* fill_complicated();

    Geometry* fill_complicated_preserve();

    void line_to(const Vec2f& position);

    void line_to_rel(const Vec2f& offset);

    void move_to(const Vec2f& position);

    void move_to_rel(const Vec2f& offset);

    Geometry* stroke_complicated(LineCapStyle cap_style, LineJoinStyle join_style, float line_width);

    Geometry* stroke_complicated_preserve(LineCapStyle cap_style, LineJoinStyle join_style, float line_width);

    TREECORE_DECLARE_NON_COPYABLE(ShapeGenerator);
    TREECORE_DECLARE_NON_MOVABLE(ShapeGenerator);

private:
    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_H