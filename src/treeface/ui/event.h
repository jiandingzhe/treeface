#ifndef TREEFACE_EVENT_H
#define TREEFACE_EVENT_H

#include "treeface/common.h"
#include "treeface/enums.h"

#include "treeface/keyboard.h"

#include <treejuce/IntTypes.h>

TREEFACE_NAMESPACE_BEGIN

struct KeyboardEvent
{
    ButtonUpDown button_action;
    KeyButton button;
    treejuce::uint16 modifier;
};

struct MouseButtonEvent
{
    ButtonUpDown button_action;
    MouseButton button;
    treejuce::int32 x;
    treejuce::int32 y;
    treejuce::uint16 combo;
};

struct MouseMotionEvent
{
    treejuce::int32 x;
    treejuce::int32 y;
    treejuce::int32 dx;
    treejuce::int32 dy;
};

struct MouseWheelEvent
{
    treejuce::int32 x;
    treejuce::int32 y;
};

struct Event
{
    treejuce::uint32 timestamp;
    void* user_data;
    union
    {
        KeyboardEvent keyboard_event;
        MouseButtonEvent mouse_button_event;
        MouseMotionEvent mouse_motion_event;
        MouseWheelEvent mouse_wheel_event;
    };
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_EVENT_H
