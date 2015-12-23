#ifndef TREEFACE_GRAPHICS_GUTS_ENUMS_H
#define TREEFACE_GRAPHICS_GUTS_ENUMS_H

namespace treeface
{

typedef enum
{
    GLYPH_TYPE_INVALID = -1,
    GLYPH_TYPE_LINE,
    GLYPH_TYPE_ARC,
    GLYPH_TYPE_BESSEL3,
    GLYPH_TYPE_BESSEL4,
} GlyphType;

typedef enum {
    VTX_ROLE_INVALID = -1,
    VTX_ROLE_START,
    VTX_ROLE_END,
    VTX_ROLE_LEFT,
    VTX_ROLE_RIGHT,
    VTX_ROLE_SPLIT,
    VTX_ROLE_MERGE
} VertexRole;

} // namespace treeface

#endif // TREEFACE_GRAPHICS_GUTS_ENUMS_H
