#ifndef TREEFACE_ENUMS_H
#define TREEFACE_ENUMS_H

#include "treeface/common.h"

namespace treeface {

typedef enum
{
    MATERIAL_RAW = 0,
    MATERIAL_SCENE_GRAPH = 1,
    MATERIAL_SCREEN_SPACE = 2,
    MATERIAL_LINE_STROKE = 3,
} MaterialType;

typedef enum
{
    EVENT_USER = 0,
    EVENT_KEYBOARD = 1,
    EVENT_MOUSE_BUTTON = 2,
    EVENT_MOUSE_MOTION = 3,
    EVENT_MOUSE_WHEEL = 4,
} EventType;

typedef enum
{
    MOUSE_BUTTON_LEFT = 1,
    MOUSE_BUTTON_MIDDLE = 1 << 1,
    MOUSE_BUTTON_RIGHT = 1 << 2,
} MouseButton;

typedef enum
{
    BUTTON_DOWN = 1,
    BUTTON_UP = 0,
} ButtonUpDown;

typedef enum
{
    FULL_SCREEN_UNKNOWN = -1,
    FULL_SCREEN_OFF = 0,
    FULL_SCREEN_FAKE = 1,
    FULL_SCREEN_REAL = 2
} FullScreenMode;

typedef enum
{
    LINE_CAP_BUTT,
    LINE_CAP_ROUND,
    LINE_CAP_SQUARE
} LineCapStyle;

typedef enum
{
    LINE_JOIN_MITER,
    LINE_JOIN_ROUND,
    LINE_JOIN_BEVEL
} LineJoinStyle;

typedef enum
{
    FILL_NON_ZERO,
    FILL_EVEN_ODD,
} FillRule;

} // namespace treeface

#endif // TREEFACE_ENUMS_H
