#ifndef TREEFACE_WIDGET_H
#define TREEFACE_WIDGET_H

#include "treeface/common.h"
#include "treeface/event/event.h"

#include <treecore/ArrayRef.h>
#include <treecore/Identifier.h>
#include <treecore/RefCountObject.h>

class TestFramework;

namespace treecore {
class String;
class Result;
} // namespace treecore

namespace treeface {

class SceneNode;
class Widget;

typedef bool (*EventFunc)(Widget* emitter, Widget* listener, const Event& event);

class Widget: public treecore::RefCountObject
{
    friend class ::TestFramework;

public:
    Widget() noexcept;
    Widget(treeface::SceneNode* node) noexcept;
    virtual ~Widget();

    bool add_child(Widget* child) noexcept;
    bool remove_child(Widget* child) noexcept;
    bool has_child(Widget* child) const noexcept;

    Widget* get_parent() noexcept;
    SceneNode* get_scene_node() noexcept;

    bool add_event_listener(const treecore::Identifier& event_name, EventFunc func, void* data) noexcept;
    bool remove_event_listener(const treecore::Identifier& event_name);
    bool remove_event_listener(const treecore::Identifier& event_name, EventFunc func);

    bool can_handle(const treecore::Identifier& event_name) const noexcept;
    bool has_handler(const treecore::Identifier& event_name, EventFunc func) const noexcept;
    bool any_child_can_handle(const treecore::Identifier& event_name) const noexcept;
    bool any_parent_can_handle(const treecore::Identifier& event_name) const noexcept;

protected:


    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_WIDGET_H
