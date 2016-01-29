#ifndef TREEFACE_SHAPE_GENERATOR_GUTS_H
#define TREEFACE_SHAPE_GENERATOR_GUTS_H

#include "treeface/graphics/ShapeGenerator.h"
#include "treeface/graphics/guts/Enums.h"

#include "treeface/gl/TypeUtils.h"
#include "treeface/graphics/HalfEdge.h"
#include "treeface/graphics/guts/PathGlyph.h"
#include "treeface/graphics/guts/SubPath.h"
#include "treeface/graphics/BBox2.h"
#include "treeface/math/Vec3.h"
#include "treeface/scene/Geometry.h"

#include <treecore/Array.h>

namespace treeface
{

extern const treecore::String UNI_NAME_LINE_WIDTH;
extern const treecore::String UNI_NAME_SKLT_MIN;
extern const treecore::String UNI_NAME_SKLT_MAX;

struct ShapeGenerator::Guts
{
    treecore::Array<SubPath> subpaths;

    void triangulate( Geometry::HostVertexCache& result_vertices,
                      treecore::Array<IndexType>& result_indices,
                      Vec2f& result_skeleton_min,
                      Vec2f& result_skeleton_max );
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_GUTS_H
