#ifndef TREEFACE_KEY_BOARD_H
#define TREEFACE_KEY_BOARD_H

#include "treeface/base/Common.h"

namespace treeface {

typedef enum
{
    KEY_MOD_ALT_LEFT = 1,
    KEY_MOD_ALT_RIGHT = 1 << 1,
    KEY_MOD_ALT = KEY_MOD_ALT_LEFT | KEY_MOD_ALT_RIGHT,
    KEY_MOD_CTRL_LEFT = 1 << 2,
    KEY_MOD_CTRL_RIGHT = 1 << 3,
    KEY_MOD_CTRL = KEY_MOD_CTRL_LEFT | KEY_MOD_CTRL_RIGHT,
    KEY_MOD_SHIFT_LEFT = 1 << 4,
    KEY_MOD_SHIFT_RIGHT = 1 << 5,
    KEY_MOD_SHIFT = KEY_MOD_SHIFT_LEFT | KEY_MOD_SHIFT_RIGHT,
    KEY_MOD_WIN_LEFT = 1 << 6,
    KEY_MOD_WIN_RIGHT = 1 << 7,
    KEY_MOD_WIN = KEY_MOD_WIN_LEFT | KEY_MOD_WIN_RIGHT,
    KEY_MOD_CAPS_LOCK = 1 << 8,
    KEY_MOD_NUM_LOCK = 1 << 9,
    KEY_MOD_SCORLL_ALTGR = 1 << 10,
} KeyModifier;

typedef enum
{
    KEY_BUTTON_UNKNWON = 0x00,

    // function keys
    KEY_BUTTON_ESCAPE = 0x01,
    KEY_BUTTON_F1     = 0x02,
    KEY_BUTTON_F2     = 0x03,
    KEY_BUTTON_F3     = 0x04,
    KEY_BUTTON_F4     = 0x05,
    KEY_BUTTON_F5     = 0x06,
    KEY_BUTTON_F6     = 0x07,
    KEY_BUTTON_F7     = 0x08,
    KEY_BUTTON_F8     = 0x09,
    KEY_BUTTON_F9     = 0x0a,
    KEY_BUTTON_F10    = 0x0b,
    KEY_BUTTON_F11    = 0x0c,
    KEY_BUTTON_F12    = 0x0d,

    // main area keys
    KEY_BUTTON_GRAVE     = 0x10,
    KEY_BUTTON_0         = 0x11,
    KEY_BUTTON_1         = 0x12,
    KEY_BUTTON_2         = 0x13,
    KEY_BUTTON_3         = 0x14,
    KEY_BUTTON_4         = 0x15,
    KEY_BUTTON_5         = 0x16,
    KEY_BUTTON_6         = 0x17,
    KEY_BUTTON_7         = 0x18,
    KEY_BUTTON_8         = 0x19,
    KEY_BUTTON_9         = 0x1a,
    KEY_BUTTON_MINUS     = 0x1b,
    KEY_BUTTON_EQUAL     = 0x1c,
    KEY_BUTTON_BACKSPACE = 0x1d,

    KEY_BUTTON_TAB           = 0x1e,
    KEY_BUTTON_Q             = 0x1f,
    KEY_BUTTON_W             = 0x20,
    KEY_BUTTON_E             = 0x21,
    KEY_BUTTON_R             = 0x22,
    KEY_BUTTON_T             = 0x23,
    KEY_BUTTON_Y             = 0x24,
    KEY_BUTTON_U             = 0x25,
    KEY_BUTTON_I             = 0x26,
    KEY_BUTTON_O             = 0x27,
    KEY_BUTTON_P             = 0x28,
    KEY_BUTTON_LEFT_BRACKET  = 0x29,
    KEY_BUTTON_RIGHT_BRACKET = 0x2a,
    KEY_BUTTON_BACK_SLASH    = 0x2b,

    KEY_BUTTON_CAPS_LOCK = 0x2c,
    KEY_BUTTON_A         = 0x2d,
    KEY_BUTTON_S         = 0x2e,
    KEY_BUTTON_D         = 0x2f,
    KEY_BUTTON_F         = 0x30,
    KEY_BUTTON_G         = 0x31,
    KEY_BUTTON_H         = 0x32,
    KEY_BUTTON_J         = 0x33,
    KEY_BUTTON_K         = 0x34,
    KEY_BUTTON_L         = 0x35,
    KEY_BUTTON_COLON     = 0x36,
    KEY_BUTTON_QUOTE     = 0x37,
    KEY_BUTTON_ENTER     = 0x38,

    KEY_BUTTON_LEFT_SHIFT  = 0x39,
    KEY_BUTTON_Z           = 0x3a,
    KEY_BUTTON_X           = 0x3b,
    KEY_BUTTON_C           = 0x3c,
    KEY_BUTTON_V           = 0x3d,
    KEY_BUTTON_B           = 0x3e,
    KEY_BUTTON_N           = 0x3f,
    KEY_BUTTON_M           = 0x40,
    KEY_BUTTON_COMMA       = 0x41,
    KEY_BUTTON_PERIOD      = 0x42,
    KEY_BUTTON_SLASH       = 0x43,
    KEY_BUTTON_RIGHT_SHIFT = 0x44,

    KEY_BUTTON_LEFT_CTRL    = 0x45,
    KEY_BUTTON_LEFT_WIN     = 0x46,
    KEY_BUTTON_CONTEXT_MENU = 0x47,
    KEY_BUTTON_LEFT_ALT     = 0x48,
    KEY_BUTTON_RIGHT_ALT    = 0x49,
    KEY_BUTTON_RIGHT_WIN    = 0x4a,
    KEY_BUTTON_RIGHT_CTRL   = 0x4b,

    // system keys
    KEY_BUTTON_PRINT_SCREEN = 0x50,
    KEY_BUTTON_SCROLL_LOCK  = 0x51,
    KEY_BUTTON_PAUSE_BREAK  = 0x52,
    KEY_BUTTON_INSERT       = 0x53,
    KEY_BUTTON_DELETE       = 0x54,
    KEY_BUTTON_HOME         = 0x55,
    KEY_BUTTON_END          = 0x56,
    KEY_BUTTON_PAGE_UP      = 0x57,
    KEY_BUTTON_PAGE_DOWN    = 0x58,

    // cursor
    KEY_BUTTON_UP    = 0x60,
    KEY_BUTTON_DOWN  = 0x61,
    KEY_BUTTON_LEFT  = 0x62,
    KEY_BUTTON_RIGHT = 0x63,

    // numpad keys
    KEY_BUTTON_NUM_LOCK      = 0x70,
    KEY_BUTTON_NUMPAD_DIV    = 0x71,
    KEY_BUTTON_NUMPAD_MUL    = 0x72,
    KEY_BUTTON_NUMPAD_SUB    = 0x73,
    KEY_BUTTON_NUMPAD_7      = 0x74,
    KEY_BUTTON_NUMPAD_8      = 0x75,
    KEY_BUTTON_NUMPAD_9      = 0x76,
    KEY_BUTTON_NUMPAD_ADD    = 0x77,
    KEY_BUTTON_NUMPAD_4      = 0x78,
    KEY_BUTTON_NUMPAD_5      = 0x79,
    KEY_BUTTON_NUMPAD_6      = 0x7a,
    KEY_BUTTON_NUMPAD_1      = 0x7b,
    KEY_BUTTON_NUMPAD_2      = 0x7c,
    KEY_BUTTON_NUMPAD_3      = 0x7d,
    KEY_BUTTON_NUMPAD_ENTER  = 0x7e,
    KEY_BUTTON_NUMPAD_0      = 0x7f,
    KEY_BUTTON_NUMPAD_PERIOD = 0x80,
} KeyButton;

} // namespace treeface

#endif // TREEFACE_KEYBOARD_H
