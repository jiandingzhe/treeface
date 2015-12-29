#ifndef TREEFACE_SHAPE_GENERATOR_GUTS_H
#define TREEFACE_SHAPE_GENERATOR_GUTS_H

#include "treeface/graphics/shapegenerator.h"
#include "treeface/graphics/guts/enums.h"

#include "treeface/gl/type.h"
#include "treeface/graphics/halfedge.h"
#include "treeface/graphics/guts/PathGlyph.h"
#include "treeface/graphics/guts/SubPath.h"
#include "treeface/math/bbox2.h"
#include "treeface/math/vec3.h"
#include "treeface/scene/geometry.h"

#include <treecore/Array.h>

namespace treeface
{

struct ShapeGenerator::Guts
{
    treecore::Array<SubPath> subpaths;

    void triangulate(treecore::Array<Vec2f>& result_vertices, treecore::Array<IndexType>& result_indices);
};

} // namespace treeface

#endif // TREEFACE_SHAPE_GENERATOR_GUTS_H
