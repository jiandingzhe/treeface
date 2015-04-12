#include "treeface/resourcemanager.h"

#include <treejuce/File.h>
#include <treejuce/HashMap.h>
#include <treejuce/HashSet.h>
#include <treejuce/InputStream.h>
#include <treejuce/MemoryInputStream.h>
#include <treejuce/Result.h>
#include <treejuce/ZipFile.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct ResourceManager::Impl
{
    treejuce::HashSet<treejuce::ZipFile*> m_packages;
    treejuce::HashMap<treejuce::String, PackageEntryPoint> m_name_pkg_map;
};

juce_ImplementSingleton(ResourceManager)

void ResourceManager::add_package(treejuce::ZipFile* pkg, PackageItemConflictPolicy pol)
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

void ResourceManager::add_package(const void* zip_data, size_t zip_data_size, PackageItemConflictPolicy pol)
{
    MemoryInputStream* mem_stream = new MemoryInputStream(zip_data, zip_data_size, false);
    ZipFile* pkg = new ZipFile(mem_stream, true);
    add_package(pkg, pol);
}

void ResourceManager::add_package(const treejuce::File& zip_file, PackageItemConflictPolicy pol)
{
    ZipFile* pkg = new ZipFile(zip_file);
    add_package(pkg, pol);
}

treejuce::InputStream* ResourceManager::get_resource(const treejuce::String& name)
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

bool ResourceManager::has_resource(const treejuce::String& name) const NOEXCEPT
{
    return m_impl->m_name_pkg_map.contains(name);
}

ResourceManager::ResourceManager(): m_impl(new Impl())
{
}

ResourceManager::~ResourceManager()
{
    HashSet<ZipFile*>::Iterator it(m_impl->m_packages);
    while (it.next())
    {
        delete it.get();
    }

    delete m_impl;
}

TREEFACE_NAMESPACE_END
