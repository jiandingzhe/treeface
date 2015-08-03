#ifndef TREEFACE_FACE_H
#define TREEFACE_FACE_H

#include "treeface/common.h"
#include "treeface/enums.h"

#include <treecore/Object.h>
#include <treecore/IntTypes.h>

class TestFramework;

namespace treeface {

class Widget;
class Scene;

struct KeyboardEvent;
struct MouseButtonEvent;
struct MouseMotionEvent;
struct MouseWheelEvent;

class Face: public treecore::Object
{
    friend class ::TestFramework;

public:
    Face(treecore::uint32 width, treecore::uint32 height) NOEXCEPT;

    JUCE_DECLARE_NON_COPYABLE(Face)
    JUCE_DECLARE_NON_MOVABLE(Face)

    treecore::uint32 get_width() const NOEXCEPT;
    void set_width(treecore::uint32 value) NOEXCEPT;

    treecore::uint32 get_height() const NOEXCEPT;
    void set_height(treecore::uint32 value) NOEXCEPT;

    float get_project_depth() const NOEXCEPT;
    void set_project_depth(float value) NOEXCEPT;

    float get_project_dist() NOEXCEPT;

    float get_frustrum_angle() const NOEXCEPT;
    void set_frustrum_angle(float value) NOEXCEPT;

    Widget* get_root_widget_frust() NOEXCEPT;
    Widget* get_root_widget_ortho() NOEXCEPT;
    Scene* get_scene_frust() NOEXCEPT;
    Scene* get_scene_ortho() NOEXCEPT;

    // TODO: inject input devices
    void inject_mouse_down(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo, treecore::uint32 timestamp) NOEXCEPT;
    void inject_mouse_down(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo) NOEXCEPT;

    void inject_mouse_up(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo, treecore::uint32 timestamp) NOEXCEPT;
    void inject_mouse_up(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo) NOEXCEPT;

    void inject_mouse_move(treecore::int32 x, treecore::int32 y, treecore::int32 dx, treecore::int32 dy, treecore::uint32 timestamp) NOEXCEPT;
    void inject_mouse_move(treecore::int32 x, treecore::int32 y, treecore::int32 dx, treecore::int32 dy) NOEXCEPT;

    void inject_mouse_wheel(treecore::int32 x, treecore::int32 y, treecore::uint32 timestamp) NOEXCEPT;
    void inject_mouse_wheel(treecore::int32 x, treecore::int32 y) NOEXCEPT;

    void inject_key_down() NOEXCEPT;
    void inject_key_up() NOEXCEPT;

    void inject_time_delta(float delta) NOEXCEPT;

    void render() NOEXCEPT;

private:
    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_FACE_H
