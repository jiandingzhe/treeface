#ifndef TREEFACE_WIDGET_H
#define TREEFACE_WIDGET_H

#include "treeface/common.h"
#include "treeface/ui/event.h"

#include <treejuce/ArrayRef.h>
#include <treejuce/Object.h>

class TestFramework;

TREEFACE_JUCE_NAMESPACE_BEGIN
class String;
class Result;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class SceneNode;
class Widget;

typedef bool (*EventFunc)(Widget* emitter, Widget* listener, const Event& event);

class Widget: public treejuce::Object
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

    bool add_event_listener(const treejuce::String& event_name, EventFunc func, void* data) NOEXCEPT;
    bool remove_event_listener(const treejuce::String& event_name);
    bool remove_event_listener(const treejuce::String& event_name, EventFunc func);

    bool can_handle(const treejuce::String& event_name) const NOEXCEPT;
    bool has_handler(const treejuce::String& event_name, EventFunc func) const NOEXCEPT;
    bool any_child_can_handle(const treejuce::String& event_name) const NOEXCEPT;
    bool any_parent_can_handle(const treejuce::String& event_name) const NOEXCEPT;

protected:


    struct Guts;
    Guts* m_guts;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_WIDGET_H
