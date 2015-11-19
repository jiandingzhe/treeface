#include "TestFramework.h"

#include "treeface/ui/widget.h"
#include "treeface/guts/widget_guts.h"

#include <treecore/RefCountHolder.h>

#include <cstdio>

using namespace treeface;
using namespace treecore;
using namespace std;

bool callback1foo_flag = false;
bool callback1foo(Widget* emitter, Widget* listener, const Event& e)
{
    printf("callback1foo %p => %p with %p\n", emitter, listener, e.user_data);
    callback1foo_flag = true;
    return false;
}

bool callback1bar_flag = false;
bool callback1bar(Widget* emitter, Widget* listener, const Event& e)
{
    printf("callback1bar_flag %p => %p with %p\n", emitter, listener, e.user_data);
    callback1bar_flag = true;
    return false;
}

bool callback2foo_flag = false;
bool callback2foo(Widget* emitter, Widget* listener, const Event& e)
{
    printf("callback2foo %p => %p with %p\n", emitter, listener, e.user_data);
    callback2foo_flag = true;
    return false;
}

bool callback2aaa_flag = false;
bool callback2aaa(Widget* emitter, Widget* listener, const Event& e)
{
    printf("callback2aaa %p => %p with %p\n", emitter, listener, e.user_data);
    callback2aaa_flag = true;
    return false;
}

bool callback4baz1_flag = false;
bool callback4baz1(Widget* emitter, Widget* listener, const Event& e)
{
    printf("callback4baz1 %p => %p with %p\n", emitter, listener, e.user_data);
    callback4baz1_flag = true;
    return false;
}

bool callback4baz2_flag = false;
bool callback4baz2(Widget* emitter, Widget* listener, const Event& e)
{
    printf("callback4baz2 %p => %p with %p\n", emitter, listener, e.user_data);
    callback4baz2_flag = true;
    return false;
}

void TestFramework::content()
{
    // create objects
    RefCountHolder<Widget> w1 = new Widget();
    RefCountHolder<Widget> w2 = new Widget();
    RefCountHolder<Widget> w3 = new Widget();
    RefCountHolder<Widget> w4 = new Widget();
    RefCountHolder<Widget> w5 = new Widget();
    RefCountHolder<Widget> w6 = new Widget();

    OK(w1.get());
    OK(w2.get());
    OK(w3.get());
    OK(w4.get());
    OK(w5.get());
    OK(w6.get());

    Widget::Guts* gut1 = w1->m_guts;
    Widget::Guts* gut2 = w2->m_guts;
    Widget::Guts* gut3 = w3->m_guts;
    Widget::Guts* gut4 = w4->m_guts;
    Widget::Guts* gut5 = w5->m_guts;
    Widget::Guts* gut6 = w6->m_guts;

    gut1->name = "w1";
    gut2->name = "w2";
    gut3->name = "w3";
    gut4->name = "w4";
    gut5->name = "w5";
    gut6->name = "w6";

    // add events
    OK(w1->add_event_listener("foo", callback1foo, nullptr));
    OK(w1->add_event_listener("bar", callback1bar, nullptr));
    OK(w2->add_event_listener("foo", callback2foo, nullptr));
    OK(w4->add_event_listener("baz", callback4baz1, nullptr));
    OK(w4->add_event_listener("baz", callback4baz2, nullptr));

    OK(w1->can_handle("foo"));
    OK(w1->can_handle("bar"));
    OK(w1->has_handler("foo", callback1foo));
    OK(w1->has_handler("bar", callback1bar));
    OK(w2->can_handle("foo"));
    OK(w2->has_handler("foo", callback2foo));
    OK(w4->can_handle("baz"));
    OK(w4->has_handler("baz", callback4baz1));
    OK(w4->has_handler("baz", callback4baz2));

    // add w2 to w1
    OK(w1->add_child(w2));

    IS(w2->m_guts->parent, w1.get());
    OK(w1->has_child(w2));
    OK(gut1->scene_node->has_child(gut2->scene_node));
    IS(gut2->scene_node->get_parent(), gut1->scene_node.get());

    OK(w1->any_child_can_handle("foo"));
    OK(w2->any_parent_can_handle("foo"));
    OK(w2->any_parent_can_handle("bar"));

    OK(w1->m_guts->resp_children_ok);
    IS(w1->m_guts->resp_children.size(), 1);
    IS(w1->m_guts->resp_children["foo"]->size(), 1);
    OK(w1->m_guts->resp_children["foo"]->contains(gut2));

    OK(w2->m_guts->resp_parents_ok);
    IS(w2->m_guts->resp_parents.size(), 2);
    IS(w2->m_guts->resp_parents["foo"]->size(), 1);
    OK(w2->m_guts->resp_parents["foo"]->contains(gut1));
    IS(w2->m_guts->resp_parents["bar"]->size(), 1);
    OK(w2->m_guts->resp_parents["bar"]->contains(gut1));

    // add w5 and w6 to w4
    OK(w4->add_child(w5));
    OK(w4->add_child(w6));

    //
    // add w4-w5-w6 to w2
    //
    OK(w2->add_child(w4));

    OK(w1->any_child_can_handle("foo"));
    OK(w1->any_child_can_handle("baz"));
    OK(w1->m_guts->resp_children_ok);
    IS(w1->m_guts->resp_children.size(), 2);
    IS(w1->m_guts->resp_children["foo"]->size(), 1);
    OK(w1->m_guts->resp_children["foo"]->contains(gut2));
    IS(w1->m_guts->resp_children["baz"]->size(), 1);
    OK(w1->m_guts->resp_children["baz"]->contains(gut4));

    OK(w2->any_child_can_handle("baz"));
    OK(w2->m_guts->resp_children_ok);
    IS(w2->m_guts->resp_children.size(), 1);
    IS(w2->m_guts->resp_children["baz"]->size(), 1);
    OK(w2->m_guts->resp_children["baz"]->contains(gut4));

    OK(w2->any_parent_can_handle("foo"));
    OK(w2->any_parent_can_handle("bar"));
    OK(w2->m_guts->resp_parents_ok);
    IS(w2->m_guts->resp_parents.size(), 2);
    IS(w2->m_guts->resp_parents["foo"]->size(), 1);
    OK(w2->m_guts->resp_parents["foo"]->contains(gut1));
    IS(w2->m_guts->resp_parents["bar"]->size(), 1);
    OK(w2->m_guts->resp_parents["bar"]->contains(gut1));

    OK(w4->any_parent_can_handle("foo"));
    OK(w4->any_parent_can_handle("bar"));
    OK(w4->m_guts->resp_parents_ok);
    IS(w4->m_guts->resp_parents.size(), 2);
    IS(w4->m_guts->resp_parents["foo"]->size(), 2);
    IS(w4->m_guts->resp_parents["foo"]->indexOf(gut1), 0);
    IS(w4->m_guts->resp_parents["foo"]->indexOf(gut2), 1);
    IS(w4->m_guts->resp_parents["bar"]->size(), 1);
    OK(w4->m_guts->resp_parents["bar"]->contains(gut1));

    IS(w4->m_guts->resp_children.size(), 0);

    OK(w5->any_parent_can_handle("foo"));
    OK(w5->any_parent_can_handle("bar"));
    OK(w5->any_parent_can_handle("baz"));
    OK(w5->m_guts->resp_parents_ok);
    IS(w5->m_guts->resp_parents.size(), 3);
    IS(w5->m_guts->resp_parents["foo"]->size(), 2);
    IS(w5->m_guts->resp_parents["foo"]->indexOf(gut1), 0);
    IS(w5->m_guts->resp_parents["foo"]->indexOf(gut2), 1);
    IS(w5->m_guts->resp_parents["bar"]->size(), 1);
    IS(w5->m_guts->resp_parents["bar"]->indexOf(gut1), 0);
    IS(w5->m_guts->resp_parents["baz"]->size(), 1);
    IS(w5->m_guts->resp_parents["baz"]->indexOf(gut4), 0);

    OK(w6->any_parent_can_handle("foo"));
    OK(w6->any_parent_can_handle("bar"));
    OK(w6->any_parent_can_handle("baz"));
    OK(w6->m_guts->resp_parents_ok);
    IS(w6->m_guts->resp_parents.size(), 3);
    IS(w6->m_guts->resp_parents["foo"]->size(), 2);
    IS(w6->m_guts->resp_parents["foo"]->indexOf(gut1), 0);
    IS(w6->m_guts->resp_parents["foo"]->indexOf(gut2), 1);
    IS(w6->m_guts->resp_parents["bar"]->size(), 1);
    IS(w6->m_guts->resp_parents["bar"]->indexOf(gut1), 0);
    IS(w6->m_guts->resp_parents["baz"]->size(), 1);
    IS(w6->m_guts->resp_parents["baz"]->indexOf(gut4), 0);

    // add w3 to w2
    OK(w2->add_child(w3));
    OK(w3->any_parent_can_handle("foo"));
    OK(w3->any_parent_can_handle("bar"));
    OK(w3->m_guts->resp_parents_ok);
    IS(w3->m_guts->resp_parents.size(), 2);
    IS(w3->m_guts->resp_parents["foo"]->size(), 2);
    IS(w3->m_guts->resp_parents["foo"]->indexOf(gut1), 0);
    IS(w3->m_guts->resp_parents["foo"]->indexOf(gut2), 1);
    IS(w3->m_guts->resp_parents["bar"]->size(), 1);
    OK(w3->m_guts->resp_parents["bar"]->contains(gut1));

    // add event listener to w2 when w2 is in hierarchy
    OK(w2->add_event_listener("aaa", callback2aaa, nullptr));
    OK(w2->can_handle("aaa"));

    OK(!gut1->resp_children_ok);

    OK(!gut3->resp_parents_ok);
    OK(!gut4->resp_parents_ok);
    OK(!gut5->resp_parents_ok);
    OK(!gut6->resp_parents_ok);

    OK(w1->any_child_can_handle("foo"));
    OK(w1->any_child_can_handle("baz"));
    OK(w1->any_child_can_handle("aaa"));
    OK(gut1->resp_children_ok);
    IS(gut1->resp_children.size(), 3);
    IS(gut1->resp_children["foo"]->size(), 1);
    OK(gut1->resp_children["foo"]->contains(gut2));
    IS(gut1->resp_children["baz"]->size(), 1);
    OK(gut1->resp_children["baz"]->contains(gut4));
    IS(gut1->resp_children["aaa"]->size(), 1);
    OK(gut1->resp_children["aaa"]->contains(gut2));

    OK(w2->any_parent_can_handle("foo"));
    OK(w2->any_parent_can_handle("bar"));
    OK(w2->any_child_can_handle("baz"));
    OK(gut2->resp_children_ok);
    IS(gut2->resp_children.size(), 1);
    IS(gut2->resp_children["baz"]->size(), 1);
    OK(gut2->resp_children["baz"]->contains(gut4));
    OK(gut2->resp_parents_ok);
    IS(gut2->resp_parents.size(), 2);
    IS(gut2->resp_parents["foo"]->size(), 1);
    OK(gut2->resp_parents["foo"]->contains(gut1));
    IS(gut2->resp_parents["bar"]->size(), 1);
    OK(gut2->resp_parents["bar"]->contains(gut1));

    OK(w3->any_parent_can_handle("foo"));
    OK(w3->any_parent_can_handle("bar"));
    OK(w3->any_parent_can_handle("aaa"));
    OK(gut3->resp_parents_ok);
    IS(gut3->resp_parents.size(), 3);
    IS(gut3->resp_parents["foo"]->size(), 2);
    OK(gut3->resp_parents["foo"]->contains(gut1));
    OK(gut3->resp_parents["foo"]->contains(gut2));
    IS(gut3->resp_parents["bar"]->size(), 1);
    OK(gut3->resp_parents["bar"]->contains(gut1));
    IS(gut3->resp_parents["aaa"]->size(), 1);
    OK(gut3->resp_parents["aaa"]->contains(gut2));

    OK(w4->any_parent_can_handle("foo"));
    OK(w4->any_parent_can_handle("bar"));
    OK(w4->any_parent_can_handle("aaa"));
    OK(gut4->resp_parents_ok);
    IS(gut4->resp_parents.size(), 3);
    IS(gut4->resp_parents["foo"]->size(), 2);
    OK(gut4->resp_parents["foo"]->contains(gut1));
    OK(gut4->resp_parents["foo"]->contains(gut2));
    IS(gut4->resp_parents["bar"]->size(), 1);
    OK(gut4->resp_parents["bar"]->contains(gut1));
    IS(gut4->resp_parents["aaa"]->size(), 1);
    OK(gut4->resp_parents["aaa"]->contains(gut2));

    OK(w5->any_parent_can_handle("foo"));
    OK(w5->any_parent_can_handle("bar"));
    OK(w5->any_parent_can_handle("baz"));
    OK(w5->any_parent_can_handle("aaa"));
    OK(gut5->resp_parents_ok);
    IS(gut5->resp_parents.size(), 4);
    IS(gut5->resp_parents["foo"]->size(), 2);
    IS(gut5->resp_parents["foo"]->indexOf(gut1), 0);
    IS(gut5->resp_parents["foo"]->indexOf(gut2), 1);
    IS(gut5->resp_parents["bar"]->size(), 1);
    IS(gut5->resp_parents["bar"]->indexOf(gut1), 0);
    IS(gut5->resp_parents["baz"]->size(), 1);
    IS(gut5->resp_parents["baz"]->indexOf(gut4), 0);
    IS(gut5->resp_parents["aaa"]->size(), 1);
    IS(gut5->resp_parents["aaa"]->indexOf(gut2), 0);

    OK(w6->any_parent_can_handle("foo"));
    OK(w6->any_parent_can_handle("bar"));
    OK(w6->any_parent_can_handle("baz"));
    OK(w6->any_parent_can_handle("aaa"));
    OK(gut6->resp_parents_ok);
    IS(gut6->resp_parents.size(), 4);
    IS(gut6->resp_parents["foo"]->size(), 2);
    IS(gut6->resp_parents["foo"]->indexOf(gut1), 0);
    IS(gut6->resp_parents["foo"]->indexOf(gut2), 1);
    IS(gut6->resp_parents["bar"]->size(), 1);
    IS(gut6->resp_parents["bar"]->indexOf(gut1), 0);
    IS(gut6->resp_parents["baz"]->size(), 1);
    IS(gut6->resp_parents["baz"]->indexOf(gut4), 0);
    IS(gut6->resp_parents["aaa"]->size(), 1);
    IS(gut6->resp_parents["aaa"]->indexOf(gut2), 0);

    // remove some callbacks
    OK(w2->remove_event_listener("foo"));
    OK(w4->remove_event_listener("baz", callback4baz1));

    OK(w1->any_child_can_handle("baz"));
    OK(w1->any_child_can_handle("aaa"));
    OK(gut1->resp_children_ok);
    IS(gut1->resp_children.size(), 2);
    IS(gut1->resp_children["baz"]->size(), 1);
    OK(gut1->resp_children["baz"]->contains(gut4));
    IS(gut1->resp_children["aaa"]->size(), 1);
    OK(gut1->resp_children["aaa"]->contains(gut2));

    OK(w2->any_parent_can_handle("foo"));
    OK(w2->any_parent_can_handle("bar"));
    OK(w2->any_child_can_handle("baz"));
    OK(gut2->resp_children_ok);
    IS(gut2->resp_children.size(), 1);
    IS(gut2->resp_children["baz"]->size(), 1);
    OK(gut2->resp_children["baz"]->contains(gut4));
    OK(gut2->resp_parents_ok);
    IS(gut2->resp_parents.size(), 2);
    IS(gut2->resp_parents["foo"]->size(), 1);
    OK(gut2->resp_parents["foo"]->contains(gut1));
    IS(gut2->resp_parents["bar"]->size(), 1);
    OK(gut2->resp_parents["bar"]->contains(gut1));

    OK(w3->any_parent_can_handle("foo"));
    OK(w3->any_parent_can_handle("bar"));
    OK(w3->any_parent_can_handle("aaa"));
    OK(gut3->resp_parents_ok);
    IS(gut3->resp_parents.size(), 3);
    IS(gut3->resp_parents["foo"]->size(), 1);
    OK(gut3->resp_parents["foo"]->contains(gut1));
    IS(gut3->resp_parents["bar"]->size(), 1);
    OK(gut3->resp_parents["bar"]->contains(gut1));
    IS(gut3->resp_parents["aaa"]->size(), 1);
    OK(gut3->resp_parents["aaa"]->contains(gut2));

    OK(w4->any_parent_can_handle("foo"));
    OK(w4->any_parent_can_handle("bar"));
    OK(w4->any_parent_can_handle("aaa"));
    OK(gut4->resp_parents_ok);
    IS(gut4->resp_parents.size(), 3);
    IS(gut4->resp_parents["foo"]->size(), 1);
    OK(gut4->resp_parents["foo"]->contains(gut1));
    IS(gut4->resp_parents["bar"]->size(), 1);
    OK(gut4->resp_parents["bar"]->contains(gut1));
    IS(gut4->resp_parents["aaa"]->size(), 1);
    OK(gut4->resp_parents["aaa"]->contains(gut2));

    OK(w5->any_parent_can_handle("foo"));
    OK(w5->any_parent_can_handle("bar"));
    OK(w5->any_parent_can_handle("baz"));
    OK(w5->any_parent_can_handle("aaa"));
    OK(gut5->resp_parents_ok);
    IS(gut5->resp_parents.size(), 4);
    IS(gut5->resp_parents["foo"]->size(), 1);
    IS(gut5->resp_parents["foo"]->indexOf(gut1), 0);
    IS(gut5->resp_parents["bar"]->size(), 1);
    IS(gut5->resp_parents["bar"]->indexOf(gut1), 0);
    IS(gut5->resp_parents["baz"]->size(), 1);
    IS(gut5->resp_parents["baz"]->indexOf(gut4), 0);
    IS(gut5->resp_parents["aaa"]->size(), 1);
    IS(gut5->resp_parents["aaa"]->indexOf(gut2), 0);

    OK(w6->any_parent_can_handle("foo"));
    OK(w6->any_parent_can_handle("bar"));
    OK(w6->any_parent_can_handle("baz"));
    OK(w6->any_parent_can_handle("aaa"));
    OK(gut6->resp_parents_ok);
    IS(gut6->resp_parents.size(), 4);
    IS(gut6->resp_parents["foo"]->size(), 1);
    IS(gut6->resp_parents["foo"]->indexOf(gut1), 0);
    IS(gut6->resp_parents["bar"]->size(), 1);
    IS(gut6->resp_parents["bar"]->indexOf(gut1), 0);
    IS(gut6->resp_parents["baz"]->size(), 1);
    IS(gut6->resp_parents["baz"]->indexOf(gut4), 0);
    IS(gut6->resp_parents["aaa"]->size(), 1);
    IS(gut6->resp_parents["aaa"]->indexOf(gut2), 0);

    // remove child widget
    OK(w4->remove_child(w6));
    OK(!w6->get_parent());
    OK(!w4->has_child(w6));

    OK(w1->any_child_can_handle("baz"));
    OK(w1->any_child_can_handle("aaa"));
    OK(gut1->resp_children_ok);
    IS(gut1->resp_children.size(), 2);
    IS(gut1->resp_children["baz"]->size(), 1);
    OK(gut1->resp_children["baz"]->contains(gut4));
    IS(gut1->resp_children["aaa"]->size(), 1);
    OK(gut1->resp_children["aaa"]->contains(gut2));

    OK(w2->any_parent_can_handle("foo"));
    OK(w2->any_parent_can_handle("bar"));
    OK(w2->any_child_can_handle("baz"));
    OK(gut2->resp_children_ok);
    IS(gut2->resp_children.size(), 1);
    IS(gut2->resp_children["baz"]->size(), 1);
    OK(gut2->resp_children["baz"]->contains(gut4));
    OK(gut2->resp_parents_ok);
    IS(gut2->resp_parents.size(), 2);
    IS(gut2->resp_parents["foo"]->size(), 1);
    OK(gut2->resp_parents["foo"]->contains(gut1));
    IS(gut2->resp_parents["bar"]->size(), 1);
    OK(gut2->resp_parents["bar"]->contains(gut1));

    OK(w3->any_parent_can_handle("foo"));
    OK(w3->any_parent_can_handle("bar"));
    OK(w3->any_parent_can_handle("aaa"));
    OK(gut3->resp_parents_ok);
    IS(gut3->resp_parents.size(), 3);
    IS(gut3->resp_parents["foo"]->size(), 1);
    OK(gut3->resp_parents["foo"]->contains(gut1));
    IS(gut3->resp_parents["bar"]->size(), 1);
    OK(gut3->resp_parents["bar"]->contains(gut1));
    IS(gut3->resp_parents["aaa"]->size(), 1);
    OK(gut3->resp_parents["aaa"]->contains(gut2));

    OK(w4->any_parent_can_handle("foo"));
    OK(w4->any_parent_can_handle("bar"));
    OK(w4->any_parent_can_handle("aaa"));
    OK(gut4->resp_parents_ok);
    IS(gut4->resp_parents.size(), 3);
    IS(gut4->resp_parents["foo"]->size(), 1);
    OK(gut4->resp_parents["foo"]->contains(gut1));
    IS(gut4->resp_parents["bar"]->size(), 1);
    OK(gut4->resp_parents["bar"]->contains(gut1));
    IS(gut4->resp_parents["aaa"]->size(), 1);
    OK(gut4->resp_parents["aaa"]->contains(gut2));

    OK(w5->any_parent_can_handle("foo"));
    OK(w5->any_parent_can_handle("bar"));
    OK(w5->any_parent_can_handle("baz"));
    OK(w5->any_parent_can_handle("aaa"));
    OK(gut5->resp_parents_ok);
    IS(gut5->resp_parents.size(), 4);
    IS(gut5->resp_parents["foo"]->size(), 1);
    IS(gut5->resp_parents["foo"]->indexOf(gut1), 0);
    IS(gut5->resp_parents["bar"]->size(), 1);
    IS(gut5->resp_parents["bar"]->indexOf(gut1), 0);
    IS(gut5->resp_parents["baz"]->size(), 1);
    IS(gut5->resp_parents["baz"]->indexOf(gut4), 0);
    IS(gut5->resp_parents["aaa"]->size(), 1);
    IS(gut5->resp_parents["aaa"]->indexOf(gut2), 0);

    IS(gut6->resp_parents.size(), 0);

    OK(w1->remove_child(w2));
    OK(!w1->has_child(w2));
    OK(!w2->get_parent());

    IS(gut1->resp_children.size(), 0);

    IS(gut2->resp_parents.size(), 0);
    OK(gut2->resp_children_ok);
    IS(gut2->resp_children.size(), 1);
    IS(gut2->resp_children["baz"]->size(), 1);
    OK(gut2->resp_children["baz"]->contains(gut4));

    OK(w3->any_parent_can_handle("aaa"));
    OK(gut3->resp_parents_ok);
    IS(gut3->resp_parents.size(), 1);
    IS(gut3->resp_parents["aaa"]->size(), 1);
    OK(gut3->resp_parents["aaa"]->contains(gut2));

    OK(w4->any_parent_can_handle("aaa"));
    OK(gut4->resp_parents_ok);
    IS(gut4->resp_parents.size(), 1);
    IS(gut4->resp_parents["aaa"]->size(), 1);
    OK(gut4->resp_parents["aaa"]->contains(gut2));

    OK(w5->any_parent_can_handle("baz"));
    OK(w5->any_parent_can_handle("aaa"));
    OK(gut5->resp_parents_ok);
    IS(gut5->resp_parents.size(), 2);
    IS(gut5->resp_parents["baz"]->size(), 1);
    IS(gut5->resp_parents["baz"]->indexOf(gut4), 0);
    IS(gut5->resp_parents["aaa"]->size(), 1);
    IS(gut5->resp_parents["aaa"]->indexOf(gut2), 0);
}
