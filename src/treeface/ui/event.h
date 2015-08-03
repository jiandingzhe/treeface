#ifndef TREEFACE_EVENT_H
#define TREEFACE_EVENT_H

#include "treeface/common.h"
#include "treeface/enums.h"

#include "treeface/keyboard.h"

#include <treecore/IntTypes.h>

namespace treeface {

struct KeyboardEvent
{
    ButtonUpDown button_action;
    KeyButton button;
    treecore::uint16 modifier;
};

struct MouseButtonEvent
{
    ButtonUpDown button_action;
    MouseButton button;
    treecore::int32 x;
    treecore::int32 y;
    treecore::uint16 combo;
};

struct MouseMotionEvent
{
    treecore::int32 x;
    treecore::int32 y;
    treecore::int32 dx;
    treecore::int32 dy;
};

struct MouseWheelEvent
{
    treecore::int32 x;
    treecore::int32 y;
};

struct Event
{
    treecore::uint32 timestamp;
    void* user_data;
    union
    {
        KeyboardEvent keyboard_event;
        MouseButtonEvent mouse_button_event;
        MouseMotionEvent mouse_motion_event;
        MouseWheelEvent mouse_wheel_event;
    };
};

} // namespace treeface

#endif // TREEFACE_EVENT_H
