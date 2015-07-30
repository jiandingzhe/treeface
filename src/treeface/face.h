#ifndef TREEFACE_FACE_H
#define TREEFACE_FACE_H

#include "treeface/common.h"
#include "treeface/enums.h"

#include <treejuce/Object.h>
#include <treejuce/IntTypes.h>

class TestFramework;

TREEFACE_NAMESPACE_BEGIN

class Widget;
class Scene;

struct KeyboardEvent;
struct MouseButtonEvent;
struct MouseMotionEvent;
struct MouseWheelEvent;

class Face: public treejuce::Object
{
    friend class ::TestFramework;

public:
    Face(treejuce::uint32 width, treejuce::uint32 height) NOEXCEPT;

    JUCE_DECLARE_NON_COPYABLE(Face)
    JUCE_DECLARE_NON_MOVABLE(Face)

    treejuce::uint32 get_width() const NOEXCEPT;
    void set_width(treejuce::uint32 value) NOEXCEPT;

    treejuce::uint32 get_height() const NOEXCEPT;
    void set_height(treejuce::uint32 value) NOEXCEPT;

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
    void inject_mouse_down(MouseButton button, treejuce::int32 x, treejuce::int32 y, treejuce::uint16 combo, treejuce::uint32 timestamp) NOEXCEPT;
    void inject_mouse_down(MouseButton button, treejuce::int32 x, treejuce::int32 y, treejuce::uint16 combo) NOEXCEPT;

    void inject_mouse_up(MouseButton button, treejuce::int32 x, treejuce::int32 y, treejuce::uint16 combo, treejuce::uint32 timestamp) NOEXCEPT;
    void inject_mouse_up(MouseButton button, treejuce::int32 x, treejuce::int32 y, treejuce::uint16 combo) NOEXCEPT;

    void inject_mouse_move(treejuce::int32 x, treejuce::int32 y, treejuce::int32 dx, treejuce::int32 dy, treejuce::uint32 timestamp) NOEXCEPT;
    void inject_mouse_move(treejuce::int32 x, treejuce::int32 y, treejuce::int32 dx, treejuce::int32 dy) NOEXCEPT;

    void inject_mouse_wheel(treejuce::int32 x, treejuce::int32 y, treejuce::uint32 timestamp) NOEXCEPT;
    void inject_mouse_wheel(treejuce::int32 x, treejuce::int32 y) NOEXCEPT;

    void inject_key_down() NOEXCEPT;
    void inject_key_up() NOEXCEPT;

    void inject_time_delta(float delta) NOEXCEPT;

    void render() NOEXCEPT;

private:
    struct Guts;
    Guts* m_guts;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_FACE_H
