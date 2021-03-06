#include "treeface/ui/guts/Widget_guts.h"

using namespace treecore;

namespace treeface {

void Widget::Guts::fetch_this_and_parent_ability( RespWidgetMap& result ) noexcept
{
    treecore_assert( result.size() == 0 );

    // copy resp-parent cache
    RespWidgetMap::Iterator it_parent_cache( resp_parents );
    while ( it_parent_cache.next() )
    {
        const Identifier& ev_name = it_parent_cache.key();
        WidgetRawArray*   curr    = new (std::nothrow) WidgetRawArray();
        result.set( ev_name, curr );
        *curr = *it_parent_cache.value();
    }

    // store ability
    EventMap::Iterator it_ev( abilities );
    while ( it_ev.next() )
    {
        const Identifier& ev_name = it_ev.key();

        // create slot if this event name is not exist
        WidgetRawArray* curr = nullptr;
        if ( result.contains( ev_name ) )
        {
            curr = result[ev_name];
        }
        else
        {
            curr = new (std::nothrow) WidgetRawArray();
            result.set( ev_name, curr );
        }

        curr->add( this );
    }
}

void Widget::Guts::recur_clear_resp_parent_cache() noexcept
{
    TREECORE_DBG( name.toString() + ": clear resp-parent cache\n" );
    resp_parents.clear();
    resp_parents_ok = false;
    for (int i = 0; i < children.size(); i++)
    {
        children[i]->m_guts->recur_clear_resp_parent_cache();
    }
}

void Widget::Guts::recur_clear_resp_child_cache() noexcept
{
    TREECORE_DBG( name.toString() + ": clear resp-child cache\n" );
    resp_children.clear();
    resp_children_ok = false;
    if (parent)
        parent->m_guts->recur_clear_resp_child_cache();
}

void Widget::Guts::recur_build_resp_parent_cache() noexcept
{
    build_resp_parent_cache();
    for (int i = 0; i < children.size(); i++)
    {
        children[i]->m_guts->recur_build_resp_parent_cache();
    }
}

void Widget::Guts::recur_build_resp_child_cache() noexcept
{
    build_resp_child_cache();
    if (parent)
        parent->m_guts->recur_build_resp_child_cache();
}

void Widget::Guts::recur_remove_resp_parent( const treecore::Identifier& ev_name, Guts* to_be_removed ) noexcept
{
    if (resp_parents_ok)
    {
        treecore_assert( resp_parents.contains( ev_name ) );
        WidgetRawArray* resp_list = resp_parents[ev_name];
        treecore_assert( resp_list->contains( to_be_removed ) );
        resp_list->removeAllInstancesOf( to_be_removed );
    }

    for (int i = 0; i < children.size(); i++)
    {
        children[i]->m_guts->recur_remove_resp_parent( ev_name, to_be_removed );
    }
}

void Widget::Guts::recur_remove_resp_child( const treecore::Identifier& ev_name, Guts* to_be_removed ) noexcept
{
    if (resp_children_ok)
    {
        treecore_assert( resp_children.contains( ev_name ) );
        WidgetRawArray* resp_list = resp_children[ev_name];
        treecore_assert( resp_list->contains( to_be_removed ) );
        resp_list->removeAllInstancesOf( to_be_removed );

        // remove resp child event name if it's the last one
        if (resp_list->size() == 0)
        {
            resp_children.remove( ev_name );
        }
    }

    if (parent)
        parent->m_guts->recur_remove_resp_child( ev_name, to_be_removed );
}

void Widget::Guts::recur_build_or_prepend_update_resp_parent_cache( const RespWidgetMap& prefix_data ) noexcept
{
    if (resp_parents_ok)
    {
        prepend_update_resp_parent_cache( prefix_data );
    }
    else
    {
        build_resp_parent_cache();
    }

    for (int i = 0; i < children.size(); i++)
    {
        children[i]->m_guts->recur_build_or_prepend_update_resp_parent_cache( prefix_data );
    }
}

void Widget::Guts::build_resp_parent_cache() noexcept
{
    TREECORE_DBG( name.toString() + ": update resp-parent cache\n" );
    treecore_assert( !resp_parents_ok );
    treecore_assert( resp_parents.size() == 0 );

    resp_parents_ok = true;

    if (!parent)
        return;

    if (!parent->m_guts->resp_parents_ok)
        parent->m_guts->build_resp_parent_cache();

    parent->m_guts->fetch_this_and_parent_ability( resp_parents );
}

void Widget::Guts::build_resp_child_cache() noexcept
{
    TREECORE_DBG( name.toString() + ": update resp-child cache\n" );
    treecore_assert( !resp_children_ok );
    treecore_assert( resp_children.size() == 0 );

    resp_children_ok = true;

    for (int i = 0; i < children.size(); i++)
    {
        Widget* child = children[i];

        if (!child->m_guts->resp_children_ok)
            child->m_guts->build_resp_child_cache();

        // record child to current widget
        EventMap::Iterator it_child_ev( child->m_guts->abilities );
        while ( it_child_ev.next() )
        {
            const Identifier& ev_name = it_child_ev.key();

            // create slot if this event name is not exist
            WidgetRawArray* curr = nullptr;
            if ( !resp_children.contains( ev_name ) )
            {
                curr = new (std::nothrow) WidgetRawArray();
                resp_children.set( ev_name, curr );
            }
            else
            {
                curr = resp_children[ev_name];
            }

            curr->add( child->m_guts );
        }

        // copy child's cache to current widget
        RespWidgetMap::Iterator it_child_resp( child->m_guts->resp_children );
        while ( it_child_resp.next() )
        {
            const Identifier& ev_name = it_child_resp.key();

            // create slot if this event name is not exist
            WidgetRawArray* curr = nullptr;
            if ( !resp_children.contains( ev_name ) )
            {
                curr = new (std::nothrow) WidgetRawArray();
                resp_children.set( ev_name, curr );
            }
            else
            {
                curr = resp_children[ev_name];
            }

            curr->addArray( *it_child_resp.value() );
        }
    }
}

void Widget::Guts::prepend_update_resp_parent_cache( const RespWidgetMap& prefix_data ) noexcept
{
    TREECORE_DBG( name.toString() + ": prepend update resp-parent cache\n" );
    treecore_assert( resp_parents_ok );

    RespWidgetMap::ConstIterator it( prefix_data );
    while ( it.next() )
    {
        const Identifier& ev_name = it.key();
        WidgetRawArray*   data    = it.value();

        if ( resp_parents.contains( ev_name ) )
        {
            WidgetRawArray* this_data = resp_parents[ev_name];
            this_data->insertArray( 0, data->getRawDataConstPointer(), data->size() );
        }
        else
        {
            WidgetRawArray* this_data = new (std::nothrow) WidgetRawArray();
            *this_data = *data;
            resp_parents.set( ev_name, this_data );
        }
    }
}

} // namespace treeface
