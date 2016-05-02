#ifndef TREEFACE_STEAKING_ARRAY_H
#define TREEFACE_STEAKING_ARRAY_H

#include <treecore/ArrayAllocationBase.h>
#include <treecore/DummyCriticalSection.h>
#include <treecore/RefCountObject.h>

#define _LOCK_THIS_OBJ_ const ScopedLockType _lock_this_obj_( m_data )
#define _LOCK_PEER_OBJ_ const ScopedLockType _lock_peer_obj_( peer.m_data )

class TestFramework;

namespace treeface
{

template<int align_size = 0, typename MutexType = treecore::DummyCriticalSection>
class SteakingArray: public treecore::RefCountObject
{
    friend class ::TestFramework;
    using int32 = treecore::int32;
    typedef typename MutexType::ScopedLockType ScopedLockType;

public:
    SteakingArray( treecore::int16 block_size ): m_blk_size( block_size ) {}

    SteakingArray( const SteakingArray& peer )
        : m_blk_size( peer.m_blk_size )
        , m_used_byte( peer.m_used_byte )
    {
        _LOCK_PEER_OBJ_;
        m_data.ensureAllocatedSize( m_used_byte );
        memcpy( m_data, peer.m_data, m_used_byte );
    }

    SteakingArray( SteakingArray&& peer )
        : m_blk_size( peer.m_blk_size )
        , m_used_byte( peer.m_used_byte )
        , m_data( std::move( peer.m_data ) )
    {}

    SteakingArray& operator = ( const SteakingArray& peer )
    {
        _LOCK_THIS_OBJ_;
        _LOCK_PEER_OBJ_;
        treecore_assert( m_blk_size == peer.m_blk_size );
        m_used_byte = peer.m_used_byte;
        m_data.ensureAllocatedSize( m_used_byte );
        memcpy( m_data, peer.m_data, m_used_byte );
    }

    SteakingArray& operator = ( SteakingArray&& peer )
    {
        _LOCK_THIS_OBJ_;
        _LOCK_PEER_OBJ_;
        treecore_assert( m_blk_size == peer.m_blk_size );
        m_used_byte = peer.m_used_byte;
        m_data      = std::move( peer.m_data );
    }

    virtual ~SteakingArray() {}

    void add_by_ptr( const void* data )
    {
        _LOCK_THIS_OBJ_;
        m_data.ensureAllocatedSize( m_used_byte + m_blk_size );
        memcpy( m_data.elements + m_used_byte, data, m_blk_size );
        m_used_byte += m_blk_size;
    }

    template<typename T>
    void add( const T& data )
    {
        treecore_assert( sizeof(T) == m_blk_size );
        add_by_ptr( &data );
    }

    void* get_by_ptr( int32 block_index ) noexcept
    {
        _LOCK_THIS_OBJ_;
        int32 byte_idx = block_index * m_blk_size;
        treecore_assert( byte_idx + m_blk_size <= m_used_byte );
        return m_data.elements + byte_idx;
    }

    const void* get_by_ptr( int32 block_index ) const noexcept
    {
        _LOCK_THIS_OBJ_;
        int32 byte_idx = block_index * m_blk_size;
        treecore_assert( byte_idx + m_blk_size <= m_used_byte );
        return m_data.elements + byte_idx;
    }

    template<typename T>
    T& get( int32 block_index ) noexcept
    {
        treecore_assert( sizeof(T) == m_blk_size );
        return *static_cast<T*>( get_by_ptr( block_index ) );
    }

    template<typename T>
    const T& get( int32 block_index ) const noexcept
    {
        treecore_assert( sizeof(T) == m_blk_size );
        return *static_cast<const T*>( get_by_ptr( block_index ) );
    }

    void* get_first_by_ptr() noexcept
    {
        _LOCK_THIS_OBJ_;
        treecore_assert( m_used_byte >= m_blk_size );
        return m_data.elements;
    }

    const void* get_first_by_ptr() const noexcept
    {
        _LOCK_THIS_OBJ_;
        treecore_assert( m_used_byte >= m_blk_size );
        return m_data.elements;
    }

    template<typename T>
    T& get_first() noexcept
    {
        treecore_assert( sizeof(T) == m_blk_size );
        return *static_cast<T*>( get_first_by_ptr() );
    }

    template<typename T>
    const T& get_first() const noexcept
    {
        treecore_assert( sizeof(T) == m_blk_size );
        return *static_cast<const T*>( get_first_by_ptr() );
    }

    void* get_last_by_ptr() noexcept
    {
        _LOCK_THIS_OBJ_;
        treecore_assert( m_used_byte >= m_blk_size );
        return m_data.elements + (m_used_byte - m_blk_size);
    }

    const void* get_last_by_ptr() const noexcept
    {
        _LOCK_THIS_OBJ_;
        treecore_assert( m_used_byte >= m_blk_size );
        return m_data.elements + (m_used_byte - m_blk_size);
    }

    template<typename T>
    T& get_last() noexcept
    {
        treecore_assert( sizeof(T) == m_blk_size );
        return *static_cast<T*>( get_last_by_ptr() );
    }

    template<typename T>
    const T& get_last() const noexcept
    {
        treecore_assert( sizeof(T) == m_blk_size );
        return *static_cast<T*>( get_last_by_ptr() );
    }

    void set_by_ptr( int32 block_index, const void* data )
    {
        _LOCK_THIS_OBJ_;
        int32 byte_idx = block_index * m_blk_size;
        treecore_assert( byte_idx + m_blk_size <= m_used_byte );
        memcpy( m_data.elements + byte_idx, data, m_blk_size );
    }

    template<typename T>
    void set( int32 block_index, const T& data )
    {
        treecore_assert( sizeof(T) == m_blk_size );
        set_by_ptr( block_index, &data );
    }

    void resize( int32 num_block )
    {
        _LOCK_THIS_OBJ_;
        int32 used_byte_old = m_used_byte;
        m_used_byte = num_block * m_blk_size;

        if      (m_used_byte > used_byte_old) m_data.ensureAllocatedSize( m_used_byte );
        else if (m_used_byte < used_byte_old) minimize_storage();
    }

    void clear()
    {
        _LOCK_THIS_OBJ_;
        m_data.setAllocatedSize( 0 );
        m_used_byte = 0;
    }

    void clear_quick() noexcept
    {
        _LOCK_THIS_OBJ_;
        m_used_byte = 0;
    }

    void* get_raw_data_ptr() noexcept
    {
        return m_data.elements;
    }

    int32 size() const noexcept
    {
        treecore_assert( m_used_byte % m_blk_size == 0 );
        return m_used_byte / m_blk_size;
    }

    int32 num_byte() const noexcept
    {
        return m_used_byte;
    }

    int32 block_size() const noexcept
    {
        return m_blk_size;
    }

protected:

    void minimize_storage()
    {
        if (m_data.numAllocated > m_used_byte * 2)
            m_data.shrinkToNoMoreThan( std::max( m_used_byte, 8 ) );
    }

    int32 m_used_byte = 0;
    const treecore::int16 m_blk_size;
    treecore::ArrayAllocationBase<treecore::int8, MutexType, align_size> m_data;
};

} // namespace treeface

#undef _LOCK_THIS_OBJ_
#undef _LOCK_PEER_OBJ_

#endif // TREEFACE_STEAKING_ARRAY_H
