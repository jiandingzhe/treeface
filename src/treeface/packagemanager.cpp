#include "treeface/packagemanager.h"

#include <treejuce/File.h>
#include <treejuce/HashMap.h>
#include <treejuce/HashSet.h>
#include <treejuce/InputStream.h>
#include <treejuce/MemoryInputStream.h>
#include <treejuce/Result.h>
#include <treejuce/ZipFile.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct PackageManager::Impl
{
    treejuce::HashSet<treejuce::ZipFile*> m_packages;
    treejuce::HashMap<treejuce::String, PackageEntryPoint> m_name_pkg_map;
};

juce_ImplementSingleton(PackageManager)

void PackageManager::add_package(treejuce::ZipFile* pkg, PackageItemConflictPolicy pol)
{
    if (!m_impl->m_packages.insert(pkg))
        return;

    for (int index = 0; index < pkg->getNumEntries(); index++)
    {
        const ZipFile::ZipEntry* entry = pkg->getEntry(index);

        if (m_impl->m_name_pkg_map.contains(entry->filename))
        {
            switch(pol)
            {
            case KEEP_EXISTING:
                break;

            case OVERWRITE:
                m_impl->m_name_pkg_map.set(entry->filename, {pkg, index, entry->fileTime});
                break;

            case USE_OLDER:
                if (entry->fileTime < m_impl->m_name_pkg_map[entry->filename].time)
                    m_impl->m_name_pkg_map.set(entry->filename, {pkg, index, entry->fileTime});
                break;

            case USE_NEWER:
                if (m_impl->m_name_pkg_map[entry->filename].time < entry->fileTime)
                    m_impl->m_name_pkg_map.set(entry->filename, {pkg, index, entry->fileTime});
                break;

            default:
                abort();
            }
        }
        else
        {
            m_impl->m_name_pkg_map.set(entry->filename, {pkg, index, entry->fileTime});
        }
    }
}

void PackageManager::add_package(const void* zip_data, size_t zip_data_size, PackageItemConflictPolicy pol)
{
    MemoryInputStream* mem_stream = new MemoryInputStream(zip_data, zip_data_size, false);
    ZipFile* pkg = new ZipFile(mem_stream, true);
    add_package(pkg, pol);
}

void PackageManager::add_package(const treejuce::File& zip_file, PackageItemConflictPolicy pol)
{
    ZipFile* pkg = new ZipFile(zip_file);
    add_package(pkg, pol);
}

treejuce::InputStream* PackageManager::get_item_stream(const treejuce::String& name)
{
    if (m_impl->m_name_pkg_map.contains(name))
    {
        PackageEntryPoint entry = m_impl->m_name_pkg_map[name];
        return entry.package->createStreamForEntry(entry.entry_index);
    }
    else
    {
        return nullptr;
    }
}

ArrayRef<uint8> PackageManager::get_item_data(const treejuce::String& name)
{
    InputStream* stream = get_item_stream(name);
    if (!stream)
        return ArrayRef<uint8>();

    size_t size = stream->getTotalLength();
    uint8* data = (uint8*) malloc(size+1);

    if (stream->read(data, size) != size)
        abort();

    // assign a zero on tail of data, so that it can be used as C string
    data[size] = 0;

    delete stream;
    return ArrayRef<uint8>(data, size);
}

bool PackageManager::has_resource(const treejuce::String& name) const NOEXCEPT
{
    return m_impl->m_name_pkg_map.contains(name);
}

PackageManager::PackageManager(): m_impl(new Impl())
{
}

PackageManager::~PackageManager()
{
    HashSet<ZipFile*>::Iterator it(m_impl->m_packages);
    while (it.next())
    {
        delete it.get();
    }

    delete m_impl;
}

TREEFACE_NAMESPACE_END
