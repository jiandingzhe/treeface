#ifndef TREEFACE_FACE_H
#define TREEFACE_FACE_H

#include "treeface/common.h"
#include "treeface/enums.h"

#include <treecore/RefCountObject.h>
#include <treecore/IntTypes.h>

class TestFramework;

namespace treeface {

class Widget;
class Scene;

struct KeyboardEvent;
struct MouseButtonEvent;
struct MouseMotionEvent;
struct MouseWheelEvent;

class Face: public treecore::RefCountObject
{
    friend class ::TestFramework;

public:
    Face(treecore::uint32 width, treecore::uint32 height) noexcept;

    TREECORE_DECLARE_NON_COPYABLE(Face)
    TREECORE_DECLARE_NON_MOVABLE(Face)

    treecore::uint32 get_width() const noexcept;
    void set_width(treecore::uint32 value) noexcept;

    treecore::uint32 get_height() const noexcept;
    void set_height(treecore::uint32 value) noexcept;

    float get_project_depth() const noexcept;
    void set_project_depth(float value) noexcept;

    float get_project_dist() noexcept;

    float get_frustrum_angle() const noexcept;
    void set_frustrum_angle(float value) noexcept;

    Widget* get_root_widget_frust() noexcept;
    Widget* get_root_widget_ortho() noexcept;
    Scene* get_scene_frust() noexcept;
    Scene* get_scene_ortho() noexcept;

    // TODO: inject input devices
    void inject_mouse_down(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo, treecore::uint32 timestamp) noexcept;
    void inject_mouse_down(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo) noexcept;

    void inject_mouse_up(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo, treecore::uint32 timestamp) noexcept;
    void inject_mouse_up(MouseButton button, treecore::int32 x, treecore::int32 y, treecore::uint16 combo) noexcept;

    void inject_mouse_move(treecore::int32 x, treecore::int32 y, treecore::int32 dx, treecore::int32 dy, treecore::uint32 timestamp) noexcept;
    void inject_mouse_move(treecore::int32 x, treecore::int32 y, treecore::int32 dx, treecore::int32 dy) noexcept;

    void inject_mouse_wheel(treecore::int32 x, treecore::int32 y, treecore::uint32 timestamp) noexcept;
    void inject_mouse_wheel(treecore::int32 x, treecore::int32 y) noexcept;

    void inject_key_down() noexcept;
    void inject_key_up() noexcept;

    void inject_time_delta(float delta) noexcept;

    void render() noexcept;

private:
    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_FACE_H
