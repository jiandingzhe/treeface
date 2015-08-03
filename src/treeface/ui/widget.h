#ifndef TREEFACE_WIDGET_H
#define TREEFACE_WIDGET_H

#include "treeface/common.h"
#include "treeface/ui/event.h"

#include <treecore/ArrayRef.h>
#include <treecore/Object.h>

class TestFramework;

namespace treecore {
class String;
class Result;
} // namespace treecore

namespace treeface {

class SceneNode;
class Widget;

typedef bool (*EventFunc)(Widget* emitter, Widget* listener, const Event& event);

class Widget: public treecore::Object
{
    friend class ::TestFramework;

public:
    Widget() NOEXCEPT;
    Widget(treeface::SceneNode* node) NOEXCEPT;
    virtual ~Widget();

    bool add_child(Widget* child) NOEXCEPT;
    bool remove_child(Widget* child) NOEXCEPT;
    bool has_child(Widget* child) const NOEXCEPT;

    Widget* get_parent() NOEXCEPT;
    SceneNode* get_scene_node() NOEXCEPT;

    bool add_event_listener(const treecore::String& event_name, EventFunc func, void* data) NOEXCEPT;
    bool remove_event_listener(const treecore::String& event_name);
    bool remove_event_listener(const treecore::String& event_name, EventFunc func);

    bool can_handle(const treecore::String& event_name) const NOEXCEPT;
    bool has_handler(const treecore::String& event_name, EventFunc func) const NOEXCEPT;
    bool any_child_can_handle(const treecore::String& event_name) const NOEXCEPT;
    bool any_parent_can_handle(const treecore::String& event_name) const NOEXCEPT;

protected:


    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_WIDGET_H
