#include "treeface/ui/Widget.h"
#include "treeface/ui/guts/Widget_guts.h"

using namespace treecore;

namespace treeface {

Widget::Widget() noexcept
    : m_guts( new (std::nothrow) Guts() )
{
    m_guts->scene_node = new (std::nothrow) SceneNode();
}

Widget::Widget( SceneNode* node ) noexcept
    : m_guts( new (std::nothrow) Guts() )
{
    m_guts->scene_node = node;
}

Widget::~Widget()
{
    if (m_guts)
        delete m_guts;
}

bool Widget::add_child( Widget* child ) noexcept
{
    if (child->m_guts->parent)
        return false;

    if ( has_child( child ) )
        return false;

    // widget hierarchy
    m_guts->children.add( child );
    child->m_guts->parent = this;

    // scene graph hierarchy
    if ( !m_guts->scene_node->add_child( child->m_guts->scene_node ) )
        die( "failed to add child %p scene node %p into parent %p scene node %p",
             child, child->m_guts->scene_node.get(), this, m_guts->scene_node.get() );

    // update parent ability cache
    Guts::RespWidgetMap parent_data;
    m_guts->fetch_this_and_parent_ability( parent_data );
    child->m_guts->recur_build_or_prepend_update_resp_parent_cache( parent_data );

    // clear resp-child cache
    m_guts->recur_clear_resp_child_cache();

    return true;
}

bool Widget::remove_child( Widget* child ) noexcept
{
    int i_child = m_guts->children.indexOf( child );
    if (i_child == -1)
        return false;

    if (child->m_guts->parent != this)
        die( "parent %p has child %p, but child's parent is %p", this, child, child->m_guts->parent );

    // widget hierarchy
    m_guts->children.remove( i_child );
    child->m_guts->parent = nullptr;

    // scene graph hierarchy
    if ( !m_guts->scene_node->remove_child( child->m_guts->scene_node ) )
        die( "failed to remove child %p scene node %p from parent %p scene node %p",
             child, child->m_guts->scene_node.get(), this, m_guts->scene_node.get() );

    // clear parent ability cache in removed children
    child->m_guts->recur_clear_resp_parent_cache();

    // clear chilren ability cache
    m_guts->recur_clear_resp_child_cache();

    return true;
}

bool Widget::has_child( Widget* child ) const noexcept
{
    return m_guts->children.contains( child );
}

Widget* Widget::get_parent() noexcept
{
    return m_guts->parent;
}

SceneNode* Widget::get_scene_node() noexcept
{
    return m_guts->scene_node;
}

bool Widget::add_event_listener( const treecore::Identifier& event_name, EventFunc func, void* data ) noexcept
{
    // store callback and user data
    EventArray* curr_handlers = nullptr;
    if ( m_guts->abilities.contains( event_name ) )
    {
        curr_handlers = m_guts->abilities[event_name];

        // find if func is already registered in this event name
        for (int i = 0; i < curr_handlers->size(); i++)
        {
            if ( (*curr_handlers)[i].fp == func )
                return false;
        }
    }
    else
    {
        curr_handlers = new EventArray();
        m_guts->abilities.set( event_name, curr_handlers );
    }

    curr_handlers->add( { func, data } );

    // clear resp-child cache for parents
    if (m_guts->parent)
        m_guts->parent->m_guts->recur_clear_resp_child_cache();

    // clear resp-parent cache for all children
    for (int i = 0; i < m_guts->children.size(); i++)
    {
        Widget* child = m_guts->children[i];
        child->m_guts->recur_clear_resp_parent_cache();
    }

    return true;
}

bool Widget::remove_event_listener( const treecore::Identifier& event_name )
{
    if ( !m_guts->abilities.contains( event_name ) )
        return false;

    m_guts->abilities.remove( event_name );

    if (m_guts->parent)
        m_guts->parent->m_guts->recur_remove_resp_child( event_name, m_guts );

    for (int i = 0; i < m_guts->children.size(); i++)
    {
        m_guts->children[i]->m_guts->recur_remove_resp_parent( event_name, m_guts );
    }

    return true;
}

bool Widget::remove_event_listener( const treecore::Identifier& event_name, EventFunc func )
{
    if ( !m_guts->abilities.contains( event_name ) )
        return false;

    EventArray* ev_list = m_guts->abilities[event_name];
    int         i_ev    = -1;
    for (int i = 0; i < ev_list->size(); i++)
    {
        if ( (*ev_list)[i].fp == func )
        {
            i_ev = i;
            break;
        }
    }

    if (i_ev == -1)
        return false;
    ev_list->remove( i_ev );

    // remove event registry if this is the last callback of the event
    if (ev_list->size() == 0)
    {
        m_guts->abilities.remove( event_name );

        if (m_guts->parent)
            m_guts->parent->m_guts->recur_remove_resp_child( event_name, m_guts );

        for (int i = 0; i < m_guts->children.size(); i++)
        {
            m_guts->children[i]->m_guts->recur_remove_resp_parent( event_name, m_guts );
        }
    }

    return true;
}

bool Widget::can_handle( const treecore::Identifier& event_name ) const noexcept
{
    return m_guts->abilities.contains( event_name );
}

bool Widget::has_handler( const treecore::Identifier& event_name, EventFunc func ) const noexcept
{
    if ( m_guts->abilities.contains( event_name ) )
    {
        EventArray* event_handlers = m_guts->abilities[event_name];

        for (int i = 0; i < event_handlers->size(); i++)
        {
            if ( (*event_handlers)[i].fp == func )
                return true;
        }

        return false;
    }
    else
    {
        return false;
    }
}

bool Widget::any_child_can_handle( const treecore::Identifier& event_name ) const noexcept
{
    m_guts->ensure_resp_child_cache();
    return m_guts->resp_children.contains( event_name );
}

bool Widget::any_parent_can_handle( const treecore::Identifier& event_name ) const noexcept
{
    m_guts->ensure_resp_parent_cache();
    return m_guts->resp_parents.contains( event_name );
}

} // namespace treeface
