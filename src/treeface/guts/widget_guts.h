#ifndef TREEFACE_WIDGET_GUTS_H
#define TREEFACE_WIDGET_GUTS_H

#include "treeface/ui/widget.h"

#include "treeface/scene/scenenode.h"

#include <treecore/HashMap.h>
#include <treecore/Holder.h>
#include <treecore/SortedSet.h>
#include <treecore/String.h>

namespace treeface {


struct EventClosure
{
    EventFunc fp;
    void* data;
};

typedef treecore::Array<EventClosure> EventArray;
typedef treecore::HashMap<treecore::String, treecore::Holder<EventArray> > EventMap;

struct Widget::Guts
{
    typedef treecore::Array<Widget::Guts*> WidgetRawArray;
    typedef treecore::HashMap<treecore::String, treecore::Holder<WidgetRawArray> > RespWidgetMap;

    treecore::String name;
    treecore::Array<treecore::Holder<Widget> > children;
    Widget* parent = nullptr;

    treecore::Holder<SceneNode> scene_node;

    bool resp_children_ok = false;
    bool resp_parents_ok = false;
    RespWidgetMap resp_children;    // event => listening children
    RespWidgetMap resp_parents;     // event => listening parents
    EventMap      abilities;        // event => handler

    void fetch_this_and_parent_ability(RespWidgetMap& result) NOEXCEPT;

    // clear children's and this widget's resp-parent cache
    void recur_clear_resp_parent_cache() NOEXCEPT;

    // clear parents' and this widget's resp-child cache
    void recur_clear_resp_child_cache() NOEXCEPT;

    // rebuild this widget's resp-parent cache
    // then update all children's
    void recur_build_resp_parent_cache() NOEXCEPT;

    // rebuild this widget's resp-child cache
    // then update all parent's
    void recur_build_resp_child_cache() NOEXCEPT;

    void recur_remove_resp_parent(const treecore::String& ev_name, Guts* to_be_removed) NOEXCEPT;

    void recur_remove_resp_child(const treecore::String& ev_name, Guts* to_be_removed) NOEXCEPT;

    // If this widget's resp-parent is up to date, prepend-update it, otherwise
    // build it. Then do this on all children.
    // This is designed for post-process the newly added child widget hierarchy,
    // which may holding an up-to-date resp-parent cache, but still need to be
    // updated with the added-to parent's resp-parent cache.
    void recur_build_or_prepend_update_resp_parent_cache(const RespWidgetMap& prefix_data) NOEXCEPT;

    void build_resp_parent_cache() NOEXCEPT;
    void build_resp_child_cache() NOEXCEPT;
    void prepend_update_resp_parent_cache(const RespWidgetMap& prefix_data) NOEXCEPT;

    inline void ensure_resp_parent_cache() NOEXCEPT
    {
        if (!resp_parents_ok)
        {
            recur_build_resp_parent_cache();
        }
    }

    inline void ensure_resp_child_cache() NOEXCEPT
    {
        if (!resp_children_ok)
        {
            recur_build_resp_child_cache();
        }
    }
};

} // namespace treeface

#endif
