#ifndef TREEFACE_RESOURCE_MANAGER_H
#define TREEFACE_RESOURCE_MANAGER_H

#include <treejuce/Atomic.h>
#include <treejuce/Singleton.h>
#include <treejuce/String.h>
#include <treejuce/Time.h>

#include "treeface/common.h"

TREEFACE_JUCE_NAMESPACE_BEGIN
class File;
class InputStream;
class Result;
class ZipFile;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

class ResourceManager
{
public:
    typedef enum
    {
        KEEP_EXISTING,
        OVERWRITE,
        USE_OLDER,
        USE_NEWER
    } PackageItemConflictPolicy;

    juce_DeclareSingleton(ResourceManager, false);

    JUCE_DECLARE_NON_COPYABLE(ResourceManager);
    JUCE_DECLARE_NON_MOVABLE(ResourceManager);

    /**
     * @brief add a zip package into resource manager
     * @param pkg: the zip package object. It will be owned by resource manager,
     *        and when ResourceManager object is destructed, this ZipFile object
     *        will also be deleted. If the package object already added to the
     *        manager, the function will return immediately.
     * @param pol: how to act when the name of an item in the newly added
     *        package already exists in the resource manager.
     */
    void add_package(treejuce::ZipFile* pkg, PackageItemConflictPolicy pol);

    /**
     * @brief add a zip package by specifying zip data in memory.
     * @param zip_data: the pointer to the raw data of a zip archive.
     * @param zip_data_size: size of the raw data.
     * @param pol: how to act when the name of an item in the newly added
     *        package already exists in the resource manager.
     */
    void add_package(const void* zip_data, size_t zip_data_size, PackageItemConflictPolicy pol);

    /**
     * @brief add a zip package by specifying zip file.
     * @param zip_file_name: the zip file.
     * @param pol: how to act when the name of an item in the newly added
     *        package already exists in the resource manager.
     */
    void add_package(const treejuce::File& zip_file, PackageItemConflictPolicy pol);

    /**
     * @brief get_resource
     * @param name
     * @return
     */
    treejuce::InputStream* get_resource(const treejuce::String& name);

    bool has_resource(const treejuce::String& name) const NOEXCEPT;

protected:
    ResourceManager();
    virtual ~ResourceManager();

    struct Impl;

    struct PackageEntryPoint
    {
        treejuce::ZipFile* package;
        int entry_index;
        treejuce::Time time;
    };


    Impl* m_impl = nullptr;
}; // class ResourceManage

TREEFACE_NAMESPACE_END

#endif // TREEFACE_RESOURCE_MANAGER_H
