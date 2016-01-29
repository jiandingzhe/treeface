#ifndef TREEFACE_GRAPHICS_GUTS_ENUMS_H
#define TREEFACE_GRAPHICS_GUTS_ENUMS_H

namespace treeface
{

typedef enum
{
    GLYPH_TYPE_INVALID = 0,
    GLYPH_TYPE_LINE,
    GLYPH_TYPE_ARC,
    GLYPH_TYPE_BESSEL3,
    GLYPH_TYPE_BESSEL4,
} GlyphType;

typedef enum {
    VTX_ROLE_INVALID = 0,
    VTX_ROLE_END   = 1,
    VTX_ROLE_SPLIT = 2,
    VTX_ROLE_LEFT  = 3,
    VTX_ROLE_RIGHT = 4,
    VTX_ROLE_MERGE = 5,
    VTX_ROLE_START = 6,
} VertexRole;

} // namespace treeface

#endif // TREEFACE_GRAPHICS_GUTS_ENUMS_H
