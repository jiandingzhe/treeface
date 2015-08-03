#include "treeface/packagemanager.h"

#include <treecore/File.h>
#include <treecore/HashMap.h>
#include <treecore/HashSet.h>
#include <treecore/Holder.h>
#include <treecore/InputStream.h>
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
    treecore::HashSet<Holder<ZipFile>> m_packages;
    treecore::HashMap<treecore::String, PackageEntryPoint> m_name_pkg_map;
};

juce_ImplementSingleton(PackageManager)

void PackageManager::add_package(treecore::ZipFile* pkg, PackageItemConflictPolicy pol)
{
    if (!m_impl->m_packages.insert(pkg))
        return;

    printf("add package %p, has %d entries\n", pkg, pkg->getNumEntries());

    for (int index = 0; index < pkg->getNumEntries(); index++)
    {
        const ZipFile::ZipEntry* entry = pkg->getEntry(index);
        printf("  entry %s\n", entry->filename.toRawUTF8());

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

void PackageManager::add_package(const treecore::File& zip_file, PackageItemConflictPolicy pol)
{
    ZipFile* pkg = new ZipFile(zip_file);
    add_package(pkg, pol);
}

treecore::InputStream* PackageManager::get_item_stream(const treecore::String& name)
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

treecore::Result PackageManager::get_item_data(const treecore::String& name,
                                               treecore::MemoryBlock& data)
{
    InputStream* stream = get_item_stream(name);
    if (!stream)
        return Result::fail("PackageManager: failed to get item using name \""+name+"\"");

    size_t size = stream->getTotalLength();
    data.ensureSize(size + 1, false);

    int size_got = stream->read(data.getData(), size);
    if (size_got != size)
        die("PackageManager item %s size %lu, got %d",
            name.toRawUTF8(), size, size_got);

    // assign a zero on tail of data, so that it can be directly used as C string
    data[size] = 0;

    delete stream;
    return Result::ok();
}

bool PackageManager::has_resource(const treecore::String& name) const NOEXCEPT
{
    return m_impl->m_name_pkg_map.contains(name);
}

PackageManager::PackageManager(): m_impl(new Impl())
{
}

PackageManager::~PackageManager()
{
    if (m_impl)
        delete m_impl;
}

} // namespace treeface
