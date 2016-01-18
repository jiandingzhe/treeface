#include "treeface/packagemanager.h"

#include "treeface/misc/error.h"

#include <treecore/File.h>
#include <treecore/HashMap.h>
#include <treecore/HashSet.h>
#include <treecore/RefCountHolder.h>
#include <treecore/InputStream.h>
#include <treecore/JSON.h>
#include <treecore/MemoryBlock.h>
#include <treecore/MemoryInputStream.h>
#include <treecore/Result.h>
#include <treecore/StringRef.h>
#include <treecore/ZipFile.h>

using namespace treecore;

namespace treeface {

struct PackageEntryPoint
{
    treecore::ZipFile* package;
    int entry_index;
    treecore::Time time;
};

struct PackageManager::Impl
{
    treecore::HashSet<RefCountHolder<ZipFile> > m_packages;
    treecore::HashMap<treecore::String, PackageEntryPoint> m_name_pkg_map;
};

void PackageManager::add_package( treecore::ZipFile* pkg, PackageItemConflictPolicy pol )
{
    if ( !m_impl->m_packages.insert( pkg ) )
        return;

    printf( "add package %p, has %d entries\n", pkg, pkg->getNumEntries() );

    for (int index = 0; index < pkg->getNumEntries(); index++)
    {
        const ZipFile::ZipEntry* entry = pkg->getEntry( index );
        printf( "  entry %s\n", entry->filename.toRawUTF8() );

        if ( m_impl->m_name_pkg_map.contains( entry->filename ) )
        {
            switch (pol)
            {
            case KEEP_EXISTING:
                break;

            case OVERWRITE:
                m_impl->m_name_pkg_map.set( entry->filename, { pkg, index, entry->fileTime } );
                break;

            case USE_OLDER:
                if (entry->fileTime < m_impl->m_name_pkg_map[entry->filename].time)
                    m_impl->m_name_pkg_map.set( entry->filename, { pkg, index, entry->fileTime } );
                break;

            case USE_NEWER:
                if (m_impl->m_name_pkg_map[entry->filename].time < entry->fileTime)
                    m_impl->m_name_pkg_map.set( entry->filename, { pkg, index, entry->fileTime } );
                break;

            default:
                abort();
            }
        }
        else
        {
            m_impl->m_name_pkg_map.set( entry->filename, { pkg, index, entry->fileTime } );
        }
    }
}

void PackageManager::add_package( const void* zip_data, size_t zip_data_size, PackageItemConflictPolicy pol )
{
    MemoryInputStream* mem_stream = new MemoryInputStream( zip_data, zip_data_size, false );
    ZipFile* pkg = new ZipFile( mem_stream, true );
    add_package( pkg, pol );
}

void PackageManager::add_package( const treecore::File& zip_file, PackageItemConflictPolicy pol )
{
    ZipFile* pkg = new ZipFile( zip_file );
    add_package( pkg, pol );
}

treecore::InputStream* PackageManager::get_item_stream( const treecore::String& name )
{
    if ( m_impl->m_name_pkg_map.contains( name ) )
    {
        PackageEntryPoint entry = m_impl->m_name_pkg_map[name];
        return entry.package->createStreamForEntry( entry.entry_index );
    }
    else
    {
        return nullptr;
    }
}

bool PackageManager::get_item_data( const treecore::String& name, treecore::MemoryBlock& data, bool append_zero )
{
    InputStream* stream = get_item_stream( name );
    if (!stream)
        return false;

    size_t size = stream->getTotalLength();
    if (append_zero)
        data.ensureSize( size + 1, false );
    else
        data.ensureSize( size, false );

    int size_got = stream->read( data.getData(), int32(size) );
    if (size_got != size)
        die( "PackageManager item %s size %lu bytes, but only got %d bytes",
             name.toRawUTF8(), size, size_got );

    // assign a zero on tail of data, so that it can be directly used as C string
    if (append_zero)
        data[size] = 0;

    delete stream;
    return true;
}

treecore::var PackageManager::get_item_json( const treecore::String& name )
{
    // load JSON string data
    treecore::MemoryBlock json_src;
    if ( !get_item_data( name, json_src, true ) )
        return treecore::var::null;

    // parse JSON
    var root_node;
    {
        Result json_re = JSON::parse( (char*) json_src.getData(), root_node );
        if (!json_re)
            throw ConfigParseError( "PackageManager: failed to parse JSON content:\n" +
                                    json_re.getErrorMessage() + "\n\n" +
                                    String( "==== JSON source ====\n\n" ) +
                                    String( (char*) json_src.getData() ) + "\n" +
                                    String( "==== end of JSON source ====\n" ) );
    }

    return root_node;
}

bool PackageManager::has_resource( const treecore::String& name ) const noexcept
{
    return m_impl->m_name_pkg_map.contains( name );
}

PackageManager::PackageManager(): m_impl( new Impl() )
{}

PackageManager::~PackageManager()
{
    if (m_impl)
        delete m_impl;
}

} // namespace treeface
